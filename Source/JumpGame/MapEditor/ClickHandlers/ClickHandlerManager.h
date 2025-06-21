#pragma once

#include "CoreMinimal.h"
#include "ClickHandlerInterface.h"
#include "Components/ActorComponent.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "ClickHandlerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControlledPropChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UClickHandlerManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UClickHandlerManager();

	UFUNCTION()
	void RegisterHandler(class UClickHandlerInterface* Handler);
	bool HandleClick(class AMapEditingPlayerController* PlayerController);
	void ResetControl();

	// 처음 클릭될 때
	UFUNCTION()
	void OnPropSlotClicked(FName PropID, class UClass* InPropClass);
	// 액터가 생성되어야 하고 가리켜야 할는 경우
	UFUNCTION()
	void OnWidgetDragLeave();
	// 액터를 다시 파괴해야 함 : Widget이 다시 나와야 함
	UFUNCTION()
	void OnWidgetDragEnter();
	// 드래그가 취소된 경우
	UFUNCTION()
	void OnPropDragCancelled();

	GETTER(FClickResponse, ControlledClickResponse)
	SETTER(FClickResponse, ControlledClickResponse)
	SETTER(bool, bRotateGizmoMode)

	UPROPERTY()
	FOnControlledPropChanged OnControlledPropChanged;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:
	UPROPERTY()
	TArray<class UClickHandlerInterface*> Handlers;

	UPROPERTY()
	FClickResponse ControlledClickResponse;

	UPROPERTY()
	FName ControlledPropSlotID = NAME_None;

	UPROPERTY()
	TSubclassOf<class APrimitiveProp> ControlledPropClass = nullptr;

	bool bMouseEnterUI = false;
	bool bRotateGizmoMode = false;
};
