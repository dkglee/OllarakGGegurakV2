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
	void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_FieldName;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Star1;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Star2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Star3;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Min;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Sec;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameStart;

	UFUNCTION()
	void OnClickGameStart();

	UPROPERTY()
	FName StartFieldName;

	UFUNCTION()
	void SetFieldInfo(FName CurrentFieldName, int32 StarCount, float ClearTime);
};
