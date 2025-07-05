// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditorMode.h"

#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"


AMapEditorMode::AMapEditorMode()
{
	DefaultPawnClass = AMapEditingPawn::StaticClass();
	GameStateClass = AMapEditorState::StaticClass();
	PlayerControllerClass = AMapEditingPlayerController::StaticClass();
}

void AMapEditorMode::BeginPlay()
{
	Super::BeginPlay();

	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	GI->CurrentMap = EMapKind::Editor;
	UE_LOG(LogTemp, Warning, TEXT("CurrentMap: %d"), GI->CurrentMap);
}
