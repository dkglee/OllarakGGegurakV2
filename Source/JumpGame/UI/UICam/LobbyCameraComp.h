// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LobbyCameraComp.generated.h"

UENUM(BlueprintType)
enum class ECameraState : uint8
{
	Main,
	Sub,
	Stage,
	CustomGame
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeViewTarget, ECameraState, NewState);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API ULobbyCameraComp : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULobbyCameraComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 카메라 전환 관련
	UPROPERTY(editanywhere)
	class APlayerController* PC;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	class ALobbyMainCamera* MainCamera;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	class ALobbySubCamera* SubCamera;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	class ALobbyStageCamera* StageCamera;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	class ALobbyCustomCamera* CustomCamera;
	
	UFUNCTION()
	void SetViewTarget(ECameraState NewState);

	UPROPERTY(BlueprintAssignable)
	FOnChangeViewTarget OnChangeViewTarget;
	
};
