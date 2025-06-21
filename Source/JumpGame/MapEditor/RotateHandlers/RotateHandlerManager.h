#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickResponse.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "RotateHandlerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAxisChanged, FVector, NewAxis);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API URotateHandlerManager : public UActorComponent
{
	GENERATED_BODY()

public:
	URotateHandlerManager();

	void HandleRotate(FClickResponse& ClickResponse);
	void HandleAxis(FVector InAxis, FClickResponse& ClickResponse);
	UFUNCTION()
	void ResetAxis();

	UPROPERTY(BlueprintAssignable)
	FOnAxisChanged OnAxisChanged;

	GETTER(FVector, Axis);
	
protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	
private:
	
	UPROPERTY()
	FVector Axis = FVector(0, 0, 1);
};
