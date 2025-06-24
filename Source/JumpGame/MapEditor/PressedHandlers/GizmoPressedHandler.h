#pragma once

#include "CoreMinimal.h"
#include "EPressedHandlingResult.h"
#include "PressedHandlerInterface.h"
#include "GizmoPressedHandler.generated.h"

UCLASS()
class JUMPGAME_API UGizmoPressedHandler : public UPressedHandlerInterface
{
	GENERATED_BODY()
public:
	UGizmoPressedHandler();
	virtual int32 GetPriority() const override { return 50; };
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, const FGizmoPressedInfo& GizmoPressedInfo) override;
	virtual ~UGizmoPressedHandler() override;

private:
	EPressedHandlingResult PressedType = EPressedHandlingResult::GizmoPressed;
};
