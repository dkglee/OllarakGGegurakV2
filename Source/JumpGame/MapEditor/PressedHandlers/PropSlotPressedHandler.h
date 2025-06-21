#pragma once

#include "EPressedHandlingResult.h"
#include "PressedHandlerInterface.h"

class JUMPGAME_API FPropSlotPressedHandler : public IPressedHandler
{
public:
	FPropSlotPressedHandler();
	virtual int32 GetPriority() const override { return 150; };
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, const FGizmoPressedInfo& GizmoPressedInfo) override;
	virtual ~FPropSlotPressedHandler() override;

private:
	EPressedHandlingResult PressedType = EPressedHandlingResult::WidgetPressed;
};
