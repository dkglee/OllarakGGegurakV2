#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "GridComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UGridComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UGridComponent();

	GETTER(float, SnapSize)
	GETTER(FVector, Size)

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	void SetSize(FVector NewSize);
	bool MoveByGizmoPrimary(FVector MouseLocation, const FHitResult& HitResult);
	void MoveByGizmo(const FVector& NewLocation);;
	void Rotate(const FVector& Direction);

private:
	void SwapSize(const FVector& Direction);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid", meta=(AllowPrivateAccess="true"))
	class AActor* UpdatedActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid", meta=(AllowPrivateAccess="true"))
	float RotateAngle = 90.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid", meta=(AllowPrivateAccess="true"))
	float SnapSize = 50.f;

	// 박스의 사이즈 : 항상 100의 배수
	// 예시 : 1x1x1 => 100x100x100
	// 예시 : Size * SnapSize * 2.0f => 1*1*1 * 50 * 2.0f = 100x100x100
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid", meta=(AllowPrivateAccess="true"))
	FVector Size = FVector(1.f, 1.f, 1.f);

	UPROPERTY()
	int32 X = 0;
	UPROPERTY()
	int32 Y = 0;
	UPROPERTY()
	int32 Z = 0;

	// 사용하지 않는 변수 : 일단 냅둠
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid", meta=(AllowPrivateAccess="true"))
	// FVector Offset = Size * SnapSize * 0.5f;
};
