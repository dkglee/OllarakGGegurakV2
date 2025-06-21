// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleProp.h"
#include "SlipperyProp.generated.h"

UCLASS(Blueprintable)
class JUMPGAME_API ASlipperyProp : public AObstacleProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlipperyProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp,
	                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex) override;

	virtual void SetCollision(bool bEnable) override;


public:
	UPROPERTY()
	class AFrog* Frog;
};
