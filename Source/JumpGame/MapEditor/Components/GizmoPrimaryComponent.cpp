// Fill out your copyright notice in the Description page of Project Settings.


#include "GizmoPrimaryComponent.h"

UGizmoPrimaryComponent::UGizmoPrimaryComponent()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GIZMOPRIMARY
	(TEXT("/Engine/EngineMeshes/Sphere.Sphere"));
	if (SM_GIZMOPRIMARY.Succeeded())
	{
		GizmoMesh = SM_GIZMOPRIMARY.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO_PRIMARY
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_PrimaryGizmo.MI_PrimaryGizmo"));
	if (MI_GIZMO_PRIMARY.Succeeded())
	{
		GizmoMaterial = MI_GIZMO_PRIMARY.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO_PRIMARY_SELECTED
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_SelectedPrimaryGizmo.MI_SelectedPrimaryGizmo"));
	if (MI_GIZMO_PRIMARY_SELECTED.Succeeded())
	{
		GizmoSelectedMaterial = MI_GIZMO_PRIMARY_SELECTED.Object;
	}

	Super::SetStaticMesh(GizmoMesh);
	Super::SetMaterial(0, GizmoMaterial);
	SetRelativeScale3D({0.1f, 0.1f, 0.1f});
}
