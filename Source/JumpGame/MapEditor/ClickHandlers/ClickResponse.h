#pragma once

#include "EClickHandlingResult.h"
#include "ClickResponse.generated.h"

USTRUCT(BlueprintType)
struct FClickResponse
{
	GENERATED_BODY()

	// 어떤 클릭 핸들러가 클릭을 처리했는지
	UPROPERTY(BlueprintReadOnly)
	EClickHandlingResult Result = EClickHandlingResult::None;

	// 어떤 Actor가 클릭되었는지
	UPROPERTY(BlueprintReadOnly)
	class APrimitiveProp* TargetProp = nullptr;

	// 어떤 Gizmo가 클릭되었는지
	UPROPERTY(BlueprintReadOnly)
	class UGizmoComponent* TargetGizmo = nullptr;

	// HitResult
	UPROPERTY(BlueprintReadOnly)
	FHitResult HitResult = FHitResult();

	// 검출된 마우스의 위치
	UPROPERTY(BlueprintReadOnly)
	FVector MouseWorldPosition = FVector::ZeroVector;

	// UI에서 PropSlot을 드래그 했을 때
	UPROPERTY(BlueprintReadOnly)
	class APrimitiveProp* ClickedPropByWidget = nullptr;
	
	// 디버그 메시지
	UPROPERTY(BlueprintReadOnly)
	FString DebugMessage = TEXT("");

	FClickResponse(const EClickHandlingResult InResult = EClickHandlingResult::None, class APrimitiveProp* InTargetProp = nullptr,
		class UGizmoComponent* InTargetGizmo = nullptr, const FHitResult& InHitResult = FHitResult(),
		const FVector& InMouseWorldPosition = FVector::ZeroVector, class APrimitiveProp* InClickedPropByWidget = nullptr,
		const FString& InDebug = TEXT(""))
			: Result(InResult), TargetProp(InTargetProp), TargetGizmo(InTargetGizmo),
			HitResult(InHitResult), MouseWorldPosition(InMouseWorldPosition),
			ClickedPropByWidget(InClickedPropByWidget), DebugMessage(InDebug)
	{}
};
