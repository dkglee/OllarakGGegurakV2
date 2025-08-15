#pragma once

#include "CoreMinimal.h"
#include "GizmoPressedInfo.h"
#include "PressedHandlerInterface.h"
#include "Components/ActorComponent.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickResponse.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "PressedHandlerManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UPressedHandlerManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPressedHandlerManager();

	void RegisterHandler(class UPressedHandlerInterface* Handler);
	bool HandlePressed(FClickResponse& ControlledInfo, class AMapEditingPlayerController* PlayerController);
	void InitializeSettings(FClickResponse& ControlledInfo, AMapEditingPlayerController* PlayerController);
	void ResetPositions();
	
	UFUNCTION()
	void OnWidgetDragging(FVector2D MousePosition);

	SETTER(bool, bCopyMode)
	
protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:
	UPROPERTY()
	TArray<class UPressedHandlerInterface*> Handlers;

	FGizmoPressedInfo GizmoPressedInfo;

	UPROPERTY()
	class UClickHandlerManager* ClickHandlerManager = nullptr;

	bool bCopyMode = false;
};
