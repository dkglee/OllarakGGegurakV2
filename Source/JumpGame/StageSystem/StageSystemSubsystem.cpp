#include "StageSystemSubsystem.h"

#include "JumpGame/Core/SaveGame/JumpSaveGame.h"
#include "JumpGame/Maps/Node/StageNodeActor.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

void UStageSystemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FSoftObjectPath StagePath(TEXT("/Game/StageSystem/Data/DT_StageDataTable.DT_StageDataTable"));
	const FSoftObjectPath FieldPath(TEXT("/Game/StageSystem/Data/DT_FieldDataTable.DT_FieldDataTable"));

	StageDataTable = Cast<UDataTable>(StagePath.TryLoad());
	FieldDataTable = Cast<UDataTable>(FieldPath.TryLoad());

	if (!StageDataTable || !FieldDataTable)
	{
		FFastLogger::LogConsole(TEXT("Failed to load Stage or Field DataTable!"));
		FFastLogger::LogFile(TEXT("StageSystem.txt"), TEXT("Failed to load Stage or Field DataTable!"));
		return;
	}
	
	BuildCaches();
	LoadProgressFromDisk();

	// TEST용
	ChosenStageID = TEXT("Stage_01");
	ChosenFieldID = TEXT("Field_01_01");
}

void UStageSystemSubsystem::Deinitialize()
{
	SaveProgressToDisk();
	StageCache.Empty();
	FieldCache.Empty();
	StageToFields.Empty();
	FieldStateCache.Empty();
	
	Super::Deinitialize();
}

const FStageTableRow* UStageSystemSubsystem::GetStage(const FName& StageID) const
{
	return StageCache.FindRef(StageID);
}

const FFieldTableRow* UStageSystemSubsystem::GetField(const FName& FieldID) const
{
	return FieldCache.FindRef(FieldID);
}

const TArray<FName>& UStageSystemSubsystem::GetFieldsInStage(const FName& StageID) const
{
	static const TArray<FName> EmptyArray;
	const TArray<FName>* Fields = StageToFields.Find(StageID);
	return Fields ? *Fields : EmptyArray;
}

FString UStageSystemSubsystem::ConvertFiledIDToPath(const FName& FieldID) const
{
	const FFieldTableRow* FieldRow = GetField(FieldID);
	if (!FieldRow)
	{
		FFastLogger::LogConsole(TEXT("Field not found for ID: %s"), *FieldID.ToString());
		FFastLogger::LogFile(TEXT("StageSystem.txt"), TEXT("Field not found for ID: %s"), *FieldID.ToString());
		return FString();
	}
	
	FString FilePath = FString::Printf(TEXT("%s%s/%s"), *DefaultPrefix, *FieldRow->StageID.ToString(), *FieldRow->FieldFilePath);
	
	FString FullPath = FPaths::ProjectContentDir() + FilePath;
	if (!FPaths::FileExists(FullPath))
	{
		FFastLogger::LogConsole(TEXT("Field file not found: %s"), *FullPath);
		FFastLogger::LogFile(TEXT("StageSystem.txt"), TEXT("Field file not found: %s"), *FullPath);
		return FString();
	}
	
	FString AbsolutePath = FPaths::ConvertRelativePathToFull(FullPath);
	FFastLogger::LogConsole(TEXT("Converted Field ID %s to path: %s"), *FieldID.ToString(), *AbsolutePath);
	return AbsolutePath;
}

void UStageSystemSubsystem::BuildCaches()
{
	if (!StageDataTable || !FieldDataTable)
	{
		FFastLogger::LogConsole(TEXT("Stage or Field DataTable is not set!"));
		return;
	}

	for (auto& Row : StageDataTable->GetRowMap())
	{
		const FName Key = Row.Key;
		auto* StageRow  = reinterpret_cast<FStageTableRow*>(Row.Value);
		StageCache.Add(Key, StageRow);
		StageToFields.Add(Key, StageRow->FieldLists);
	}

	for (auto& Row : FieldDataTable->GetRowMap())
	{
		const FName Key = Row.Key;
		auto* FieldRow  = reinterpret_cast<FFieldTableRow*>(Row.Value);
		FieldCache.Add(Key, FieldRow);

		if (!StageToFields.Contains(FieldRow->StageID))
		{
			StageToFields.Add(FieldRow->StageID, TArray<FName>());
		}
		StageToFields[FieldRow->StageID].AddUnique(FieldRow->FieldID);
	}
}

