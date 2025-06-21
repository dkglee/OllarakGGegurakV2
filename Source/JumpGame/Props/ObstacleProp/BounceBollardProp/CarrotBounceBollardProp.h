// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpGame/Props/ObstacleProp/BounceBollardProp.h"
#include "CarrotBounceBollardProp.generated.h"

UCLASS()
class JUMPGAME_API ACarrotBounceBollardProp : public ABounceBollardProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACarrotBounceBollardProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnBollardHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void MulticastRPC_PlayEffect_Implementation(FVector Location, int32 Index = 0) override;
	void ResetHitFrogList(class AFrog* Frog);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle CoolTimeHandle;
	float CoolTime = 0.5f;

	UPROPERTY(EditAnywhere)
	TArray<AFrog*> HitFrogList;
};
