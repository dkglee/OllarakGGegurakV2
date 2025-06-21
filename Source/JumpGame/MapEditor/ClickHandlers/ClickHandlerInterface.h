#pragma once

// UI, Gizmo, Actor, Background 클릭시 수행
#include "ClickResponse.h"
#include "CoreMinimal.h"
#include "ClickHandlerInterface.generated.h"

UCLASS()
class JUMPGAME_API UClickHandlerInterface : public UObject
{
	GENERATED_BODY()
	
public:
	UClickHandlerInterface() {};
	virtual int32 GetPriority() const { return -1; };
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode) { return false; };
	virtual ~UClickHandlerInterface() override {};
	virtual void Init(class AMapEditingPawn* InEditingPawn) { ;
		// 초기화 함수, 필요시 오버라이드
		EditingPawn = InEditingPawn; 
	}
	
protected:
	UPROPERTY()
	class AMapEditingPawn* EditingPawn = nullptr; // 편집 중인 Pawn
};
