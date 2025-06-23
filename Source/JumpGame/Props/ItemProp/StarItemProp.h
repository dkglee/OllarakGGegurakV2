// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemProp.h"
#include "StarItemProp.generated.h"

UCLASS()
class JUMPGAME_API AStarItemProp : public AItemProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStarItemProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 별 먹음 -> 별 개수 카운트 증가
	virtual void EatItem() override;
	virtual void EatVisualEffect() override;
	virtual void LastingEffect() override;
	
};
