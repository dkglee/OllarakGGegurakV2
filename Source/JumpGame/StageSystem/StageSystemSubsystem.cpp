#include "StageSystemSubsystem.h"

#include "JumpGame/Utils/FastLogger.h"

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
}

void UStageSystemSubsystem::Deinitialize()
{
	StageCache.Empty();
	FieldCache.Empty();
	StageToFields.Empty();
	
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


