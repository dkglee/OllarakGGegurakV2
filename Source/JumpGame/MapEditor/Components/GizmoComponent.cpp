#include "GizmoComponent.h"

UGizmoComponent::UGizmoComponent()
{
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

	Super::SetMaterial(0, GizmoSelectedDynamicMaterial);
}

void UGizmoComponent::SetUnSelected()
{
	bSelected = false;

	Super::SetMaterial(0, GizmoDynamicMaterial);
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

	GizmoDynamicMaterial = UMaterialInstanceDynamic::Create(GizmoMaterial, this);
	GizmoSelectedDynamicMaterial = UMaterialInstanceDynamic::Create(GizmoSelectedMaterial, this);

	SetMaterial(0, GizmoDynamicMaterial);
}
