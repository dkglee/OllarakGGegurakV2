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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
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

	UPROPERTY(EditAnywhere, Category="Gizmo|Screen")
	float DesiredScreenWidthPx = 25.0f;        // 목표 가로 픽셀 수

	FVector BaselineScale = FVector::OneVector; // = {0.5,0.25,0.5}
	float MeshWidthUnit1 = 100.f;              // 스케일 1.0 때 X폭(cm)
};
