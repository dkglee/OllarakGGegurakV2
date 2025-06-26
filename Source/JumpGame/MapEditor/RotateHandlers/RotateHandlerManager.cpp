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
	for (APrimitiveProp* SelectedProp : ClickResponse.SelectedProps)
	{
		if (!SelectedProp || !SelectedProp->IsValidLowLevel()) continue;
		
		UGridComponent* GridComponent = SelectedProp->GetGridComp();
		if (!GridComponent) continue;

		GridComponent->Rotate(Axis);

		SelectedProp->RotateAllGizmos();
	}
}

void URotateHandlerManager::HandleAxis(FVector InAxis, FClickResponse& ClickResponse)
{
	APrimitiveProp* PrimitiveProp = FCommonUtil::SafeLast(ClickResponse.SelectedProps);
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

