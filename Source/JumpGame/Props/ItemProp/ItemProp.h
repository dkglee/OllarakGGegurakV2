// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpGame/Props/BaseProp.h"
#include "ItemProp.generated.h"

UCLASS()
class JUMPGAME_API AItemProp : public ABaseProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);

	UFUNCTION()
	void InitializeProp();
	// 먹었을 시
	UFUNCTION()
	virtual void EatItem();
	// 먹은 후 얻는 효과 ( ex. 점프력 강화 )
	UFUNCTION()
	virtual void EatUsefulEffect();
	// 먹은 후 시각 효과 ( ex. 반짝이 효과 )
	UFUNCTION()
	virtual void EatVisualEffect();
	// 지속 효과 ( ex. 회전 및 반짝이 효과 )
	UFUNCTION()
	virtual void LastingEffect();
	UFUNCTION()
	void RemoveItem();
};
