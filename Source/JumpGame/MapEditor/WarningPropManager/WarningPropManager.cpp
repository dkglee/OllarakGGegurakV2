#include "WarningPropManager.h"

#include "EEditorWarningType.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/MapEditor/CategorySystem/CategorySystem.h"
#include "JumpGame/MapEditor/CategorySystem/PropWrap.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "JumpGame/UI/MapEditing/MapEditingHUD.h"
#include "JumpGame/UI/MapEditing/WarningUI.h"

UWarningPropManager::UWarningPropManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWarningPropManager::RegisterNecessaryProp(APrimitiveProp* Prop)
{
	AMapEditorState* EditorState = GetWorld()->GetGameState<AMapEditorState>();
	if (!EditorState)
	{
		return;
	}

	const UPropWrap* PropInfo = EditorState->GetCategorySystem()->GetPropsByID(Prop->GetPropDataComponent()->GetPropID());
	if (!PropInfo)
	{
		return;
	}
	if (PropInfo->Data.PropMaxCount <= 0)
	{
		return; // MaxCount가 0 이하인 Prop은 Warning 대상이 아님.
	}
	if (!NecessaryProps.Contains(PropInfo->Data.PropID))
	{
		NecessaryProps.Add(PropInfo->Data.PropID, FWarningNecessaryPropList());
	}
	FWarningNecessaryPropList& NecessaryPropList = NecessaryProps[PropInfo->Data.PropID];
	if (NecessaryPropList.NecessaryProps.Contains(Prop))
	{
		return; // 이미 등록된 Prop은 무시
	}
	NecessaryPropList.NecessaryProps.Add(Prop);

	EEditorWarningType WarningType = CheckWarnings();
	SetWarning(WarningType);
	ResetIndex();
}

void UWarningPropManager::RegisterCollisionProp(APrimitiveProp* Prop)
{
	if (!Prop)
	{
		return;
	}

	if (OnCollisionProps.Contains(Prop))
	{
		return; // 이미 등록된 Prop은 무시
	}

	OnCollisionProps.Add(Prop);

	EEditorWarningType WarningType = CheckWarnings();
	SetWarning(WarningType);
	ResetIndex();
}

void UWarningPropManager::UnRegisterCollisionProp(APrimitiveProp* Prop)
{
	if (!Prop)
	{
		return;
	}

	if (!OnCollisionProps.Contains(Prop))
	{
		return; // 등록되지 않은 Prop은 무시
	}

	OnCollisionProps.Remove(Prop);

	FFastLogger::LogScreen(FColor::Red, TEXT("UnRegisterCollisionProp: %s"), *Prop->GetName());
	
	EEditorWarningType WarningType = CheckWarnings();
	SetWarning(WarningType);
	ResetIndex();
}

void UWarningPropManager::UnRegisterNecessaryProp(APrimitiveProp* Prop)
{
	if (!Prop)
	{
		return;
	}

	FName PropID = Prop->GetPropDataComponent()->GetPropID();
	if (NecessaryProps.Contains(PropID))
	{
		FWarningNecessaryPropList& NecessaryPropList = NecessaryProps[PropID];
		NecessaryPropList.NecessaryProps.Remove(Prop);
	}

	EEditorWarningType WarningType = CheckWarnings();
	SetWarning(WarningType);
	ResetIndex();
}

void UWarningPropManager::UnRegister(APrimitiveProp* Prop)
{
	UnRegisterCollisionProp(Prop);
	UnRegisterNecessaryProp(Prop);

	EEditorWarningType WarningType = CheckWarnings();
	SetWarning(WarningType);
}

void UWarningPropManager::InitWarningManager(UCategorySystem* CategorySystem, UWarningUI* WarningUI)
{
	CachedCategorySystem = CategorySystem;
	CachedWarningUI = WarningUI;

	TotalNecessaryPropCount = CachedCategorySystem->GetNecessaryPropCount();
	EEditorWarningType WarningType = CheckWarnings();
	SetWarning(WarningType);
	ResetIndex();
}

void UWarningPropManager::BeginPlay()
{
	Super::BeginPlay();

}

void UWarningPropManager::SetWarning(EEditorWarningType WarningType)
{
	switch (WarningType)
	{
	case EEditorWarningType::None:
		if (CachedWarningUI)
		{
			CachedWarningUI->WarningCleared();
		}
		break;
	case EEditorWarningType::Collision:
		if (CachedWarningUI)
		{
			CachedWarningUI->WarningOccured();
			CachedWarningUI->SetWarningText(TEXT("충돌하는 블록이 있어요!"));
		}
		break;
	case EEditorWarningType::OverCount:
		if (CachedWarningUI)
		{
			CachedWarningUI->WarningOccured();
			CachedWarningUI->SetWarningText(TEXT("필수 블록이 너무 많아요!"));
		}
		break;
	case EEditorWarningType::LessCount:
		if (CachedWarningUI)
		{
			CachedWarningUI->WarningOccured();
			CachedWarningUI->SetWarningText(TEXT("필수 블록이 부족해요!"));
		}
		break;
	default:
		break;
	};
}

