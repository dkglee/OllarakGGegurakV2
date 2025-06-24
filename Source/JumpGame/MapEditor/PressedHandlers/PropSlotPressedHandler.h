#pragma once

#include "CoreMinimal.h"
#include "EPressedHandlingResult.h"
#include "PressedHandlerInterface.h"
#include "PropSlotPressedHandler.generated.h"

UCLASS()
class JUMPGAME_API UPropSlotPressedHandler : public UPressedHandlerInterface
{
	GENERATED_BODY()
public:
	UPropSlotPressedHandler();
	virtual int32 GetPriority() const override { return 2000000; };
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, FGizmoPressedInfo& GizmoPressedInfo) override;
	virtual ~UPropSlotPressedHandler() override;

private:
	EPressedHandlingResult PressedType = EPressedHandlingResult::WidgetPressed;
};
