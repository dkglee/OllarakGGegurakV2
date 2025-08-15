#include "GizmoComponent.h"

#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

UGizmoComponent::UGizmoComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Tick을 뷰파이 연산 뒤에서 실행하면, 카메라가 최신 위치로 갱신된 뒤 스케일을 조정
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GIZMO
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/SM_Gizmo2.SM_Gizmo2"));
	if (SM_GIZMO.Succeeded())
	{
		GizmoMesh = SM_GIZMO.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_Gizmo.MI_Gizmo"));
	if (MI_GIZMO.Succeeded())
	{
		GizmoMaterial = MI_GIZMO.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO_SELECTED
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_SelectedGizmo.MI_SelectedGizmo"));
	if (MI_GIZMO_SELECTED.Succeeded())
	{
		GizmoSelectedMaterial = MI_GIZMO_SELECTED.Object;
	}
	
	Super::SetStaticMesh(GizmoMesh);
	Super::SetCollisionProfileName(TEXT("GizmoPrefet"));
	Super::SetMaterial(0, GizmoMaterial);
	SetRelativeScale3D({0.5f, 0.25f, 0.5f});

	TranslucencySortPriority = 10;
	
	SetCastShadow(false);
}

FVector UGizmoComponent::GetDirection() const
{
	FVector Direction = GetUpVector();
	Direction.Normalize();
	return Direction;
}

void UGizmoComponent::SetSelected()
{
	bSelected = true;

	// Super::SetMaterial(0, GizmoSelectedDynamicMaterial);
	if (!GizmoDynamicMaterial) return;
	GizmoDynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), 1.0f);
}

void UGizmoComponent::SetUnSelected()
{
	bSelected = false;

	// Super::SetMaterial(0, GizmoDynamicMaterial);
	if (!GizmoDynamicMaterial) return;
	GizmoDynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), 0.5f);
}

void UGizmoComponent::ChangeColorByNewAxis(const FVector& NewAxis)
{
	if (!GizmoDynamicMaterial || !GizmoSelectedDynamicMaterial)
	{
		return;
	}
	FVector AbsoluteNewAxis = NewAxis.GetSafeNormal().GetAbs();
	FIntVector3 IntVecoVector3 = FIntVector3(
		FMath::RoundToInt(AbsoluteNewAxis.X),
		FMath::RoundToInt(AbsoluteNewAxis.Y),
		FMath::RoundToInt(AbsoluteNewAxis.Z)
		);
	// Z축이면 파란색
	if (IntVecoVector3.Z > 0)
	{
		GizmoDynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(0, 0, 1));
		GizmoSelectedDynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(0, 0, 1));
	}
	// X축이면 빨간색
	else if (IntVecoVector3.X > 0)
	{
		GizmoDynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(1, 0, 0));
		GizmoSelectedDynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(1, 0, 0));
	}
	// Y축이면 초록색
	else if (IntVecoVector3.Y > 0)
	{
		GizmoDynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(0, 1, 0));
		GizmoSelectedDynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(0, 1, 0));
	}
	// else
	{
		GizmoDynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(1, 1, 1));
		GizmoSelectedDynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(1, 1, 1));
	}
}

void UGizmoComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
	
	GizmoDynamicMaterial = UMaterialInstanceDynamic::Create(GizmoMaterial, this);
	GizmoSelectedDynamicMaterial = UMaterialInstanceDynamic::Create(GizmoSelectedMaterial, this);

	SetMaterial(0, GizmoDynamicMaterial);

	// ① 기준 스케일 보존
	BaselineScale = GetRelativeScale3D();              // {0.5,0.25,0.5}

	// 메시의 AABB 가로폭(‐X ~ +X) 구하기 → 원본 스케일 1 기준 길이
	if (const UStaticMesh* SM = GetStaticMesh())
	{
		const FBoxSphereBounds B = SM->GetBounds();
		MeshWidthUnit1 = B.BoxExtent.X * 2.f;          // 원본X폭
	}
}

void UGizmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 안전 체크
	if (MeshWidthUnit1 <= KINDA_SMALL_NUMBER) return;

	// ③ 카메라·거리·화면 해상도
	APlayerCameraManager* Cam = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (!Cam)
	{
		return;
	}

	const float Distance = FVector::Dist(Cam->GetCameraLocation(), GetComponentLocation());
	const float HalfFovRad = FMath::DegreesToRadians(Cam->GetFOVAngle() * 0.5f);

	FVector2D VPSize(1.f, 1.f);
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(VPSize);
	}
	if (VPSize.Y < 1.f)
	{
		return;
	}
	
	// 1픽셀이 차지하는 월드단위(cm) → 목표 폭(cm)
	const float UnitsPerPixel = 2.f * Distance * FMath::Tan(HalfFovRad) / VPSize.Y;
	const float DesiredWorldW = DesiredScreenWidthPx * UnitsPerPixel;

	// ‘현재 기본폭’ = 원본폭 × BaselineScale.X
	const float BaselineWorldW = MeshWidthUnit1 * BaselineScale.X;

	// 균일 팩터 (비율) 구해 각 축에 곱하기
	const float UniformFactor = DesiredWorldW / BaselineWorldW;

	FVector RawScale = BaselineScale * UniformFactor;

	// FVector MinScale = FVector::Min(BaselineScale, RawScale);
	FVector MaxScale = FVector::Max(BaselineScale, RawScale);

	FVector FinScale;
	FinScale.X = FMath::Clamp(RawScale.X, BaselineScale.X, MaxScale.X);
	FinScale.Y = FMath::Clamp(RawScale.Y, BaselineScale.Y, MaxScale.Y);
	FinScale.Z = FMath::Clamp(RawScale.Z, BaselineScale.Z, MaxScale.Z);
	
	// SetRelativeScale3D(BaselineScale * UniformFactor);	
	SetRelativeScale3D(FinScale);	
}
