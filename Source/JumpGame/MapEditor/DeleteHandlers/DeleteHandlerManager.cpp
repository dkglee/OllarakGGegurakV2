#include "DeleteHandlerManager.h"

#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/MapEditor/CategorySystem/CategorySystem.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
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
			AMapEditorState* GameState = GetWorld()->GetGameState<AMapEditorState>();
			if (!GameState)
			{
				return;
			}
			GameState->GetCategorySystem()->IncrementPropCountByID(SelectedProp->GetPropDataComponent()->GetPropID());
			SelectedProp->Destroy();
		}
	}
}
