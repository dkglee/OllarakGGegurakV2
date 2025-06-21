#include "PropDataComponent.h"

UPropDataComponent::UPropDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPropDataComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UPropDataComponent::SetPropID(const FName& InPropID)
{
	PropID.RowName = InPropID;
}
