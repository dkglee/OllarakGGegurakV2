#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarningUI.generated.h"

UCLASS()
class JUMPGAME_API UWarningUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void PlayWarningAnim();
	UFUNCTION()
	void SetWarningText(const FString& Text);
	UFUNCTION()
	void SetSubText(const FString& Text);
	UFUNCTION()
	void StopWarningAnim();

// private:
	UFUNCTION()
	void WarningOccured();
	UFUNCTION()
	void WarningCleared();
	UFUNCTION()
	void OnWarningButtonReleased();
	UFUNCTION()
	void OnWarningButtonHovered();
	UFUNCTION()
	void OnWarningButtonUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetAnimation* WarningForwardAnim;
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetAnimation* WarningReverseAnim;
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetAnimation* WarningButtonActivateAnim;
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetAnimation* WarningOccuredAnim;
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetAnimation* WarningClearedAnim;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UImage* WarningImage;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UTextBlock* WarningText;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UButton* WarningToggle;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	class USizeBox* WarningBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UTextBlock* SubText;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "UI")
	bool bIsButtonClicked = false;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "UI")
	bool bIsWarningOccured = false; // 경고가 발생했는지 여부
};
