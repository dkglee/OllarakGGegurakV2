#pragma once

#include "CoreMinimal.h"
#include "GizmoPressedInfo.h"
#include "PressedHandlerInterface.h"
#include "Components/ActorComponent.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickResponse.h"
#include "PressedHandlerManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UPressedHandlerManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPressedHandlerManager();

	void RegisterHandler(TSharedPtr<IPressedHandler> Handler);
	bool HandlePressed(FClickResponse& ControlledInfo, class AMapEditingPlayerController* PlayerController);
	void InitializeSettings(FClickResponse& ControlledInfo, AMapEditingPlayerController* PlayerController);
	void ResetPositions();
	
	UFUNCTION()
	void OnWidgetDragging(FVector2D MousePosition);

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:
	// GC의 관리 대상이 아님
	// 주의 : 내부적으로 UObject를 캐싱하지 않기 : 즉 일회성으로만 사용
	TArray<TSharedPtr<IPressedHandler>> Handlers;

	FGizmoPressedInfo GizmoPressedInfo;

	UPROPERTY()
	class UClickHandlerManager* ClickHandlerManager = nullptr;
};
