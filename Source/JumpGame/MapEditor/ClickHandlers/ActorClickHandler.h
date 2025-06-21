#pragma once

#include "ClickHandlerInterface.h"
#include "ClickHandlerInterface.h"
#include "ActorClickHandler.generated.h"

UCLASS()
class JUMPGAME_API UActorClickHandler : public UClickHandlerInterface
{
	GENERATED_BODY()
	
public:
	UActorClickHandler();
	virtual int32 GetPriority() const override { return 50; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode) override;
	virtual ~UActorClickHandler() override;
};