void UStageSystemSubsystem::SaveFieldResult(FName FieldID, int32 StarCount, float ClearTime)
{
	if (FFieldTableRow* FieldRow = FieldCache.FindRef(FieldID))
	{
		const int32 PrevStars = FieldRow->FieldStarCount;
		const float PrevTime = FieldRow->FieldClearTime;

		if (StarCount > PrevStars)
		{
			// 별이 더 많으면 무조건 별 + 시간 갱신 (별먹은게 우선순위)
			FieldRow->FieldStarCount = StarCount;
			FieldRow->FieldClearTime = ClearTime;
		}
		else if (StarCount == PrevStars && (PrevTime <= 0.f || ClearTime < PrevTime))
		{
			// 별은 같고, 시간이 더 빠르면 시간만 갱신
			FieldRow->FieldClearTime = ClearTime;
		}
	}
}

void UStageSystemSubsystem::SetFieldState(FName FieldID, EFieldProgressState NewState)
{
	if (!FieldCache.Contains(FieldID)) return;

	const EFieldProgressState* Cur = FieldStateCache.Find(FieldID);
	if (!Cur || *Cur != NewState)
	{
		FieldStateCache.Add(FieldID, NewState);
		SaveProgressToDisk();
	}
}

EFieldProgressState UStageSystemSubsystem::GetFieldState(FName FieldID) const
{
	if (const EFieldProgressState* Ptr = FieldStateCache.Find(FieldID))
		return *Ptr;
	return EFieldProgressState::None;
}

void UStageSystemSubsystem::SaveProgressToDisk()
{
	auto* SG = Cast<UJumpSaveGame>(UGameplayStatics::CreateSaveGameObject(UJumpSaveGame::StaticClass()));

	/*for (const auto& Pair : FieldCache)
	{
		FFieldTableRow* FieldRow = Pair.Value;
		if (!FieldRow) continue;

		SG->SavedFieldStarCountMap.Add(FieldRow->FieldID, FieldRow->FieldStarCount);
		SG->SavedFieldClearTimeMap.Add(FieldRow->FieldID, FieldRow->FieldClearTime);
	}*/
	SG->SavedFieldStarCountMap.Empty();
	SG->SavedFieldClearTimeMap.Empty();
	for (const auto& P : FieldCache)
	{
		const FFieldTableRow* Row = P.Value;
		SG->SavedFieldStarCountMap.Add(Row->FieldID, Row->FieldStarCount);
		SG->SavedFieldClearTimeMap.Add(Row->FieldID, Row->FieldClearTime);
	}

	SG->SavedFieldStateMap.Empty();
	for (const auto& P : FieldStateCache)
	{
		SG->SavedFieldStateMap.Add(P.Key, static_cast<uint8>(P.Value));
	}

	UGameplayStatics::SaveGameToSlot(SG, TEXT("StageSaveSlot"), 0);
}

void UStageSystemSubsystem::LoadProgressFromDisk()
{
	if (USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(TEXT("StageSaveSlot"), 0))
	{
		UJumpSaveGame* SG = Cast<UJumpSaveGame>(Loaded);
		if (!SG) return;

		/*for (auto& Pair : FieldCache)
		{
			FFieldTableRow* FieldRow = Pair.Value;
			if (!FieldRow) continue;

			if (SG->SavedFieldStarCountMap.Contains(FieldRow->FieldID))
			{
				FieldRow->FieldStarCount = SG->SavedFieldStarCountMap[FieldRow->FieldID];
			}

			if (SG->SavedFieldClearTimeMap.Contains(FieldRow->FieldID))
			{
				FieldRow->FieldClearTime = SG->SavedFieldClearTimeMap[FieldRow->FieldID];
			}
		}*/
		// 별·시간 복원
		for (auto& P : FieldCache)
		{
			FFieldTableRow* Row = P.Value;
			if (!Row) continue;
			Row->FieldStarCount = SG->SavedFieldStarCountMap.FindRef(Row->FieldID);
			Row->FieldClearTime = SG->SavedFieldClearTimeMap.FindRef(Row->FieldID);
		}

		FieldStateCache.Empty();
		for (auto& P : SG->SavedFieldStateMap)
		{
			FieldStateCache.Add(P.Key, static_cast<EFieldProgressState>(P.Value));
		}
	}
}

