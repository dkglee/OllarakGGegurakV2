// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditingPlayerController.h"

#include "JumpGame/MapEditor/DragDropOperation/WidgetMapEditDragDropOperation.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "JumpGame/Utils/CursorManager.h"
#include "Kismet/GameplayStatics.h"

AMapEditingPlayerController::AMapEditingPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bShowMouseCursor = true;

	HitResultTraceDistance = 10000.f;
}

void AMapEditingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UCursorManager::SetCursor(this, ECursorName::GreenCursor);

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);  // ★ 핵심! : 이렇게 해야 마우스가 창 밖으로 안나감
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}

bool AMapEditingPlayerController::OnClickOperation(APrimitiveProp* InControlledProp, FClickResponse& ClickResponse)
{
	if (ClickOperations.Contains(ClickResponse.Result))
	{
		bool bResult = (this->*ClickOperations[ClickResponse.Result])(InControlledProp, ClickResponse);
		if (bResult)
		{
			// DrawDebugSphere(GetWorld(), ClickResponse.MouseWorldPosition, 10.f, 12, FColor::Red, false, 5.f);
		}
		return bResult;
	}
	return false;
}

// Click Operation : Gizmo
bool AMapEditingPlayerController::OnGizmoClickOperation(APrimitiveProp* InControlledProp, FClickResponse& ClickResponse)
{
	FVector2D MouseScreenPosition;
	if (!GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y)) return false;

	// 선택되어 있는 액터가 없다면 false를 반환
	if (!InControlledProp) return false;
	
	// 충돌 체크를 진행한다.
	bool bResult = false;
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), true);
	GetMyHitResultAtScreenPosition(MouseScreenPosition, ECC_GameTraceChannel5, CollisionQueryParams, HitResults);
	for (auto& HitResult : HitResults)
	{
		if (HitResult.IsValidBlockingHit() && HitResult.GetComponent() && HitResult.GetComponent()->GetOwner() == InControlledProp)
		{
			bResult = true;
			ClickResponse.Result = EClickHandlingResult::GizmoEditing;
			ClickResponse.HitResult = HitResult;
			ClickResponse.MouseWorldPosition = HitResult.Location;
			break ;
		}
	}
	if (!bResult)
	{
		ClickResponse.Result = EClickHandlingResult::None;
		ClickResponse.HitResult = FHitResult();
		ClickResponse.MouseWorldPosition = FVector::ZeroVector;
	}
	
	return bResult;
}

// 최적화용 HitResult
bool AMapEditingPlayerController::GetMyHitResultAtScreenPosition(const FVector2D ScreenPosition, const ECollisionChannel TraceChannel, const FCollisionQueryParams& CollisionQueryParams, TArray<FHitResult>& OutHitResult) const
{
	FVector WorldOrigin;
	FVector WorldDirection;
	if (UGameplayStatics::DeprojectScreenToWorld(this, ScreenPosition, WorldOrigin, WorldDirection) == true)
	{
		// TODO: TroubleShooting
		// TraceChannel하고 ObjectType의 차이를 확인해야함
		return GetWorld()->LineTraceMultiByObjectType(OutHitResult, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, TraceChannel, CollisionQueryParams);
	}

	return false;
}


// Click Operation : Actor // Gizmo가 실패하고 나서 다시 시도
bool AMapEditingPlayerController::OnActorClickOperation(APrimitiveProp* InControlledProp, FClickResponse& ClickResponse)
{
	FVector2D MouseScreenPosition;
	if (!GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y)) return false;

	// 현재의 InControlledProp가 nullptr이 아니라면 해당 액터의 Collision을 켜준다.
	if (InControlledProp) 
	{
		InControlledProp->SetPrimitivePropCollision(true);
	}

	// 마우스 위치에 있는 액터를 HitResult로 가져온다.
	FHitResult HitResult;
	GetHitResultAtScreenPosition(MouseScreenPosition, ECC_Visibility, true, HitResult);
	
	if (InControlledProp)
	{
		InControlledProp->SetPrimitivePropCollision(false);
	}

	bool bResult = true;
	if (HitResult.IsValidBlockingHit())
	{
		bResult = true;
		ClickResponse.Result = EClickHandlingResult::ActorEditing;
		ClickResponse.HitResult = HitResult;
		ClickResponse.MouseWorldPosition = HitResult.Location;
	}
	else
	{
		bResult = false;
		ClickResponse.Result = EClickHandlingResult::None;
		ClickResponse.HitResult = FHitResult();
		ClickResponse.MouseWorldPosition = FVector::ZeroVector;
	}
	
	return bResult;
}

