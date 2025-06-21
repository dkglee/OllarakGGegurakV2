#pragma once

#include "ClickHandlerInterface.h"
#include "BackgroundClickHandler.generated.h"

UCLASS()
class JUMPGAME_API UBackgroundClickHandler : public UClickHandlerInterface
{
	GENERATED_BODY()
	
public:
	UBackgroundClickHandler();
	virtual int32 GetPriority() const override { return 0; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode) override;
	virtual ~UBackgroundClickHandler() override;
};
