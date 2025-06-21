#include "LoadMapComponent.h"

#include "JsonObjectConverter.h"
#include "JumpGame/MapEditor/CategorySystem/PropStruct.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "DesktopPlatformModule.h"
#include "Blueprint/UserWidget.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/UI/FileBrowser/FileBrowserUI.h"
#include "JumpGame/UI/MapEditing/MapLoadingUI.h"

ULoadMapComponent::ULoadMapComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UMapLoadingUI> MAP_LOADING_UI_CLASS
	(TEXT("/Game/UI/MapEditing/WBP_MapLoading.WBP_MapLoading_C"));
	if (MAP_LOADING_UI_CLASS.Succeeded())
	{
		MapLoadingUIClass = MAP_LOADING_UI_CLASS.Class;
	}
}

void ULoadMapComponent::BeginPlay()
{
	Super::BeginPlay();

	FileBrowserUI = CreateWidget<UFileBrowserUI>(GetWorld(), FileBrowserUIClass);
	FileBrowserUI->AddToViewport(999);
	MapLoadingUI = CreateWidget<UMapLoadingUI>(GetWorld(), MapLoadingUIClass);
}

void ULoadMapComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsLoading)
	{
		return ;
	}

	BuildMapFromSaveDataV2();
}

void ULoadMapComponent::OnPickFileComplete(const FString& FileName, bool bSuccess)
{
	FileBrowserUI->OnFileSelectedDelegate.Unbind();
	if (!bSuccess)
	{
		return ;
	}
	
	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	if (!GI)
	{
		return ;
	}
	GI->ClearMapFilePath();
	GI->SetMapFilePath(FileName);
}

void ULoadMapComponent::OnLoadFileComplete(const FString& FileName, bool bSuccess)
{
	FileBrowserUI->OnFileSelectedDelegate.Unbind();
	
	if (!bSuccess)
	{
		return ;
	}
	
	FString JsonString;

	FFastLogger::LogScreen(FColor::Red, TEXT("LoadMapComponent::OnLoadFileComplete : %s"), *FileName);
	
	if (!LoadFileToJsonString(FileName, JsonString))
	{
		return ;
	}
	if (!ParseJsonStringToMap(JsonString))
	{
		return ;
	}
	bIsLoading = true;
	if (bShowLoading)
	{
		MapLoadingUI->AddToViewport(99);
		MapLoadingUI->SetLoadingText(TEXT("맵 로딩 중..."));
		MapLoadingUI->PlayLoadingAnim();
	}
	//
	// BuildMapFromSaveData();
}

void ULoadMapComponent::LoadMap()
{
	FFastLogger::LogScreen(FColor::Red, TEXT("LoadMapComponent::LoadMap"));
	FileBrowserUI->SetVisibility(ESlateVisibility::Visible);
	FileBrowserUI->OnFileSelectedDelegate.BindUObject(this, &ULoadMapComponent::OnLoadFileComplete);

	FString RelativeDir = FPaths::ProjectDir() + TEXT("AppData/Content/Maps/");

	FString AbsoluteDir = FPaths::ConvertRelativePathToFull(RelativeDir);
	FPaths::MakePlatformFilename(AbsoluteDir);
	
	FileBrowserUI->SetSuffixes({TEXT(".json")});
	FileBrowserUI->LoadDirectoryContents(AbsoluteDir);
}

void ULoadMapComponent::LoadMapWithString(const FString& FileName)
{
	FString JsonString;

	FFastLogger::LogFile(TEXT("LoadMapComponent"), TEXT("LoadFile : %s"), *FileName);
	if (!LoadFileToJsonString(FileName, JsonString))
	{
		return ;
	}
	if (!ParseJsonStringToMap(JsonString))
	{
		return ;
	}
	bIsLoading = true;
	//
	// BuildMapFromSaveData();
}

void ULoadMapComponent::PickFile(const FString& Suffix, bool bBindFunction)
{
	FileBrowserUI->SetVisibility(ESlateVisibility::Visible);
	if (bBindFunction)
	{
		FileBrowserUI->OnFileSelectedDelegate.Unbind();
		FileBrowserUI->OnFileSelectedDelegate.BindUObject(this, &ULoadMapComponent::OnPickFileComplete);
	}

	FString RelativeDir = FPaths::ProjectDir() + TEXT("AppData/Content/Maps/");

	FString AbsoluteDir = FPaths::ConvertRelativePathToFull(RelativeDir);
	FPaths::MakePlatformFilename(AbsoluteDir);
	
	FileBrowserUI->SetSuffixes({Suffix});
	FileBrowserUI->LoadDirectoryContents(AbsoluteDir);
}

