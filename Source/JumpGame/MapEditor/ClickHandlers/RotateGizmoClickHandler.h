#pragma once

#include "ClickHandlerInterface.h"
#include "RotateGizmoClickHandler.generated.h"

UCLASS()
class JUMPGAME_API URotateGizmoClickHandler : public UClickHandlerInterface
{
	GENERATED_BODY()
	
public:
	URotateGizmoClickHandler();
	virtual int32 GetPriority() const override { return 200; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode) override;
	virtual ~URotateGizmoClickHandler() override;
};
