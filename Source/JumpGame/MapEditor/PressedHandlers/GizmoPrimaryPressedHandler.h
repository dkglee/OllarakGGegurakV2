#pragma once

#include "CoreMinimal.h"
#include "EPressedHandlingResult.h"
#include "PressedHandlerInterface.h"
#include "GizmoPrimaryPressedHandler.generated.h"

UCLASS()
class JUMPGAME_API UGizmoPrimaryPressedHandler : public UPressedHandlerInterface
{
	GENERATED_BODY()
	
public:
	UGizmoPrimaryPressedHandler();
	virtual int32 GetPriority() const override { return 100; };
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, FGizmoPressedInfo& GizmoPressedInfo) override;
	virtual ~UGizmoPrimaryPressedHandler() override;

private:
	EPressedHandlingResult PressedType = EPressedHandlingResult::GizmoPrimaryPressed;
};
