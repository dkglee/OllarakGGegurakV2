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
	virtual int32 GetPriority() const override { return 150; };
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, const FGizmoPressedInfo& GizmoPressedInfo) override;
	virtual ~UPropSlotPressedHandler() override;

private:
	EPressedHandlingResult PressedType = EPressedHandlingResult::WidgetPressed;
};
