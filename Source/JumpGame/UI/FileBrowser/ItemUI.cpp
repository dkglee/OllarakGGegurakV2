// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUI.h"

#include "Components/Image.h"

void UItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SelectImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemUI::SetSelected(bool bSelected)
{
	if (bSelected)
	{
		SelectImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SelectImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FReply UItemUI::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	if (OnDoubleClicked.IsBound())
	{
		OnDoubleClicked.Execute(ItemPath);
	}
	return FReply::Handled();
}

FReply UItemUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (OnClicked.IsBound())
	{
		OnClicked.Execute(ItemPath, this);
	}
	return FReply::Handled();
}
