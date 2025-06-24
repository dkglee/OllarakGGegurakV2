// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EPressedHandlingResult.h"
#include "PressedHandlerInterface.h"
#include "CopyPressedHandler.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UCopyPressedHandler : public UPressedHandlerInterface
{
	GENERATED_BODY()

public:
	UCopyPressedHandler() {};
	virtual int32 GetPriority() const override { return 1000; };
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, FGizmoPressedInfo& GizmoPressedInfo) override;
	virtual ~UCopyPressedHandler() override {};

private:
	void DuplicateSelectedProps(FClickResponse& PressedResponse, AMapEditingPlayerController* PlayerController, FGizmoPressedInfo& GizmoPressedInfo, FVector DeltaMove);
	
	EPressedHandlingResult PressedType = EPressedHandlingResult::GizmoPressed;
};
