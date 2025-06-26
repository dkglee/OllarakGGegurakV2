// Fill out your copyright notice in the Description page of Project Settings.


#include "CopyPressedHandler.h"

#include "BaseGizmos/GizmoMath.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/CategorySystem/CategorySystem.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/Components/GizmoPrimaryComponent.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/MapEditor/Components/RotateGizmoComponent.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"

bool UCopyPressedHandler::HandlePressed(FClickResponse& PressedResponse, AMapEditingPlayerController* PlayerController,
                                        FGizmoPressedInfo& GizmoPressedInfo)
{
	// 기즈모 클릭 상태가 아닌 경우
	if (PressedResponse.Result != EClickHandlingResult::GizmoEditing)
	{
		return false;
	}

	// 이동 기즈모가 아닌 경우 처리하지 않음
	if (PressedResponse.TargetGizmo->IsA(UGizmoPrimaryComponent::StaticClass()) ||
		PressedResponse.TargetGizmo->IsA(URotateGizmoComponent::StaticClass()))
	{
		return false;
	}

	// 복사 모드가 활성화되지 않은 경우
	// 이미 한 번 복제된 경우에도 처리하지 않음
	if (!GizmoPressedInfo.Has(FGizmoPressedInfo::CopyMode) ||
		GizmoPressedInfo.Has(FGizmoPressedInfo::AlreadyCopied))
	{
		return false;
	}
	
	// 예외 처리: 선택된 Prop가 없는 경우
	if (PressedResponse.SelectedProps.Num() == 0)
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

	if (DeltaMove.Length() >= Grid->GetSnapSize())
	{
		DuplicateSelectedProps(PressedResponse, PlayerController, GizmoPressedInfo, DeltaMove);	
	}
	
	return true;
}

void UCopyPressedHandler::DuplicateSelectedProps(FClickResponse& PressedResponse,
	AMapEditingPlayerController* PlayerController, FGizmoPressedInfo& GizmoPressedInfo, FVector DeltaMove)
{
	TArray<APrimitiveProp*> NewProps;
	// 복제 처리함
	GizmoPressedInfo.Flags |= FGizmoPressedInfo::AlreadyCopied;

	// 선택된 Prop들을 복제
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

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(NewLocation);
		SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
		SpawnTransform.SetScale3D({1.f, 1.f, 1.f});

		// Prop 복제
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		APrimitiveProp* NewProp = GetWorld()->SpawnActor<APrimitiveProp>(SelectedProp->GetClass(), SpawnTransform, SpawnParameters);
		if (NewProp)
		{
			NewProp->GetGridComp()->SetSize(SelectedProp->GetGridComp()->GetSize());
			NewProp->SetActorRotation(SelectedProp->GetActorRotation());
			NewProp->RotateAllGizmos();

			NewProps.Add(NewProp);
		}
		AMapEditorState* EditorState = Cast<AMapEditorState>(GetWorld()->GetGameState());
		if (EditorState)
		{
			EditorState->GetCategorySystem()->DecrementPropCountByID(SelectedProp->GetPropDataComponent()->GetPropID());
		}

		NewProp->SetSelected(false);
		NewProp->DeactivateGizmos();
		if (SelectedProp == FCommonUtil::SafeLast(PressedResponse.SelectedProps))
		{
			NewProp->ActivateGizmos(false);
		}

		// 이전에 선택된 애들을 다 비활성화
		SelectedProp->SetUnSelected();
	}

	PressedResponse.SelectedProps.Empty();
	PressedResponse.SelectedProps.Append(NewProps);

	GizmoPressedInfo.OriginalActorLocations.Empty();
	// 새로 복제된 Prop들의 위치를 저장
	for (APrimitiveProp* NewProp : NewProps)
	{
		if (NewProp && NewProp->IsValidLowLevel())
		{
			GizmoPressedInfo.OriginalActorLocations.Add(NewProp, NewProp->GetActorLocation());
		}
	}
	
	// 새로 복제된 Prop들을 업데이트 해줌
	EditingPawn->GetClickHandlerManager()->SetControlledClickResponse(PressedResponse);
}
