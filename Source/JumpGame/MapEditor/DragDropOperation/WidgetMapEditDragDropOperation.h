#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetMapEditDragDropOperation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDragCancelledWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDraggedWidget, FVector2D, MousePosition);

UCLASS()
class JUMPGAME_API UWidgetMapEditDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UWidgetMapEditDragDropOperation();

	virtual void BeginDestroy() override;

	UFUNCTION()
	virtual void Dragged_Implementation(const FPointerEvent& PointerEvent) override;
	UFUNCTION()
	void OnDragging(UDragDropOperation* Operation);
	UFUNCTION()
	void OnDraggingCancelled(UDragDropOperation* Operation);

	UFUNCTION()
	void OnDragEnter();
	UFUNCTION()
	void OnDragLeave();

	static bool ScreenToViewportLocal(const FPointerEvent& PointerEvent, UWorld* World, FVector2D& OutLocalPx, FVector2D& OutLocalUMG);

	UPROPERTY(BlueprintAssignable)
	FOnDragCancelledWidget OnDragCancelledWidget;
	UPROPERTY(BlueprintAssignable)
	FOnDraggedWidget OnDraggedWidget;
};
