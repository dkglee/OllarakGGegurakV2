#include "SaveMapComponent.h"

#include "JsonObjectConverter.h"
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

	DefaultDirectory = FPaths::ProjectDir() + TEXT("AppData/Saved/");
	DefaultDirectory += TEXT(R"(SaveMap/)");
	FFastLogger::LogConsole(TEXT("DefaultDirectory : %s"), *DefaultDirectory);
}

bool USaveMapComponent::SaveMap(const FString& FileName, const FString& ImageBase64)
{
	CollisionPropTags.Empty();
	SaveDataArray.SaveDataArray.Empty();
	GetAllPropsInfo(SaveDataArray.SaveDataArray);
	if (CollisionPropTags.Num() != 0)
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("충돌중인 프롭이 있습니다! 저장 실패!!"));
		return false;
	}
	SaveDataArray.ImageBase64 = ImageBase64; // TODO: 이미지 저장 기능 추가
	return SaveDataToFile(SaveDataArray, FileName);
}

void USaveMapComponent::GetAllPropsInfo(TArray<FSaveData>& OutSaveDataArray)
{
	// 현재 월드에 있는 APrmitiveActor를 모두 가져옴
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APrimitiveProp::StaticClass(), AllActors);
	
	for (AActor* Actor : AllActors)
	{
		APrimitiveProp* PrimitiveProp = Cast<APrimitiveProp>(Actor);
		if (PrimitiveProp->IsOnCollision())
		{
			if (PrimitiveProp->Tags.Num() != 0)
			{
				CollisionPropTags.Add(PrimitiveProp->Tags.Last().ToString());
			}
			else
			{
				CollisionPropTags.Add(TEXT("NoTag"));
			}
			continue;
		}
		
		FSaveData SaveData;

		SaveData.Id = PrimitiveProp->GetPropDataComponent()->GetPropID();
		SaveData.Position = PrimitiveProp->GetActorLocation();
		SaveData.Rotation = PrimitiveProp->GetActorRotation();
		SaveData.Size = PrimitiveProp->GetGridComp()->GetSize();
		
		OutSaveDataArray.Add(SaveData);
	}
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

	if (bSuccess)
	{
		FFastLogger::LogScreen(FColor::Green, TEXT("성공적으로 저장했습니다! 경로: %s"), *FullPath);
	}
	else
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("저장 실패했습니다. 경로: %s"), *FullPath);
	}
	return bSuccess;
}