EEditorWarningType UWarningPropManager::CheckWarnings()
{
	EEditorWarningType WarningType = EEditorWarningType::None;
	for (const FCheckWarnings& CheckFunc : CheckWarningFunctions)
	{
		if (!(this->*CheckFunc)(WarningType))
		{
			break ;
		}
	}
	return WarningType;
}

bool UWarningPropManager::CheckCollisionProps(EEditorWarningType& OutWarningType)
{
	// 충돌하는 객체가 없으면 경고 없음
	bool Flag = OnCollisionProps.Num() == 0;
	OutWarningType = Flag ? EEditorWarningType::None : EEditorWarningType::Collision;
	return Flag;
}

bool UWarningPropManager::CheckNecessaryProps(EEditorWarningType& OutWarningType)
{
	// 필수 Prop이 Max보다 많은 경우
	for (const auto& Pair : NecessaryProps)
	{
		const FWarningNecessaryPropList& NecessaryPropList = Pair.Value;
		const UPropWrap* PropInfo = CachedCategorySystem->GetPropsByID(Pair.Key);
		if (!PropInfo || PropInfo->Data.PropMaxCount <= 0)
		{
			continue; // MaxCount가 0 이하인 Prop은 Warning 대상이 아님.
		}

		if (NecessaryPropList.NecessaryProps.Num() > PropInfo->Data.PropMaxCount)
		{
			OutWarningType = EEditorWarningType::OverCount;
			return false; // 과다 등록된 Prop이 있으면 Warning 발생
		}

		if (NecessaryPropList.NecessaryProps.Num() < PropInfo->Data.PropMaxCount)
		{
			OutWarningType = EEditorWarningType::LessCount;
		}
	}

	// 필수 Prop의 ID 개수가 Warning
	if (NecessaryProps.Num() < TotalNecessaryPropCount)
	{
		OutWarningType = EEditorWarningType::LessCount; // 필수 Prop이 부족한 경우 Warning 발생
	}
	
	return OutWarningType == EEditorWarningType::None; // Warning이 없으면 true, 있으면 false
}

APrimitiveProp* UWarningPropManager::GetTargetProp()
{
	// 우선 충돌중인 Prop을 반환
	APrimitiveProp* TargetProp = nullptr;
	if (OnCollisionProps.Num() > 0 && OnCollisionProps.IsValidIndex(CurrentIndex))
	{
		TargetProp = OnCollisionProps[CurrentIndex++].Get();

		CurrentIndex %= OnCollisionProps.Num(); // 인덱스가 범위를 벗어나지 않도록 순환
		if (TargetProp && TargetProp->IsValidLowLevel())
		{
			return TargetProp; // 충돌중인 Prop이 유효하면 반환
		}
	}

	// 충돌중인 Prop이 없으면 필수 Prop 중 초과한 Prop을 반환
	for (const auto& Pair : NecessaryProps)
	{
		const FWarningNecessaryPropList& NecessaryPropList = Pair.Value;
		const UPropWrap* PropInfo = CachedCategorySystem->GetPropsByID(Pair.Key);
		if (!PropInfo || PropInfo->Data.PropMaxCount <= 0)
		{
			continue; // MaxCount가 0 이하인 Prop은 Warning 대상이 아님.
		}
		
		if (NecessaryPropList.NecessaryProps.Num() > PropInfo->Data.PropMaxCount)
		{
			// 초과한 Prop이 있으면 그 중 하나를 반환
			TargetProp = NecessaryPropList.NecessaryProps[NecessaryIndexMap[Pair.Key]++]; // 첫 번째 Prop을 반환
			NecessaryIndexMap[Pair.Key] %= NecessaryPropList.NecessaryProps.Num(); // 인덱스가 범위를 벗어나지 않도록 순환
			return TargetProp; // 유효한 Prop을 반환
		}
	}
	
	return TargetProp;
}

void UWarningPropManager::ResetIndex()
{
	CurrentIndex = 0; // 인덱스를 초기화
	NecessaryIndexMap.Empty(); // 필요시 필요한 Prop 인덱스 맵도 초기화
	for (const auto& Pair : NecessaryProps)
	{
		NecessaryIndexMap.Add(Pair.Key, 0); // 각 Prop ID에 대해 인덱스를 초기화
	}
}
