#pragma once

// UI, Gizmo, Actor, Background 클릭시 수행
#include "CoreMinimal.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickResponse.h"
#include "GizmoPressedInfo.h"
#include "PressedHandlerInterface.generated.h"

UCLASS()
class JUMPGAME_API UPressedHandlerInterface : public UObject
{
	GENERATED_BODY()
	
public:
	UPressedHandlerInterface() {};
	virtual int32 GetPriority() const { return -1; };
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, FGizmoPressedInfo& GizmoPressedInfo) { return false; };
	virtual ~UPressedHandlerInterface() override {};
	virtual void Init(class AMapEditingPawn* InEditingPawn) { ;
		// 초기화 함수, 필요시 오버라이드
		EditingPawn = InEditingPawn; 
	}

protected:
	UPROPERTY()
	class AMapEditingPawn* EditingPawn = nullptr; // 편집 중인 Pawn
};
