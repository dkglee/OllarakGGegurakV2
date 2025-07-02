// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditorState.h"

#include "Blueprint/UserWidget.h"
#include "JumpGame/AIServices/Shared/HttpManagerComponent.h"
#include "JumpGame/MapEditor/CategorySystem/CategorySystem.h"
#include "JumpGame/MapEditor/WarningPropManager/WarningPropManager.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Props/SaveLoad/SaveMapComponent.h"
#include "JumpGame/UI/MapEditing/MapEditingHUD.h"
#include "JumpGame/Utils/FastLogger.h"

AMapEditorState::AMapEditorState()
{
	PrimaryActorTick.bCanEverTick = true;
	CategorySystem = CreateDefaultSubobject<UCategorySystem>(TEXT("CategorySystem"));
	
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_MAPEDITING_HUD
	(TEXT("/Game/UI/MapEditing/WBP_MapEditingHUD.WBP_MapEditingHUD_C"));
	if (WBP_MAPEDITING_HUD.Succeeded())
	{
		MapEditingHUDClass = WBP_MAPEDITING_HUD.Class;
	}

	SaveMapComponent = CreateDefaultSubobject<USaveMapComponent>(TEXT("SaveMapComponent"));
	LoadMapComponent = CreateDefaultSubobject<ULoadMapComponent>(TEXT("LoadMapComponent"));
	HttpManagerComponent = CreateDefaultSubobject<UHttpManagerComponent>(TEXT("HttpManagerComponent"));
	WarningPropManager = CreateDefaultSubobject<UWarningPropManager>(TEXT("WarningPropManager"));
}

void AMapEditorState::BeginPlay()
{
	Super::BeginPlay();
	
	MapEditingHUD = CreateWidget<UMapEditingHUD>(GetWorld(), MapEditingHUDClass);
	if (MapEditingHUD)
	{
		MapEditingHUD->AddToViewport();

		AMapEditingPawn* MapEditingPawn = Cast<AMapEditingPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (MapEditingPawn)
		{
			InitWidget(MapEditingPawn->GetClickHandlerManager(), MapEditingPawn->GetDragDropOperation());
		}
	}
	WarningPropManager->InitWarningManager(CategorySystem, MapEditingHUD->GetEditWarningUI());
}

void AMapEditorState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMapEditorState::InitWidget(class UClickHandlerManager* ClickHandlerManager,
                                 class UWidgetMapEditDragDropOperation* DragDropOperation)
{
	if (MapEditingHUD)
	{
		MapEditingHUD->InitWidget(ClickHandlerManager, DragDropOperation);
	}
}

