#include "RotateHandlerManager.h"

#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/MapEditor/Components/RotateGizmoComponent.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"

URotateHandlerManager::URotateHandlerManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

}

void URotateHandlerManager::BeginPlay()
{
	Super::BeginPlay();


}

void URotateHandlerManager::InitializeComponent()
{
	Super::InitializeComponent();

	AMapEditingPawn* Pawn = Cast<AMapEditingPawn>(GetOwner());
	if (!Pawn) return;

	Pawn->GetClickHandlerManager()->OnControlledPropChanged.AddDynamic(this, &URotateHandlerManager::ResetAxis);
}

void URotateHandlerManager::HandleRotate(FClickResponse& ClickResponse)
{
	APrimitiveProp* PrimitiveProp = ClickResponse.TargetProp;
	if (!PrimitiveProp) return;

	UGridComponent* GridComponent = PrimitiveProp->GetGridComp();
	if (!GridComponent) return;

	// FVector PrimitiveProp->GetRotateGizmo()->GetDirection();
	GridComponent->Rotate(Axis);

	// PrimitiveProp->SetNewSizeByRotation(GridComponent->GetSize());
	PrimitiveProp->RotateAllGizmos();
}

void URotateHandlerManager::HandleAxis(FVector InAxis, FClickResponse& ClickResponse)
{
	APrimitiveProp* PrimitiveProp = ClickResponse.TargetProp;
	if (!PrimitiveProp) return;

	Axis = InAxis.GetSafeNormal().GetAbs();

	Cast<URotateGizmoComponent>(PrimitiveProp->GetRotateGizmo())->SetAxisDirection(Axis);
	
	OnAxisChanged.Broadcast(Axis);
}

void URotateHandlerManager::ResetAxis()
{
	Axis = FVector(0, 0, 1);
	OnAxisChanged.Broadcast(Axis);
}

