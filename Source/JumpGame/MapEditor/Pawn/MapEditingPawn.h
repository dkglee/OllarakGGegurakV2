// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "MapEditingPawn.generated.h"

UCLASS()
class JUMPGAME_API AMapEditingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMapEditingPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetActive(bool bInActive);;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	GETTER(class UClickHandlerManager*, ClickHandlerManager)
	GETTER(class UWidgetMapEditDragDropOperation*, DragDropOperation);
	GETTER(class URotateHandlerManager*, RotateHandlerManager);
	GETTER(bool, bRotateGizmoMode);

private:
	UFUNCTION()
	void HandleLeftClick(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleLeftPressedStarted(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleLeftPressed(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleLeftPressedCompleted(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleRightClickStarted(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleMove(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleMouseMove(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleDelete(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleRotate(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleAxis(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleScroll(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleTeleport(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleChangeMoveGizmoMode(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleChangeRotateGizmoMode(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void MoveForward(float Val);
	UFUNCTION()
	void MoveRight(float Val);
	UFUNCTION()
	void MoveUp(float Val);
	UFUNCTION(BlueprintCallable)
	void OnMoveFinished();

	// IMC_MAPEDITING
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_MapEditing = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Click = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Pressed = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Moveable = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Delete = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Rotate = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Axis = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Scroll = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Teleport = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_MoveGizmoMode = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_RotateGizmoMode = nullptr;

	UPROPERTY()
	bool bCanMove = false;

	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPawnMovementComponent* MovementComponent = nullptr;
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent = nullptr;

	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UClickHandlerManager* ClickHandlerManager = nullptr;
	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPressedHandlerManager* PressedHandlerManager = nullptr;
	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDeleteHandlerManager* DeleteHandlerManager = nullptr;
	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URotateHandlerManager* RotateHandlerManager = nullptr;

	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetMapEditDragDropOperation* DragDropOperation = nullptr;

	UPROPERTY()
	bool bActive = true;

	UPROPERTY(EditAnywhere, Category = "Movement|Speed")
	float ScrollStep = 200.f;          // 한 번 스크롤할 때 증감폭

	UPROPERTY(EditAnywhere, Category = "Movement|Speed")
	float MinSpeed  = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Speed")
	float MaxSpeed  = 6000.f;

	// false : 이동 Gizmo 모드
	// true : 회전 Gizmo 모드
	UPROPERTY(EditAnywhere, Category = "Handle|GizmoMode")
	bool bRotateGizmoMode = false;
};
