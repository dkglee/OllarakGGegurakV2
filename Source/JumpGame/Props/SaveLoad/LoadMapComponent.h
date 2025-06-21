#pragma once

#include "CoreMinimal.h"
#include "SaveData.h"
#include "Components/ActorComponent.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "LoadMapComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapLoaded);
// Delegate를 두어서 맵이 전부 로드 되었다는 것을 알려주는 것이 좋을 듯 함.

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API ULoadMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULoadMapComponent();

	UFUNCTION()
	void LoadMap();
	UFUNCTION()
	void LoadMapWithString(const FString& FileName);
	UFUNCTION()
	void PickFile(const FString& Suffix, bool bBindFunction = true);

	UFUNCTION()
	void OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames);
	UFUNCTION()
	void PrintData(const FString& File);
	void ShowLoading(bool bInShowLoading) { bShowLoading = bInShowLoading; };

	UPROPERTY(BlueprintAssignable)
	FOnMapLoaded OnMapLoaded;

	void OnPickFileComplete(const FString& FileName, bool bSuccess);
	void OnLoadFileComplete(const FString& FileName, bool bSuccess);

	GETTER(class UFileBrowserUI*, FileBrowserUI);
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	bool LoadFileToJsonString(const FString& FilePath, FString& OutString);
	UFUNCTION()
	bool ParseJsonStringToMap(const FString& JsonString);
	UFUNCTION()
	void BuildMapFromSaveData();
	UFUNCTION()
	void BuildMapFromSaveDataV2();
	UFUNCTION()
	void SpawnProp(TSubclassOf<class APrimitiveProp> PropClass, const FSaveData& SaveData);

	UPROPERTY()
	FSaveDataArray SaveDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapLoad", meta = (AllowPrivateAccess = "true"))
	class UDataTable* PropTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapLoadUI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UFileBrowserUI> FileBrowserUIClass = nullptr;
	UPROPERTY()
	class UFileBrowserUI* FileBrowserUI = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapLoadUI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMapLoadingUI> MapLoadingUIClass = nullptr;
	UPROPERTY()
	class UMapLoadingUI* MapLoadingUI = nullptr;
	

	UPROPERTY()
	bool bIsLoading = false;
	UPROPERTY()
	uint32 ChunkSize = 100;
	UPROPERTY()
	uint32 CurrentChunkIndex = 0;
	UPROPERTY()
	bool bShowLoading = false;
};
