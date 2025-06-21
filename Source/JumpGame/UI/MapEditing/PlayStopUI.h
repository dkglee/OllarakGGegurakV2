// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayStopUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UPlayStopUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	void ChangeToFrog();
	void ChangeToEditor();
	void ChangePlayer();
	UFUNCTION()
	void OnPlayStopButtonClicked();

	UPROPERTY()
	class AFrog* PlayPawn;
	UPROPERTY()
	class AMapEditingPawn* EditPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* PlayPawnInputContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* EditPawnInputContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UTexture2D* PlayIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UTexture2D* StopIcon;

	UPROPERTY(meta =(BindWidget), BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UButton* PlayStopButton;
	UPROPERTY(meta =(BindWidget), BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UImage* IMG_PlayStop;

	UPROPERTY()
	bool bIsPlayMode = false;
	
	FTimerHandle WidgetTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AFrog> FrogClass;
};
