// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EEditorWarningType.h"
#include "Components/ActorComponent.h"
#include "WarningPropManager.generated.h"

USTRUCT()
struct FWarningNecessaryPropList	
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<class APrimitiveProp*> NecessaryProps;

	FWarningNecessaryPropList(TArray<class APrimitiveProp*> InNecessaryProps = TArray<APrimitiveProp*>())
		: NecessaryProps(InNecessaryProps)
	{}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UWarningPropManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWarningPropManager();

	void RegisterNecessaryProp(class APrimitiveProp* Prop);
	void RegisterCollisionProp(APrimitiveProp* Prop);
	void UnRegisterCollisionProp(APrimitiveProp* Prop);
	void UnRegisterNecessaryProp(APrimitiveProp* Prop);
	void UnRegister(APrimitiveProp* Prop);

	void InitWarningManager(class UCategorySystem* CategorySystem, class UWarningUI* WarningUI);

	UFUNCTION()
	class APrimitiveProp* GetTargetProp();
	UFUNCTION()
	void ResetIndex();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetWarning(EEditorWarningType WarningType);
	
	EEditorWarningType CheckWarnings();

	bool CheckCollisionProps(EEditorWarningType& OutWarningType);
	bool CheckNecessaryProps(EEditorWarningType& OutWarningType);
	
	using FCheckWarnings = bool (UWarningPropManager::*)(EEditorWarningType&);
	TArray<FCheckWarnings> CheckWarningFunctions = {
		&UWarningPropManager::CheckCollisionProps,
		&UWarningPropManager::CheckNecessaryProps
	};

	UPROPERTY()
	TArray<TWeakObjectPtr<APrimitiveProp>> OnCollisionProps;
	UPROPERTY()
	TMap<FName, FWarningNecessaryPropList> NecessaryProps;
	UPROPERTY()
	class UWarningUI* CachedWarningUI = nullptr;
	UPROPERTY()
	class UCategorySystem* CachedCategorySystem = nullptr;

	UPROPERTY()
	int32 TotalNecessaryPropCount = 0;

	int32 CurrentIndex = 0;
	TMap<FName, int32> NecessaryIndexMap;
};
