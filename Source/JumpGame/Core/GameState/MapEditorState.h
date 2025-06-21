// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameState.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "MapEditorState.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AMapEditorState : public ANetworkGameState
{
	GENERATED_BODY()

public:
	AMapEditorState();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void InitWidget(class UClickHandlerManager* ClickHandlerManager, class UWidgetMapEditDragDropOperation* DragDropOperation);

	GETTER(class UCategorySystem*, CategorySystem);
	GETTER(class USaveMapComponent*, SaveMapComponent);
	GETTER(class ULoadMapComponent*, LoadMapComponent);
	GETTER(class UMapEditingHUD*, MapEditingHUD);

private:
	UPROPERTY(Category = EditorState, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> MapEditingHUDClass = nullptr;
	UPROPERTY(Category = EditorState, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMapEditingHUD* MapEditingHUD = nullptr;
	UPROPERTY(Category = EditorState, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCategorySystem* CategorySystem = nullptr;

	UPROPERTY(Category = EditorState, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USaveMapComponent* SaveMapComponent = nullptr;
	UPROPERTY(Category = EditorState, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ULoadMapComponent* LoadMapComponent = nullptr;
};
