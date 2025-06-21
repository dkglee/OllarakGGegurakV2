// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrimitiveProp/PrimitiveProp.h"
#include "BaseProp.generated.h"

class UBoxComponent;

UCLASS(Blueprintable)
class JUMPGAME_API ABaseProp : public APrimitiveProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetCollision(bool bEnable) override;

	virtual void MaterialChangeOnCollision() override;
	
	virtual void ChangeAllMaterialsToUnSelect() override;
	virtual void ChangeAllMaterialsToSelect() override;
	virtual void ChangeAllMaterialsToOnCollision() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 물체의 외관 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* PivotScene;

public:
	UFUNCTION(BlueprintCallable)
	void SetPropActive(bool Value);
	UFUNCTION(BlueprintCallable)
	void SetPropCheatMode(bool Value);

public:
	// 작동 정지
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsActive = true;

	// 치트키
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCheatMode = false;

	// 인게임일 때
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bInGame = false;
};
