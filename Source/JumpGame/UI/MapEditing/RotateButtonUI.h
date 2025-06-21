#pragma once

#include "CoreMinimal.h"
#include "AxisEnum.h"
#include "Blueprint/UserWidget.h"
#include "RotateButtonUI.generated.h"

UCLASS()
class JUMPGAME_API URotateButtonUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnRotateButtonClicked();
	UFUNCTION()
	void OnChangeAxisButtonClicked();
	UFUNCTION()
	void OnAxisChanged(FVector Axis);
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "RotateButton", meta = (AllowPrivateAccess = "true"))
	class UButton* RotateButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "RotateButton", meta = (AllowPrivateAccess = "true"))
	class UButton* ChangeAxisButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "RotateButton", meta = (AllowPrivateAccess = "true"))
	class UImage* AxisImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateButton", meta = (AllowPrivateAccess = "true"))
	class UTexture2D* XAxisImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateButton", meta = (AllowPrivateAccess = "true"))
	class UTexture2D* YAxisImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateButton", meta = (AllowPrivateAccess = "true"))
	class UTexture2D* ZAxisImage;

	UPROPERTY()
	int32 CurrentAxisIndex = 0;
	UPROPERTY()
	TArray<class UTexture2D*> AxisImages;
	UPROPERTY()
	TArray<FVector> AxisVectors = { FVector(1, 0, 0), FVector(0, 1, 0), FVector(0, 0, 1) }; 

	UPROPERTY()
	class AMapEditingPawn* MapEditingPawn;
};
