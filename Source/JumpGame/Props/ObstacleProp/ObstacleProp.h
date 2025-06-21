// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpGame/Props/BaseProp.h"
#include "ObstacleProp.generated.h"

class AFrog;

UCLASS(Blueprintable)
class JUMPGAME_API AObstacleProp : public ABaseProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObstacleProp();

// protected:
	UFUNCTION()
	void OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                      const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 디버깅
	UPROPERTY(EditAnywhere)
	bool bDebug = false;
	
	// Launch 관련
	// 장애물: 트램펄린, 튕기기막대, 회전망치
	// LaunchVelocity를 초기값으로 주면 튕겨내지 않음
	UPROPERTY(EditAnywhere, Category = "Launch")
	FVector LaunchVelocity	= FVector(0, 0, 0);
	// 서버에서
	virtual void LaunchCharacter(AFrog* Character, FVector Direction, float Force, bool bXYOverride = false, bool bZOverride = false);

	virtual void CalculateForce(AFrog* Character);

	// Rotate (자체 회전)관련
	// 장애물: 회전망치, 뿔망치, 굴러오는 공
	// UPROPERTY(ReplicatedUsing=OnRep_ObstacleRotate)
	FRotator DeltaRot;
	UPROPERTY(ReplicatedUsing=OnRep_ObstacleRotate)
	FRotator Rotation = FRotator(0, 0, 0);
	UPROPERTY(EditAnywhere, Category = "Rotate")
	float RotAngle = 0;
	UPROPERTY (EditAnywhere, Category = "Rotate")
	FRotator RotAxis = FRotator(0, 0, 0);
	// 서버에서
	virtual void ObstacleRotate();
	// 클라이언트에서
	UFUNCTION()
	void OnRep_ObstacleRotate();

	// 이펙트, 사운드 등
	UFUNCTION(NetMulticast, reliable, BlueprintCallable)
	virtual void MulticastRPC_PlayEffect(FVector Location, int32 Index = 0);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayHitEffect(int32 Index = 0);
	
protected:
	UPROPERTY(EditAnywhere, Category = "SoundCue", meta = (AllowPrivateAccess = "true"))
	class USoundCue* HitSound;
	UPROPERTY(EditAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* HitEffectComp;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bPlayHitEffect = false;
};

