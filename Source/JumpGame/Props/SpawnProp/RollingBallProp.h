// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "RollingBallProp.generated.h"

UCLASS()
class JUMPGAME_API ARollingBallProp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARollingBallProp();
	UFUNCTION()
	void OnMyRollingBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, FVector NormalImpulse,
							const FHitResult& Hit);

protected:
	UFUNCTION()
	void OnMyRollingBallOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                            bool bFromSweep, const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 오브젝트 풀링으로 관리될 오브젝트
	// 오브젝트 풀의 레퍼런스 초기화를 위한 함수 (어디소속인지)
	FORCEINLINE void SetObjectPool(class UObjectPoolComponent* InObjectPool) { ObjectPool = InObjectPool; }
	void ReturnSelf(float RemainTime);
	void SetActive(bool bIsActive);
	FORCEINLINE bool IsActive() { return bActive; }
	
private:
	UPROPERTY()
	bool bActive; // 현재 사용중인가요?
	UPROPERTY()
	float BackTime = 4.0f; // 몇초 지나면 돌아올지
	UPROPERTY()
	float LaunchTime = 0; // 몇초 지났니
	
	// 소속 풀
	UPROPERTY()
	class UObjectPoolComponent* ObjectPool;

public:
	// 외관
	UPROPERTY(visibleAnywhere, BlueprintReadWrite)
	USceneComponent* PivotScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
	UPROPERTY(visibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComp;
	UPROPERTY(visibleAnywhere,BlueprintReadWrite)
	class UArrowComponent* Arrow;

	// 날아가자
	FTimerHandle PoolTimerHandle;
	UPROPERTY()
	FVector LaunchDir;
	// 앞으로 날아가는 속도
	UPROPERTY()
	float RollingSpeed = 800.f;
	UFUNCTION()
	void LaunchProjectile();

	// 구르기
	UFUNCTION()
	void RollingBall();

	// 이펙트
	UFUNCTION(netmulticast, reliable)
	void MulticastRPC_PlayEffect(FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	class UParticleSystemComponent* HitEffectComp;
};
