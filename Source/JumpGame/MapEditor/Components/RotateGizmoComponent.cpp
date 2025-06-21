// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateGizmoComponent.h"

#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/MapEditor/RotateHandlers/RotateHandlerManager.h"
#include "Kismet/GameplayStatics.h"

URotateGizmoComponent::URotateGizmoComponent()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ROTATE_GIZMO
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/SM_RotateGizmo.SM_RotateGizmo"));
	if (SM_ROTATE_GIZMO.Succeeded())
	{
		GizmoMesh = SM_ROTATE_GIZMO.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO_ROTATE
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_RotateGizmo.MI_RotateGizmo"));
	if (MI_GIZMO_ROTATE.Succeeded())
	{
		GizmoMaterial = MI_GIZMO_ROTATE.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO_SELECTED_ROTATE
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_SelectedRotateGizmo.MI_SelectedRotateGizmo"));
	if (MI_GIZMO_SELECTED_ROTATE.Succeeded())
	{
		GizmoSelectedMaterial = MI_GIZMO_SELECTED_ROTATE.Object;
	}

	Super::SetStaticMesh(GizmoMesh);
	Super::SetMaterial(0, GizmoMaterial);
	SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	BaseScale = GetRelativeScale3D();
}

FVector URotateGizmoComponent::GetDirection() const
{
	return AxisDirection.GetSafeNormal();
}

void URotateGizmoComponent::SetSelected()
{
	Super::SetSelected();

	if (EditingPawn)
	{
		FVector Axis = EditingPawn->GetRotateHandlerManager()->GetAxis();
		SetAxisDirection(Axis);
	}
}

void URotateGizmoComponent::SetUnSelected()
{
	Super::SetUnSelected();

	// 축을 0으로 초기화 : Yaw 축 회전으로 초기화

	if (EditingPawn)
	{
		FVector Axis = EditingPawn->GetRotateHandlerManager()->GetAxis();
		SetAxisDirection(Axis);
	}
	
	// Pitch : 90 Yaw : 0 Roll : 0 => Roll 축 회전으로 초기화
	// Pitch : 0 Yaw : 0 Roll : 90 => Pitch 축 회전으로 초기화
	// Pitch : 0 Yaw : 0 Roll : 0 => Yaw 축 회전으로 초기화
	// SetRelativeRotation(FRotator(0, 0, 90)); => Pitch 축 회전으로 초기화
}

void URotateGizmoComponent::SetAxisDirection(const FVector& NewAxisDirection)
{
	AxisDirection = NewAxisDirection;
	if (AxisDirection.X)
	{
		SetRelativeRotation(FRotator(-90, 0, 0));
	}
	else if (AxisDirection.Y)
	{
		SetRelativeRotation(FRotator(0, 0, -90));
	}
	else if (AxisDirection.Z)
	{
		SetRelativeRotation(FRotator(0, 0, 0));
	}
}

void URotateGizmoComponent::Clicked()
{
	GetWorld()->GetTimerManager().ClearTimer(TempTimerHandle);
	
	TWeakObjectPtr<URotateGizmoComponent> WeakThis = this;
	// 2) 상수 정의
	constexpr float MinScale      = 0.8f;
	constexpr float MaxScale      = 0.83f;
	constexpr float TotalTime     = 0.25f;        // 전체 길이
	constexpr float HalfTime      = TotalTime * 0.5f;

	// 3) 시간 누적용 지역 변수(λ 캡처)
	float Elapsed = 0.f;

	FFastLogger::LogScreen(FColor::Green, TEXT("Clicked Rotate Gizmo"));

	// 4) 매 프레임 람다 실행
	GetWorld()->GetTimerManager().SetTimer(
		TempTimerHandle,
		FTimerDelegate::CreateLambda([WeakThis, Elapsed]() mutable
		{
			if (!WeakThis.IsValid())
				return;

			URotateGizmoComponent* Gizmo = WeakThis.Get();
			const float Delta = Gizmo->GetWorld()->GetDeltaSeconds();
			Elapsed += Delta;

			/* ----- 구간별 보간 ----- */
			float Scale;
			if (Elapsed <= HalfTime)
			{
				// 0.6 → 1.0 (처음 0.25 초)
				const float Alpha = Elapsed / HalfTime;          // 0~1
				Scale = FMath::Lerp(MinScale, MaxScale, Alpha);
			}
			else
			{
				// 1.0 → 0.6 (마지막 0.25 초)
				const float Alpha = (Elapsed - HalfTime) / HalfTime; // 0~1
				Scale = FMath::Lerp(MaxScale, MinScale, Alpha);
			}

			FFastLogger::LogScreen(FColor::Green, TEXT("Elapsed: %.2f, Scale: %.2f"), Elapsed, Scale);
			Gizmo->SetRelativeScale3D(FVector(Scale));

			/* ----- 종료 처리 ----- */
			if (Elapsed >= TotalTime)
			{
				Gizmo->SetUnSelected();
				Gizmo->GetWorld()->GetTimerManager().ClearTimer(Gizmo->TempTimerHandle);
			}
		}),
		0.01f,   /* Rate : 0 = 한 프레임마다 */
		true    /* bLoop */
	);
}

void URotateGizmoComponent::BeginPlay()
{
	Super::BeginPlay();

	EditingPawn = Cast<AMapEditingPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AMapEditingPawn::StaticClass()));
}
