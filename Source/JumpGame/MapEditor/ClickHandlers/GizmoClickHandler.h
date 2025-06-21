#pragma once

#include "ClickHandlerInterface.h"
#include "GizmoClickHandler.generated.h"

UCLASS()
class JUMPGAME_API UGizmoClickHandler : public UClickHandlerInterface
{
	GENERATED_BODY()
	
public:
	UGizmoClickHandler();
	virtual int32 GetPriority() const override { return 100; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode) override;
	virtual ~UGizmoClickHandler() override;
};
