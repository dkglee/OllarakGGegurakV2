// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpGame/Characters/Frog.h"
#include "LobbyFrog.generated.h"

class UStageMapNodeComponent;
class ULobbyCameraComp;

UENUM(BlueprintType)
enum class ELobbyCharacterState : uint8
{
	InLobby, // 로비 상태
	OnStageMap // 스테이지 상태 (이동 허용)
};

UCLASS()
class JUMPGAME_API ALobbyFrog : public AFrog
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALobbyFrog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float PitchRatio = 0.f;
	float YawRatio = 0.f;

	FVector2D PlusPitchMinMax = FVector2D(0.f, 50.f);
	FVector2D MinusPitchMinMax = FVector2D(0.f, 70.f);
	FVector2D MinMax = FVector2D(0.f, 70.f);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	ULobbyCameraComp* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStageMapNodeComponent* StageMapNodeComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELobbyCharacterState CurrentState;

	UPROPERTY(EditAnywhere, Category="Animation")
	TSubclassOf<class UAnimInstance> LobbyAnimBP;
	UPROPERTY(EditAnywhere, Category="Animation")
	TSubclassOf<class UAnimInstance> StageMapAnimBP;
	void UpdateAnimation(ELobbyCharacterState NewState);

	UFUNCTION()
	void RestoreNodePosition(); // 들어갔던 노드로 복귀하자
};

