#include "SaveMapComponent.h"

#include "JsonObjectConverter.h"
#include "JumpGame/MapEditor/CategorySystem/PropStruct.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "Kismet/GameplayStatics.h"

USaveMapComponent::USaveMapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USaveMapComponent::BeginPlay()
{
	Super::BeginPlay();

	DefaultDirectory = FPaths::ProjectContentDir() + TEXT("Maps/CustomMap/");
	// DefaultDirectory += TEXT(R"(SaveMap/)");
	FFastLogger::LogConsole(TEXT("DefaultDirectory : %s"), *DefaultDirectory);
}

bool USaveMapComponent::SaveMap(const FString& FileName, const FString& ImageBase64)
{
	SaveDataArray.SaveDataArray.Empty();
	PropCountMap.clear();
	if (!GetAllPropsInfo(SaveDataArray.SaveDataArray))
	{
		return false;
	}
	SaveDataArray.ImageBase64 = ImageBase64; // TODO: 이미지 저장 기능 추가
	return SaveDataToFile(SaveDataArray, FileName);
}

bool USaveMapComponent::GetAllPropsInfo(TArray<FSaveData>& OutSaveDataArray)
{
	// 현재 월드에 있는 APrmitiveActor를 모두 가져옴
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APrimitiveProp::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		APrimitiveProp* PrimitiveProp = Cast<APrimitiveProp>(Actor);
		// 충돌 중인 Prop이 있는지
		if (PrimitiveProp->IsOnCollision())
		{
			return false;
		}
		
		FSaveData SaveData;

		SaveData.Id = PrimitiveProp->GetPropDataComponent()->GetPropID();
		
		FPropStruct* PropInfo = PropTable->FindRow<FPropStruct>(SaveData.Id, TEXT("LoadMap"), true);
		if (!PropInfo || PropInfo->bIsHidden)
		{
			continue;
		}

		if (PropCountMap.find(SaveData.Id.ToString()) == PropCountMap.end())
		{
			PropCountMap[SaveData.Id.ToString()] = std::make_pair(0, PropInfo->PropMaxCount);
		}
		PropCountMap[SaveData.Id.ToString()].first += 1;
		
		SaveData.Position = PrimitiveProp->GetActorLocation();
		SaveData.Rotation = PrimitiveProp->GetActorRotation();
		SaveData.Size = PrimitiveProp->GetGridComp()->GetSize();
		
		OutSaveDataArray.Add(SaveData);
	}

	return CheckNecessaryProps();
	// return true;
}

bool USaveMapComponent::SaveDataToFile(const FSaveDataArray& InSaveDataArray, const FString& FileName)
{
	// InSaveDataArray를 JSON으로 변환
	FString JsonString;

	FJsonObjectConverter::UStructToJsonObjectString(InSaveDataArray, JsonString);

	// 경로에서 디렉토리만 추출
	FString Directory = DefaultDirectory + FPaths::GetPath(FileName);
    
	// 디렉토리 없으면 생성
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		PlatformFile.CreateDirectoryTree(*Directory);
	}

	FString FullPath = Directory + FileName;
	// 확장자 추가
	if (!FullPath.EndsWith(TEXT(".json")))
	{
		FullPath += TEXT(".json");
	}
	// 저장
	bool bSuccess = FFileHelper::SaveStringToFile(JsonString, *FullPath);

	return bSuccess;
}


bool USaveMapComponent::CheckNecessaryProps()
{
	for (auto& It : PropCountMap)
	{
		if (It.second.second <= 0) // MaxCount가 0 이하면 무시
		{
			continue;	
		}
		// 0 보다 많으면 필수 갯수를 다 채우지 못함
		// 0 보다 작으면 더 많이 설치됨
		if (It.second.first != It.second.second)
		{
			return false;
		}
	}
	return true;
}