// Click Operation : Background
// 이전의 동작이 전부 실패했을 경우
bool AMapEditingPlayerController::OnBackgroundClickOperation(APrimitiveProp* InControlledProp, FClickResponse& ClickResponse)
{
	FVector2D MouseScreenPosition;
	if (!GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y)) return false;

	// 그 외에 공중에 마우스가 있는 경우
	FVector MouseLocation;
	FVector MouseDirection;
	UGameplayStatics::DeprojectScreenToWorld(this, MouseScreenPosition, MouseLocation, MouseDirection);

	ClickResponse.Result = EClickHandlingResult::BackgroundEditing;
	ClickResponse.HitResult = FHitResult();
	ClickResponse.MouseWorldPosition = MouseLocation + (MouseDirection * 1000.f);
	
	return true;
}

bool AMapEditingPlayerController::OnPressedOperation(const EPressedHandlingResult EPressedOperation, FHitResult& InHitResult)
{
	if (PressedOperations.Contains(EPressedOperation))
	{
		bool bResult = (this->*PressedOperations[EPressedOperation])(InHitResult);
		return bResult;
	}
	return false;
}

bool AMapEditingPlayerController::OnPropSlotPressedOperation(FHitResult& InHitResult)
{
	FVector2D MouseScreenPosition = MousePosition;
	
	FHitResult HitResult;
	GetHitResultAtScreenPosition(MouseScreenPosition, ECC_Visibility, true, HitResult);

	if (HitResult.IsValidBlockingHit())
	{
		InHitResult = HitResult;
		return true;
	}

	// 그 외에 공중에 마우스가 있는 경우
	FVector MouseLocation;
	FVector MouseDirection;
	UGameplayStatics::DeprojectScreenToWorld(this, MouseScreenPosition, MouseLocation, MouseDirection);

	HitResult.Location = MouseLocation + (MouseDirection * 1000.f);
	InHitResult = HitResult;
	
	return true;
}

bool AMapEditingPlayerController::OnGizmoPrimaryPressedOperation(FHitResult& InHitResult)
{
	FVector2D MouseScreenPosition;
	if (!GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y)) return false;

	FHitResult HitResult;
	GetHitResultAtScreenPosition(MouseScreenPosition, ECC_Visibility, true, HitResult);

	if (HitResult.IsValidBlockingHit())
	{
		InHitResult = HitResult;
		return true;
	}

	// 그 외에 공중에 마우스가 있는 경우
	FVector MouseLocation;
	FVector MouseDirection;
	UGameplayStatics::DeprojectScreenToWorld(this, MouseScreenPosition, MouseLocation, MouseDirection);

	HitResult.Location = MouseLocation + (MouseDirection * 1000.f);
	InHitResult = HitResult;
	
	return true;
}

bool AMapEditingPlayerController::OnGizmoPressedOperation(FHitResult& InHitResult)
{
	FVector MouseLocation;
	FVector MouseDirection;

	DeprojectMousePositionToWorld(MouseLocation, MouseDirection);

	InHitResult.Location = MouseLocation;
	InHitResult.Normal = MouseDirection;

	return true;
}

void AMapEditingPlayerController::OnMousePointUpdate(FVector2D InMousePosition)
{
	MousePosition = InMousePosition;
}

void AMapEditingPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMapEditingPawn* MapEditingPawn = Cast<AMapEditingPawn>(InPawn);
	if (MapEditingPawn)
	{
		MapEditingPawn->GetDragDropOperation()->OnDraggedWidget.RemoveDynamic(this, &AMapEditingPlayerController::OnMousePointUpdate);
		MapEditingPawn->GetDragDropOperation()->OnDraggedWidget.AddDynamic(this, &AMapEditingPlayerController::OnMousePointUpdate);
	}
}