bool ULoadMapComponent::LoadFileToJsonString(const FString& FilePath, FString& JsonString)
{
	// 파일에서 문자열 읽기
	if (FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		// UE_LOG(LogTemp, Log, TEXT("파일에서 JSON을 성공적으로 읽었습니다!"));
		// UE_LOG(LogTemp, Log, TEXT("JSON 문자열: %s"), *JsonString);

		// 이제 여기서 JsonString → FSaveDataArray 파싱도 가능함
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("파일을 읽는 데 실패했습니다: %s"), *FilePath);
	return false;
}

bool ULoadMapComponent::ParseJsonStringToMap(const FString& JsonString)
{
	SaveDataArray.SaveDataArray.Empty();
	if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &SaveDataArray, 0, 0))
	{
		// UE_LOG(LogTemp, Log, TEXT("JSON → 구조체 변환 성공"));
		return true;
	}
	// UE_LOG(LogTemp, Error, TEXT("JSON → 구조체 변환 실패"));
	return false;
}

void ULoadMapComponent::BuildMapFromSaveData()
{
	for (const FSaveData& SaveData : SaveDataArray.SaveDataArray)
	{
		FPropStruct* PropInfo = PropTable->FindRow<FPropStruct>(SaveData.Id, TEXT("LoadMap"), true);
		if (!PropInfo)
		{
			continue ;
		}
		TSubclassOf<APrimitiveProp> PropClass = PropInfo->PropClass;

		SpawnProp(PropClass, SaveData);	
	}
	OnMapLoaded.Broadcast();
}

void ULoadMapComponent::BuildMapFromSaveDataV2()
{
	// SaveData에 SaveDataArray의 배열에서 500개씩 가져와서 처리하면 될 듯
	for (uint32 i = CurrentChunkIndex * ChunkSize; i < (uint32)SaveDataArray.SaveDataArray.Num() && i < (CurrentChunkIndex + 1) * ChunkSize; ++i)
	{
		const FSaveData& SaveData = SaveDataArray.SaveDataArray[i];
		FPropStruct* PropInfo = PropTable->FindRow<FPropStruct>(SaveData.Id, TEXT("LoadMap"), true);
		if (!PropInfo)
		{
			continue ;
		}
		TSubclassOf<APrimitiveProp> PropClass = PropInfo->PropClass;

		SpawnProp(PropClass, SaveData);
	}
	
	if ((CurrentChunkIndex + 1) * ChunkSize >= (uint32)SaveDataArray.SaveDataArray.Num())
	{
		bIsLoading = false;
		CurrentChunkIndex = 0;
		OnMapLoaded.Broadcast();
		if (bShowLoading)
		{
			MapLoadingUI->RemoveFromParent();
		}
	}
	else
	{
		CurrentChunkIndex++;
	}
}

void ULoadMapComponent::SpawnProp(TSubclassOf<APrimitiveProp> PropClass, const FSaveData& SaveData)
{
	FVector Location = SaveData.Position;
	FRotator Rotation = SaveData.Rotation;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Location);
	SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
	SpawnTransform.SetScale3D({1.f, 1.f, 1.f});
	
	FVector Size = SaveData.Size;

	// APrimitiveProp* NewProp = GetWorld()->SpawnActorDeferred<APrimitiveProp>(PropClass,	SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	// if (NewProp)
	// {
	// 	// NewProp->SetSize(Size);
	// 	NewProp->FinishSpawning(SpawnTransform);
	// 	NewProp->GetGridComp()->SetSize(Size);
	// 	NewProp->SetActorRotation(Rotation);
	// 	NewProp->RotateAllGizmos();
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Prop Spawn 실패"));
	// }
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APrimitiveProp* NewProp = GetWorld()->SpawnActor<APrimitiveProp>(PropClass, SpawnTransform, SpawnParameters);
	if (NewProp)
	{
		NewProp->GetGridComp()->SetSize(Size);
		NewProp->SetActorRotation(Rotation);
		NewProp->RotateAllGizmos();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Prop Spawn 실패"));
	}
}

void ULoadMapComponent::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes,
	TArray<FString>& OutFileNames)
{
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				//Opening the file picker!
				uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
				DesktopPlatform->OpenFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNames);
			}
		}
	}
}

void ULoadMapComponent::PrintData(const FString& File)
{
	//Parse the data into a string array
	FFastLogger::LogScreen(FColor::Red, TEXT("File: %s"), *File);
	
	TArray<FString> LoadedText;
	FFileHelper::LoadFileToStringArray(LoadedText, *File);
	//Print the contents
	for (int32 i = 0; i < LoadedText.Num(); i++)
	{
		// GLog->Log(LoadedText[i]);
		FFastLogger::LogScreen(FColor::Red, TEXT("%s"), *LoadedText[i]);
	}
}
