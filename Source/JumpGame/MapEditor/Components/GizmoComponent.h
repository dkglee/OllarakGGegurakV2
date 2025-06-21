// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GizmoComponent.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UGizmoComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UGizmoComponent();

	UFUNCTION()
	virtual FVector GetDirection() const;
	UFUNCTION(BlueprintCallable)
	virtual void SetSelected();
	UFUNCTION(BlueprintCallable)
	virtual void SetUnSelected();

	virtual ~UGizmoComponent() override {}

	UFUNCTION()
	void ChangeColorByNewAxis(const FVector& NewAxis);
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	class UStaticMesh* GizmoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	class UMaterialInstance* GizmoMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	class UMaterialInstance* GizmoSelectedMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	class UMaterialInstanceDynamic* GizmoDynamicMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	class UMaterialInstanceDynamic* GizmoSelectedDynamicMaterial;

	UPROPERTY()
	bool bSelected = false;
};
