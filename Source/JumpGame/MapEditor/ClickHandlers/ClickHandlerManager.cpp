#include "ClickHandlerManager.h"

#include "ActorClickHandler.h"
#include "BackgroundClickHandler.h"
#include "ClickContext.h"
#include "GizmoClickHandler.h"
#include "MultiSelectClickHandler.h"
#include "PropSlotClickHandler.h"
#include "RotateGizmoClickHandler.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/DragDropOperation/WidgetMapEditDragDropOperation.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/MapEditor/WarningPropManager/WarningPropManager.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "JumpGame/Utils/FastLogger.h"

UClickHandlerManager::UClickHandlerManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UClickHandlerManager::RegisterHandler(UClickHandlerInterface* Handler)
{
	Handlers.Add(Handler);
	
	Algo::Sort(Handlers, [](const UClickHandlerInterface* A, const UClickHandlerInterface* B)
	{
		return A->GetPriority() > B->GetPriority();   // 우선순위가 높은 것이 앞으로
	});
	Handler->Init(Cast<AMapEditingPawn>(GetOwner()));
}

bool UClickHandlerManager::HandleClick(AMapEditingPlayerController* PlayerController)
{
	AActor* TempActor = FCommonUtil::SafeLast(ControlledClickResponse.SelectedProps);

	FClickContext ClickContext;
	ClickContext.Flags = bRotateGizmoMode ? FClickContext::RotateGizmoMode : 0;
	ClickContext.Flags |= bCtrlMultiSelect ? FClickContext::CtrlMultiSelect : 0;
	ClickContext.Flags |= bShiftMultiSelect ? FClickContext::ShiftMultiSelect : 0;
	
	bool bHandled = false;
	for (const auto& Handler : Handlers)
	{
		if (Handler->HandleClick(ControlledClickResponse, PlayerController, ClickContext))
		{
			bHandled = true;
			break ;
		}
	}
	
	if (TempActor != FCommonUtil::SafeLast(ControlledClickResponse.SelectedProps))
	{
		OnControlledPropChanged.Broadcast();
	}
	
	return bHandled;
}

void UClickHandlerManager::ResetControl()
{
	// 기존의 ControlledClickResponse를 초기화
	if (UGizmoComponent* ControlledGizmo = ControlledClickResponse.TargetGizmo)
	{
		ControlledGizmo->SetUnSelected();
	}
	for (APrimitiveProp* SelectedProp : ControlledClickResponse.SelectedProps)
	{
		if (SelectedProp)
		{
			SelectedProp->SetUnSelected();
		}
	}
	ControlledClickResponse = FClickResponse();
	OnControlledPropChanged.Broadcast();
}

void UClickHandlerManager::BeginPlay()
{
	Super::BeginPlay();

	UClickHandlerInterface* PropSlotClickHandler = NewObject<UPropSlotClickHandler>();
	UClickHandlerInterface* MultiSelectClickHandler = NewObject<UMultiSelectClickHandler>();
	UClickHandlerInterface* RotateGizmoClickHandler = NewObject<URotateGizmoClickHandler>();
	UClickHandlerInterface* GizmoClickHandler = NewObject<UGizmoClickHandler>();
	UClickHandlerInterface* ActorClickHandler = NewObject<UActorClickHandler>();
	UClickHandlerInterface* BackgroundClickHandler = NewObject<UBackgroundClickHandler>();

	RegisterHandler(PropSlotClickHandler);
	RegisterHandler(MultiSelectClickHandler);
	RegisterHandler(RotateGizmoClickHandler);
	RegisterHandler(GizmoClickHandler);
	RegisterHandler(ActorClickHandler);
	RegisterHandler(BackgroundClickHandler);
}

void UClickHandlerManager::InitializeComponent()
{
	Super::InitializeComponent();

	AMapEditingPawn* MapEditingPawn = Cast<AMapEditingPawn>(GetOwner());
	MapEditingPawn->GetDragDropOperation()->OnDragCancelledWidget.AddDynamic(this, &UClickHandlerManager::OnPropDragCancelled);
}


void UClickHandlerManager::OnPropSlotClicked(FName PropID, UClass* InPropClass)
{
	ControlledPropSlotID = PropID;
	ControlledPropClass = InPropClass;
}

void UClickHandlerManager::OnWidgetDragLeave()
{
	bMouseEnterUI = false;
	
	AActor* Actor = FCommonUtil::SafeLast(ControlledClickResponse.SelectedProps);
	if (Actor) Actor->SetActorHiddenInGame(false);
}

void UClickHandlerManager::OnWidgetDragEnter()
{
	bMouseEnterUI = true;

	if (ControlledClickResponse.Result != EClickHandlingResult::UIEditing)
	{
		ControlledClickResponse.Result = EClickHandlingResult::UIEditing;
		
		APrimitiveProp* Prop = GetWorld()->SpawnActor<APrimitiveProp>(ControlledPropClass, FVector::ZeroVector, FRotator::ZeroRotator);
		ControlledClickResponse.ClickedPropByWidget = Prop;
		
		AMapEditingPlayerController* PlayerController = Cast<AMapEditingPlayerController>(GetWorld()->GetFirstPlayerController());
		HandleClick(PlayerController);
	}

	AActor* Actor = FCommonUtil::SafeLast(ControlledClickResponse.SelectedProps);
	if (Actor)
	{
		Actor->SetActorHiddenInGame(true);
		AMapEditorState* GameState = GetWorld()->GetGameState<AMapEditorState>();
		if (GameState)
		{
			GameState->GetWarningPropManager()->RegisterNecessaryProp(Cast<APrimitiveProp>(Actor));
		}
	}
}

void UClickHandlerManager::OnPropDragCancelled()
{
	ControlledPropSlotID = NAME_None;
	ControlledPropClass = nullptr;
	
	if (bMouseEnterUI)
	{
		APrimitiveProp* LastSelected = FCommonUtil::SafeLast(ControlledClickResponse.SelectedProps);
		if (LastSelected)
		{
			AMapEditorState* GameState = GetWorld()->GetGameState<AMapEditorState>();
			if (GameState)
			{
				GameState->GetWarningPropManager()->UnRegister(Cast<APrimitiveProp>(LastSelected));
			}
			LastSelected->Destroy();
			OnPropDragCancelledDelegate.Execute();
		}
		ControlledClickResponse = FClickResponse();
	}

	ControlledClickResponse.Result = EClickHandlingResult::None;
	ControlledClickResponse.ClickedPropByWidget = nullptr;
	OnPropDragCancelledDelegate.Clear();
}
