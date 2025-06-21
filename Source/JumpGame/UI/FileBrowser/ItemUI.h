// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemType.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "ItemUI.generated.h"

DECLARE_DELEGATE_OneParam(FOnButtonDoubleClicked, const FString& /*Path*/);
DECLARE_DELEGATE_TwoParams(FOnButtonClicked, const FString& /*Path*/, class UItemUI* /*ItemUI*/);

UCLASS()
class JUMPGAME_API UItemUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	GETTER_SETTER(FString, ItemPath)
	GETTER_SETTER(FString, ItemName);
	GETTER_SETTER(class UTextBlock*, ItemText);
	GETTER_SETTER(EItemType, ItemType);
	
	FOnButtonClicked OnClicked;
	FOnButtonDoubleClicked OnDoubleClicked;

	void SetSelected(bool bSelected);;
protected:
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UTextBlock* ItemText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UImage* SelectImage;
	UPROPERTY()
	FString ItemPath;
	UPROPERTY()
	FString ItemName;
	UPROPERTY()
	EItemType ItemType;
};
