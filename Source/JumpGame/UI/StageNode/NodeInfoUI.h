// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NodeInfoUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UNodeInfoUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_FieldName;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Star1;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Star2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Star3;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_ClearTF;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Time;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameStart;

	UFUNCTION()
	void OnClickGameStart();

	UFUNCTION()
	void SetFieldInfo(FName CurrentFieldName, int32 StarCount, float ClearTime);
	UPROPERTY()
	FName SelectFieldLevel;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* StarBlackTex;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* StarYellowTex;
	
	UFUNCTION()
	void UpdateStarImages(int32 StarCount);
	UFUNCTION()
	void UpdateClearTime(float ClearTime);
	UFUNCTION()
	void UpdateClearText(int32 StarCount, float ClearTime);
};

