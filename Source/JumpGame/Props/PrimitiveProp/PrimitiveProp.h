#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "PrimitiveProp.generated.h"

UCLASS(Blueprintable)
class JUMPGAME_API APrimitiveProp : public AActor
{
	GENERATED_BODY()

public:
	APrimitiveProp();

protected:
	UFUNCTION()
	void OnGridPropBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);
	UFUNCTION()
	void OnGridPropEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;

public:
	bool IsOnCollision() const { return bIsOnCollision; }
	
	virtual void Tick(float DeltaTime) override;
	// 액터가 선택되었을 때 호출되는 함수
	// 내부 Mesh 컴포넌트의 색상 및 충돌 설정
	UFUNCTION(Blueprintable, BlueprintCallable)
	virtual void SetCollision(bool bEnable) {}

	UFUNCTION(BlueprintCallable)
	void SetSize(const FVector& InSize);
	UFUNCTION(BlueprintCallable)
	void SetSelected(bool bRotateGizmoMode);
	UFUNCTION(BlueprintCallable)
	void SetUnSelected();
	UFUNCTION(BlueprintCallable)
	void SetPrimitivePropCollision(bool bCond);
	UFUNCTION(BlueprintCallable)
	void SetGizmosCollision(bool bCond);
	UFUNCTION(BlueprintCallable)
	virtual void RotateAllGizmos();
	UFUNCTION(BlueprintCallable)
	void SetNewSizeByRotation(const FVector& InSize);

	GETTER(class UBoxComponent*, GridOuterCollision)
	GETTER(class UBoxComponent*, GridInnerCollision)
	GETTER(class UGridComponent*, GridComp);
	GETTER(class UPropDataComponent*, PropDataComponent);
	GETTER(class UGizmoComponent*, RotateGizmo);

	bool IsClickable() const { return bCanClick; }
	bool IsCollisionEnabled() const { return bEnableCollision; }

	void ShowRotateGizmo();;
	void ShowMoveGizmo();
	
protected:
	UFUNCTION()
	void HideGizmos();
	
	UFUNCTION()
	void SetGizmoLocation(class UGizmoComponent* Gizmo, const FVector& Direction, const FVector& InBoxSize);
	UFUNCTION()
	void SetGizmoRotation(class UGizmoComponent* Gizmo, const FVector& Direction);
	
	UFUNCTION()
	virtual void MaterialChangeOnCollision() {};
	UFUNCTION()
	virtual void ChangeAllMaterialsToUnSelect() {};
	virtual void ChangeAllMaterialsToSelect() {};
	virtual void ChangeAllMaterialsToOnCollision() {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGridComponent* GridComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoPrimary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoOne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoTwo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoThree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoFour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoFive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoSix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* RotateGizmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	TArray<class UGizmoComponent*> GizmoArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class UBoxComponent* GridOuterCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class UBoxComponent* GridInnerCollision;

	// 1 * 1 * 1 size
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	FVector DefaultSize = FVector(1.0f, 1.0f, 1.0f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	FVector DefaultCollisionExtentOffset = FVector(1, 1, 1);

	UPROPERTY()
	TArray<FVector> GizmoDirectionArray = {
		FVector(1, 0, 0),  // +X
		FVector(-1, 0, 0), // -X
		FVector(0, 1, 0),  // +Y
		FVector(0, -1, 0), // -Y
		FVector(0, 0, 1),  // +Z
		FVector(0, 0, -1)  // -Z
	};

	UPROPERTY()
	bool bSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PropDataComponent")
	class UPropDataComponent* PropDataComponent = nullptr;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetComponent", meta = (AllowPrivateAccess = "true"))
	// class UWidgetComponent* RotateWidgetComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetComponent", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* SelectedObjectMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetComponent", meta = (AllowPrivateAccess = "true"))
	TArray<class UMaterialInstanceDynamic*> UnSelectedObjectMaterials;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetComponent", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* OnCollisionObjectMaterial = nullptr;

	UPROPERTY()
	bool bIsOnCollision = false;
	UPROPERTY()
	int32 CollisionCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClickHandling", meta = (AllowPrivateAccess = "true"))
	bool bCanClick = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionEnable", meta = (AllowPrivateAccess = "true"))
	bool bEnableCollision = true;
};
