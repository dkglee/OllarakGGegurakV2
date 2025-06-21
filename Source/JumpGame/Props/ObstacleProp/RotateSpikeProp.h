// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleProp.h"
#include "RotateSpikeProp.generated.h"

// 회전 + 가시가 달려있는데 가시에 부딪히면 약하게 넉백

UCLASS(Blueprintable)
class JUMPGAME_API ARotateSpikeProp : public AObstacleProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARotateSpikeProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void MulticastRPC_PlayEffect_Implementation(FVector Location, int32 Index = 0) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetCollision(bool bEnable) override;
	virtual void CalculateForce(AFrog* Character) override;

public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Hammer;
	// 가시
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Spike;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwingHammer();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReverseSwingHammer();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cable", meta = (AllowPrivateAccess = "true"))
	class UCableComponent* CableComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cable", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CableMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cable", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CableTop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cable", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Propeller;

	UPROPERTY(ReplicatedUsing = OnRep_SwingHammer, BlueprintReadWrite, Category = "Hammer", meta = (AllowPrivateAccess = "true"), EditAnywhere)
	float HammerAngle;
	UFUNCTION()
	void OnRep_SwingHammer();
};