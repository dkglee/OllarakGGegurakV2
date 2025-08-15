// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicProp.h"
#include "EndGameProp.generated.h"

UCLASS()
class JUMPGAME_API AEndGameProp : public ALogicProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEndGameProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						  const FHitResult& SweepResult);


};
