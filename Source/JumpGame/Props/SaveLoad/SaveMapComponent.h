// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveData.h"
#include "Components/ActorComponent.h"
#include "Net/Core/NetBitArray.h"
#include "SaveMapComponent.generated.h"

#define SAVE_MAP_DEFAULT_DIRECTORY TEXT(R"(C:\Users\user\Desktop\)")

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API USaveMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USaveMapComponent();

	bool SaveMap(const FString& FileName, const FString& ImageBase64);

protected:
	virtual void BeginPlay() override;

private:
	void GetAllPropsInfo(TArray<FSaveData>& OutSaveDataArray);
	bool SaveDataToFile(const FSaveDataArray& InSaveDataArray, const FString& FileName);

	UPROPERTY()
	FSaveDataArray SaveDataArray;
	UPROPERTY()
	FString DefaultDirectory = SAVE_MAP_DEFAULT_DIRECTORY;

	UPROPERTY()
	TArray<FString> CollisionPropTags;
};
