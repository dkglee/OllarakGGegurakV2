#include "DeleteHandlerManager.h"

#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"

UDeleteHandlerManager::UDeleteHandlerManager()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDeleteHandlerManager::BeginPlay()
{
	Super::BeginPlay();

}

void UDeleteHandlerManager::HandleDelete(FClickResponse& ClickResponse)
{
	if (ClickResponse.TargetProp)
	{
		ClickResponse.TargetProp->Destroy();
	}
}
