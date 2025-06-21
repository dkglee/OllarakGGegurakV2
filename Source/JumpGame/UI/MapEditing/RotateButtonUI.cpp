// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateButtonUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/MapEditor/RotateHandlers/RotateHandlerManager.h"

void URotateButtonUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AxisImages.Add(XAxisImage);
	AxisImages.Add(YAxisImage);
	AxisImages.Add(ZAxisImage);

	RotateButton->OnClicked.AddDynamic(this, &URotateButtonUI::OnRotateButtonClicked);
	ChangeAxisButton->OnClicked.AddDynamic(this, &URotateButtonUI::OnChangeAxisButtonClicked);

	MapEditingPawn = Cast<AMapEditingPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	MapEditingPawn->GetRotateHandlerManager()->OnAxisChanged.AddDynamic(this, &URotateButtonUI::OnAxisChanged);
}

void URotateButtonUI::OnRotateButtonClicked()
{
	FClickResponse ControlledInfo = MapEditingPawn->GetClickHandlerManager()->GetControlledClickResponse();
	if (MapEditingPawn->GetbRotateGizmoMode())
	{
		MapEditingPawn->GetRotateHandlerManager()->HandleRotate(ControlledInfo);
	}
}

void URotateButtonUI::OnChangeAxisButtonClicked()
{
	CurrentAxisIndex = (CurrentAxisIndex + 1) % AxisVectors.Num();
	FVector Axis = AxisVectors[CurrentAxisIndex];
	
	FClickResponse ControlledInfo = MapEditingPawn->GetClickHandlerManager()->GetControlledClickResponse();
	if (MapEditingPawn->GetbRotateGizmoMode())
	{
		MapEditingPawn->GetRotateHandlerManager()->HandleAxis(Axis, ControlledInfo);
		AxisImage->SetBrushFromTexture(AxisImages[CurrentAxisIndex]);
	}
}

void URotateButtonUI::OnAxisChanged(FVector Axis)
{
	CurrentAxisIndex = AxisVectors.IndexOfByKey(Axis);
	if (CurrentAxisIndex == INDEX_NONE)
	{
		CurrentAxisIndex = 0;
	}
	AxisImage->SetBrushFromTexture(AxisImages[CurrentAxisIndex]);
}
