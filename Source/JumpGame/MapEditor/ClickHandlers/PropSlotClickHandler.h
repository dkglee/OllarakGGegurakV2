#pragma once

#include "CoreMinimal.h"
#include "ClickHandlerInterface.h"
#include "PropSlotClickHandler.generated.h"

UCLASS()
class JUMPGAME_API UPropSlotClickHandler : public UClickHandlerInterface
{
	GENERATED_BODY()
	
public:
	UPropSlotClickHandler();
	virtual int32 GetPriority() const override { return 300; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode) override;
	virtual ~UPropSlotClickHandler() override;
};
