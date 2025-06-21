// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleProp.h"
#include "ConveyorBeltProp.generated.h"

UCLASS(Blueprintable)
class JUMPGAME_API AConveyorBeltProp : public AObstacleProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AConveyorBeltProp();

protected:
	virtual void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult) override;
	virtual void OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 이동 방향
	UPROPERTY(EditAnywhere)
	class UArrowComponent* Arrow;
	UPROPERTY()
	FVector BeltDir = FVector::ZeroVector;
	UPROPERTY()
	float BeltSpeed = 250.f;

	/*// 플레이어 캐스팅
	UPROPERTY()
	class AFrog* Character;*/
	// 여러 플레이어가 오버랩될 경우
	UPROPERTY()
	TSet<AFrog*> OverlappingFrogs;
	UPROPERTY()
	TSet<AFrog*> CopyOverlapFrogs;

	// 이동
	void ConveyorMove();

public:
	virtual void ChangeAllMaterialsToUnSelect() override;
	virtual void ChangeAllMaterialsToSelect() override;
	virtual void ChangeAllMaterialsToOnCollision() override;
	
	virtual void RotateAllGizmos() override;

	void UpdateWaterMaterialRotation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetComponent", meta = (AllowPrivateAccess = "true"))
	TArray<class UMaterialInstanceDynamic*> UnSelectedWaterMaterials;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WaterMeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInterface* ConveyorMaterial;
	UPROPERTY()
	class UMaterialInstanceDynamic* ConveyorDynamicMaterial;
};
