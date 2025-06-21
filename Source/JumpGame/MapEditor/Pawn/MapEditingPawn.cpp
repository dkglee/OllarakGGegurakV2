// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditingPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TestActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/MapEditor/DeleteHandlers/DeleteHandlerManager.h"
#include "JumpGame/MapEditor/DragDropOperation/WidgetMapEditDragDropOperation.h"
#include "JumpGame/MapEditor/PressedHandlers/PressedHandlerManager.h"
#include "JumpGame/MapEditor/RotateHandlers/RotateHandlerManager.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "JumpGame/UI/MapEditing/MapEditingHUD.h"
#include "JumpGame/Utils/CursorManager.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMapEditingPawn::AMapEditingPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_MAPEDITING
	(TEXT("/Game/MapEditor/Input/IMC_MapEditInputContext.IMC_MapEditInputContext"));
	if (IMC_MAPEDITING.Succeeded())
	{
		IMC_MapEditing = IMC_MAPEDITING.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_CLICK
	(TEXT("/Game/MapEditor/Input/Actions/IA_Click.IA_Click"));
	if (IA_CLICK.Succeeded())
	{
		IA_Click = IA_CLICK.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVEABLE
	(TEXT("/Game/MapEditor/Input/Actions/IA_Moveable.IA_Moveable"));
	if (IA_MOVEABLE.Succeeded())
	{
		IA_Moveable = IA_MOVEABLE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVE
	(TEXT("/Game/MapEditor/Input/Actions/IA_Move.IA_Move.IA_Move"));
	if (IA_MOVE.Succeeded())
	{
		IA_Move = IA_MOVE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_TURN
	(TEXT("/Game/MapEditor/Input/Actions/IA_Turn.IA_Turn.IA_Turn"));
	if (IA_TURN.Succeeded())
	{
		IA_Turn = IA_TURN.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_PRESSED
	(TEXT("/Game/MapEditor/Input/Actions/IA_Pressed.IA_Pressed"));
	if (IA_PRESSED.Succeeded())
	{
		IA_Pressed = IA_PRESSED.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_DELETE
	(TEXT("/Game/MapEditor/Input/Actions/IA_Delete.IA_Delete"));
	if (IA_DELETE.Succeeded())
	{
		IA_Delete = IA_DELETE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ROTATE
	(TEXT("/Game/MapEditor/Input/Actions/IA_Rotate.IA_Rotate"));
	if (IA_ROTATE.Succeeded())
	{
		IA_Rotate= IA_ROTATE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_AXIS
	(TEXT("/Game/MapEditor/Input/Actions/IA_Axis.IA_Axis"));
	if (IA_AXIS.Succeeded())
	{
		IA_Axis= IA_AXIS.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_SCROLL
	(TEXT("/Game/MapEditor/Input/Actions/IA_Scroll.IA_Scroll"));
	if (IA_SCROLL.Succeeded())
	{
		IA_Scroll = IA_SCROLL.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_TELEPORT
	(TEXT("/Game/MapEditor/Input/Actions/IA_Teleport.IA_Teleport"));
	if (IA_TELEPORT.Succeeded())
	{
		IA_Teleport = IA_TELEPORT.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVE_GIZMO_MODE
	(TEXT("/Game/MapEditor/Input/Actions/IA_MoveGizmoMode.IA_MoveGizmoMode"));
	if (IA_MOVE_GIZMO_MODE.Succeeded())
	{
		IA_MoveGizmoMode = IA_MOVE_GIZMO_MODE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ROTATE_GIZMO_MODE
	(TEXT("/Game/MapEditor/Input/Actions/IA_RotateGizmoMode.IA_RotateGizmoMode"));
	if (IA_ROTATE_GIZMO_MODE.Succeeded())
	{
		IA_RotateGizmoMode = IA_ROTATE_GIZMO_MODE.Object;
	}

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponentMapEditing"));
	CollisionComponent->InitSphereRadius(35.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("EditorPawn"));
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;

	RootComponent = CollisionComponent;
	
	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("MovementComponentMapEditing"));
	MovementComponent->UpdatedComponent = RootComponent;

	ClickHandlerManager = CreateDefaultSubobject<UClickHandlerManager>(TEXT("ClickHandlerManager"));
	PressedHandlerManager = CreateDefaultSubobject<UPressedHandlerManager>(TEXT("PressedHandlerManager"));
	DeleteHandlerManager = CreateDefaultSubobject<UDeleteHandlerManager>(TEXT("DeleteHandlerManager"));
	RotateHandlerManager = CreateDefaultSubobject<URotateHandlerManager>(TEXT("RotateHandlerManager"));
	DragDropOperation = CreateDefaultSubobject<UWidgetMapEditDragDropOperation>(TEXT("DragDropOperation"));
}

// Called when the game starts or when spawned
void AMapEditingPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (SubSystem)
		{
			SubSystem->AddMappingContext(IMC_MapEditing, 0);
		}
	}
	UCursorManager::SetCursor(this, ECursorName::LeafCursor);
}

// Called every frame
void AMapEditingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMapEditingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		PlayerInput->BindAction(IA_Click, ETriggerEvent::Started, this, &AMapEditingPawn::HandleLeftClick);
		
		PlayerInput->BindAction(IA_Pressed, ETriggerEvent::Started, this, &AMapEditingPawn::HandleLeftPressedStarted);
		PlayerInput->BindAction(IA_Pressed, ETriggerEvent::Triggered, this, &AMapEditingPawn::HandleLeftPressed);
		PlayerInput->BindAction(IA_Pressed, ETriggerEvent::Completed, this, &AMapEditingPawn::HandleLeftPressedCompleted);
		
		PlayerInput->BindAction(IA_Moveable, ETriggerEvent::Started, this, &AMapEditingPawn::HandleRightClickStarted);
		PlayerInput->BindAction(IA_Moveable, ETriggerEvent::Completed, this, &AMapEditingPawn::HandleRightClickStarted);
		
		PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMapEditingPawn::HandleMove);
		PlayerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AMapEditingPawn::HandleMouseMove);

		PlayerInput->BindAction(IA_Delete, ETriggerEvent::Started, this, &AMapEditingPawn::HandleDelete);

		PlayerInput->BindAction(IA_Rotate, ETriggerEvent::Started, this, &AMapEditingPawn::HandleRotate);
		PlayerInput->BindAction(IA_Axis, ETriggerEvent::Triggered, this, &AMapEditingPawn::HandleAxis);

		PlayerInput->BindAction(IA_Scroll, ETriggerEvent::Triggered, this, &AMapEditingPawn::HandleScroll);
		PlayerInput->BindAction(IA_Teleport, ETriggerEvent::Started, this, &AMapEditingPawn::HandleTeleport);

		PlayerInput->BindAction(IA_MoveGizmoMode, ETriggerEvent::Started, this, &AMapEditingPawn::HandleChangeMoveGizmoMode);
		PlayerInput->BindAction(IA_RotateGizmoMode, ETriggerEvent::Started, this, &AMapEditingPawn::HandleChangeRotateGizmoMode);
	}
}

void AMapEditingPawn::HandleLeftClick(const FInputActionValue& InputActionValue)
{
	AMapEditingPlayerController* PC = Cast<AMapEditingPlayerController>(GetController());
	if (!PC) return ;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	PC->SetShowMouseCursor(true);
	PC->SetInputMode(InputMode);
	
	ClickHandlerManager->HandleClick(PC);
}

void AMapEditingPawn::HandleLeftPressedStarted(const FInputActionValue& InputActionValue)
{
	AMapEditingPlayerController* PC = Cast<AMapEditingPlayerController>(GetController());
	if (!PC) return ;
	
	FClickResponse ControlledInfo = ClickHandlerManager->GetControlledClickResponse();
	PressedHandlerManager->InitializeSettings(ControlledInfo, PC);
	UCursorManager::SetCursor(this, ECursorName::LeafPoint);
}

void AMapEditingPawn::HandleLeftPressed(const FInputActionValue& InputActionValue)
{
	AMapEditingPlayerController* PC = Cast<AMapEditingPlayerController>(GetController());
	if (!PC) return ;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	PC->SetShowMouseCursor(true);
	PC->SetInputMode(InputMode);

	if (bRotateGizmoMode)
	{
		return ;
	}
	
	FClickResponse ControlledInfo = ClickHandlerManager->GetControlledClickResponse();
	PressedHandlerManager->HandlePressed(ControlledInfo, PC);
}

void AMapEditingPawn::HandleLeftPressedCompleted(const FInputActionValue& InputActionValue)
{
	PressedHandlerManager->ResetPositions();
	UCursorManager::SetCursor(this, ECursorName::LeafCursor);
}

void AMapEditingPawn::HandleRightClickStarted(const FInputActionValue& InputActionValue)
{
	// 마우스 오른쪽 클릭을 했을 때만 움직일 수 있음
	bCanMove = !bCanMove;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;
	
	if (bCanMove)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(InputMode);
	}
}

void AMapEditingPawn::HandleMove(const FInputActionValue& InputActionValue)
{
	if (!bCanMove) return;

	FVector MoveInput = InputActionValue.Get<FVector>();
	
	MoveForward(MoveInput.Y);
	MoveRight(MoveInput.X);
	MoveUp(MoveInput.Z);
}

void AMapEditingPawn::HandleMouseMove(const FInputActionValue& InputActionValue)
{
	if (!bCanMove) return;

	FVector2d TurnInput = InputActionValue.Get<FVector2d>();
	
	AddControllerYawInput(-TurnInput.X);
	AddControllerPitchInput(TurnInput.Y);
}

void AMapEditingPawn::HandleDelete(const FInputActionValue& InputActionValue)
{
	FClickResponse ControlledInfo = ClickHandlerManager->GetControlledClickResponse();
	ClickHandlerManager->ResetControl();
	DeleteHandlerManager->HandleDelete(ControlledInfo);
}

void AMapEditingPawn::HandleRotate(const FInputActionValue& InputActionValue)
{
	// 정해져 있는 회전값으로 회전
	if (bCanMove || !bRotateGizmoMode) return;
	FClickResponse ControlledInfo = ClickHandlerManager->GetControlledClickResponse();
	RotateHandlerManager->HandleRotate(ControlledInfo);
}

void AMapEditingPawn::HandleAxis(const FInputActionValue& InputActionValue)
{
	// 회전 축 설정
	if (bCanMove || !bRotateGizmoMode) return;
	FVector Axis = InputActionValue.Get<FVector>();
	FClickResponse ControlledInfo = ClickHandlerManager->GetControlledClickResponse();
	RotateHandlerManager->HandleAxis(Axis, ControlledInfo);
}

void AMapEditingPawn::HandleScroll(const FInputActionValue& InputActionValue)
{
	float Scroll = InputActionValue.Get<float>();
	if (FMath::IsNearlyZero(Scroll)) return;

	// 스크롤을 통해서 이동 속도 조절
	if (UFloatingPawnMovement* FloatMove = Cast<UFloatingPawnMovement>(MovementComponent))
	{
		FloatMove->MaxSpeed = FMath::Clamp(
			FloatMove->MaxSpeed + Scroll * ScrollStep,
			MinSpeed, MaxSpeed);

		// 가속/감속값도 같이 늘려서 조작감을 유지
		FloatMove->Acceleration = FloatMove->MaxSpeed * 2.f;
		FloatMove->Deceleration = FloatMove->MaxSpeed * 6.f;
	}
}

void AMapEditingPawn::HandleTeleport(const FInputActionValue& InputActionValue)
{
	FClickResponse HandlingInfo = ClickHandlerManager->GetControlledClickResponse();
	if (!HandlingInfo.TargetProp || !HandlingInfo.TargetProp->IsValidLowLevel())
	{
		return;
	}
	// Target Location을 가져옴
	FVector TargetLocation = HandlingInfo.TargetProp->GetActorLocation() - FVector(0.f, 0.f, 50.f);
	// Target Location에서 현재 카메라가 보고 있는 방향의 뒤로 Size + @ 만큼 이동
	FVector Direction = Controller->GetControlRotation().Vector();
	FVector Size = HandlingInfo.TargetProp->GetGridComp()->GetSize() * HandlingInfo.TargetProp->GetGridComp()->GetSnapSize();
	float MaxOffset = Size.GetMax() + 200.f;
	FVector NewLocation = TargetLocation - Direction * MaxOffset;

	// Pawn을 이동
	/** --- Latent action 설정 --- */
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget   = this;          // 필수
	LatentInfo.UUID             = __LINE__;      // 고유값(아무 숫자나 OK)
	LatentInfo.Linkage          = 0;
	LatentInfo.ExecutionFunction = FName(TEXT("OnMoveFinished"));;    // 이동 완료 시 호출 함수 이름(선택)

	// 0.25초 동안 EaseIn‧EaseOut 보간
	UKismetSystemLibrary::MoveComponentTo(
		GetRootComponent(),          // Pawn 루트(전 Actor 이동)
		NewLocation,                 // 최종 위치
		GetActorRotation(),          // 회전 유지
		true,                        // bEaseOut
		true,                        // bEaseIn
		0.25f,                       // Duration
		false,                       // bForceShortestRotationPath
		EMoveComponentAction::Move,  // 바로 이동
		LatentInfo);
}

void AMapEditingPawn::HandleChangeMoveGizmoMode(const FInputActionValue& InputActionValue)
{
	if (!bRotateGizmoMode || bCanMove) return;
	
	bRotateGizmoMode = false;
	ClickHandlerManager->SetbRotateGizmoMode(bRotateGizmoMode);

	FClickResponse ControlledInfo = ClickHandlerManager->GetControlledClickResponse();
	if (ControlledInfo.TargetProp && ControlledInfo.TargetProp->IsValidLowLevel())
	{
		ControlledInfo.TargetProp->ShowMoveGizmo();
	}
	
	if (!(ControlledInfo.TargetGizmo && ControlledInfo.TargetGizmo->IsValidLowLevel()))
	{
		return ;
	}

	ControlledInfo.TargetGizmo->SetUnSelected();
	ControlledInfo.TargetGizmo = nullptr;
	ClickHandlerManager->SetControlledClickResponse(ControlledInfo);
}

void AMapEditingPawn::HandleChangeRotateGizmoMode(const FInputActionValue& InputActionValue)
{
	if (bRotateGizmoMode || bCanMove) return;
	
	bRotateGizmoMode = true;
	ClickHandlerManager->SetbRotateGizmoMode(bRotateGizmoMode);

	// 현재 클릭되고 있는 Gizmo가 있는지 확인
	FClickResponse ControlledInfo = ClickHandlerManager->GetControlledClickResponse();
	if (ControlledInfo.TargetProp && ControlledInfo.TargetProp->IsValidLowLevel())
	{
		ControlledInfo.TargetProp->ShowRotateGizmo();
	}
	
	if (!(ControlledInfo.TargetGizmo && ControlledInfo.TargetGizmo->IsValidLowLevel()))
	{
		return ;
	}

	ControlledInfo.TargetGizmo->SetUnSelected();
	ControlledInfo.TargetGizmo = nullptr;
	ClickHandlerManager->SetControlledClickResponse(ControlledInfo);
}

void AMapEditingPawn::MoveForward(float Val)
{
	if (FMath::IsNearlyZero(Val)) return;
	if (Controller)
	{
		FRotator const ControlSpaceRot = Controller->GetControlRotation();

		// transform to world space and add it
		AddMovementInput( FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::X ), Val );
	}
}

void AMapEditingPawn::MoveRight(float Val)
{
	if (FMath::IsNearlyZero(Val)) return;
	if (Controller)
	{
		FRotator const ControlSpaceRot = Controller->GetControlRotation();

		// transform to world space and add it
		AddMovementInput( FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::Y ), Val );
	}
}

void AMapEditingPawn::MoveUp(float Val)
{
	if (FMath::IsNearlyZero(Val)) return;
	AddMovementInput(FVector::UpVector, Val);
}

void AMapEditingPawn::SetActive(bool bInActive)
{
	bActive = bInActive;
}

void AMapEditingPawn::OnMoveFinished()
{
}
