#pragma once

#include "EPressedHandlingResult.h"
#include "PressedHandlerInterface.h"

class JUMPGAME_API FGizmoPressedHandler : public IPressedHandler
{
public:
	FGizmoPressedHandler();
	virtual int32 GetPriority() const override { return 50; };
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, const FGizmoPressedInfo& GizmoPressedInfo) override;
	virtual ~FGizmoPressedHandler() override;

private:
	EPressedHandlingResult PressedType = EPressedHandlingResult::GizmoPressed;
};
