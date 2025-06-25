#pragma once

#include "CoreMinimal.h"
#include "ClickHandlerInterface.h"
#include "MultiSelectClickHandler.generated.h"

UCLASS()
class JUMPGAME_API UMultiSelectClickHandler : public UClickHandlerInterface
{
	GENERATED_BODY()

	UMultiSelectClickHandler() {};
	virtual int32 GetPriority() const override { return 250; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, FClickContext& ClickContext) override;
	virtual ~UMultiSelectClickHandler() override {};
};
