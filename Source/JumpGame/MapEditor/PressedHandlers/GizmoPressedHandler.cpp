// Fill out your copyright notice in the Description page of Project Settings.


#include "GizmoPressedHandler.h"

#include "BaseGizmos/GizmoMath.h"
#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/ClickHandlers/GizmoClickHandler.h"
#include "JumpGame/MapEditor/Components/GizmoPrimaryComponent.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/MapEditor/Components/RotateGizmoComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


UGizmoPressedHandler::UGizmoPressedHandler()
{
}

UGizmoPressedHandler::~UGizmoPressedHandler()
{
}

bool UGizmoPressedHandler::HandlePressed(FClickResponse& PressedResponse,
	class AMapEditingPlayerController* PlayerController, FGizmoPressedInfo& GizmoPressedInfo)
{
	if (PressedResponse.Result != EClickHandlingResult::GizmoEditing)
	{
		return false;
	}

	// 예외 처리: 잘못된 Gizmo 타입
	if (PressedResponse.TargetGizmo->IsA(UGizmoPrimaryComponent::StaticClass()) ||
		PressedResponse.TargetGizmo->IsA(URotateGizmoComponent::StaticClass()))
	{
		return false;
	}

	UGizmoComponent* Gizmo = PressedResponse.TargetGizmo;
	if (!Gizmo)
	{
		return false;
	}

	APrimitiveProp* LastSelected = FCommonUtil::SafeLast(PressedResponse.SelectedProps);
	if (!LastSelected)
	{
		return false;
	}
	
	TWeakObjectPtr<UGridComponent> GridComponent = LastSelected->GetGridComp();
	UGridComponent* Grid = GridComponent.Get();
	if (!Grid)
	{
		return false;
	}

	FHitResult HitResult;
	if (!PlayerController->OnPressedOperation(PressedType, HitResult))
	{
		return false;
	}

	FVector InitialRayOrigin = GizmoPressedInfo.InitialMouseRayOrigin;
	FVector InitialRayDirection = GizmoPressedInfo.InitialMouseRayDirection;
	
	FVector RayOrigin = HitResult.Location;
	FVector RayDirection = HitResult.Normal;

	// 교차점으로 중심을 옮김
	FVector GizmoOrigin = PressedResponse.SelectedProps.Last()->GetActorLocation();
	FVector GizmoDirection = Gizmo->GetDirection();
	
	// 1. 최초 클릭 시점의 레이와 기즈모 선분 사이 최근접점
	FVector InitialNearestOnLine, InitialNearestOnRay;
	float InitialLineT, InitialRayT;
	GizmoMath::NearestPointOnLineToRay(
		GizmoOrigin, GizmoDirection,
		InitialRayOrigin, InitialRayDirection,
		InitialNearestOnLine, InitialLineT,
		InitialNearestOnRay, InitialRayT
	);

	// 2. 현재 드래그 위치에서의 최근접점
	FVector CurrentNearestOnLine, CurrentNearestOnRay;
	float CurrentLineT, CurrentRayT;
	GizmoMath::NearestPointOnLineToRay(
		GizmoOrigin, GizmoDirection,
		RayOrigin, RayDirection,
		CurrentNearestOnLine, CurrentLineT,
		CurrentNearestOnRay, CurrentRayT
	);

	// 3. 이동 거리 = 선 위에서의 파라미터 차이만큼 이동
	float DeltaT = CurrentLineT - InitialLineT;
	FVector DeltaMove = GizmoDirection * DeltaT;
	
	// 4. Actor 이동 (다중 이동)
	for (APrimitiveProp* SelectedProp : PressedResponse.SelectedProps)
	{
		if (!SelectedProp || !SelectedProp->IsValidLowLevel())
		{
			continue;
		}
		if (!GizmoPressedInfo.OriginalActorLocations.Contains(SelectedProp))
		{
			continue;
		}
		FVector OriginalLocation = GizmoPressedInfo.OriginalActorLocations.FindRef(SelectedProp);
		FVector NewLocation = OriginalLocation + DeltaMove;

		UGridComponent* PropGrid = SelectedProp->GetGridComp();
		PropGrid->MoveByGizmo(NewLocation);
	}
	
	// FVector NewLocation = GizmoPressedInfo.OriginalActorLocation + DeltaMove;

	// GridComponent 이동 수행
	// Grid->MoveByGizmo(NewLocation);
	return true;
}
