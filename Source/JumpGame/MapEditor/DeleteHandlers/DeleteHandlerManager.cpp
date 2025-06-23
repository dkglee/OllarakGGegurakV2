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
	for (APrimitiveProp* SelectedProp : ClickResponse.SelectedProps)
	{
		if (SelectedProp)
		{
			SelectedProp->Destroy();
		}
	}
}
