// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicProp.h"
#include "RisingWaterProp.generated.h"

UENUM(BlueprintType)
enum class EWaterStateEnum : uint8
{
	None,
	Rise
};

/**
 * 
 */
UCLASS(Blueprintable)
class JUMPGAME_API ARisingWaterProp : public ALogicProp
{
	GENERATED_BODY()

public:
	ARisingWaterProp();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
	UFUNCTION()
	void OnBeginDeepOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp,
	                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBeginShallowOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBeginSurfaceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBeginDeadZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp,
							   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void SetCollision(bool bEnable) override;
	
public:
	UFUNCTION()
	void RiseWater(float DeltaTime);
	UFUNCTION()
	void StopRising(float Time = -1.f);
	UFUNCTION()
	void StartRising();
	UFUNCTION()
	void SetRisingSpeed(float Speed);
	float GetWaterSurfaceZ();

	UFUNCTION(NetMulticast, reliable, BlueprintCallable)
	void MulticastRPC_PlayEffect(FVector Location);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* DeepCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* ShallowCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* SurfaceCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* DeadZoneCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* RespawnPoint;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_WaterState)
	EWaterStateEnum WaterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Water State")
	float CurrentRisingSpeed;

	UFUNCTION()
	void OnRep_WaterState();
	
public:
	FTimerHandle ResumeRisingTimerHandle;
	FTimerHandle TimerHandle;
	FTimerHandle WaterStartTimer;

	UPROPERTY(EditAnywhere, Category = "SoundCue", meta = (AllowPrivateAccess = "true"))
	class USoundCue* HitSound;
};
