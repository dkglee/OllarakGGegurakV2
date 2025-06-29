// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildProp.h"
#include "ChameleonProp.generated.h"

UCLASS()
class JUMPGAME_API AChameleonProp : public ABuildProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChameleonProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void CopyMeshAndMaterial();
	void RecoverMeshAndMaterial();

public:
	UPROPERTY()
	UStaticMesh* OriginalMesh{nullptr};
	UPROPERTY()
	TArray<UMaterialInterface*> OriginalMaterials{nullptr};
	UPROPERTY()
	FTransform OriginalWorldTransform{FTransform::Identity};
	UPROPERTY()
	FTransform OriginalRelativeTransform{FTransform::Identity};
};
