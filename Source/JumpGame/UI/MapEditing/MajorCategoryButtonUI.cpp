// Fill out your copyright notice in the Description page of Project Settings.


#include "MajorCategoryButtonUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "JumpGame/MapEditor/CategorySystem/CategoryName.h"

void UMajorCategoryButtonUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MajorCategoryButton->OnClicked.AddDynamic(this, &UMajorCategoryButtonUI::OnMajorCategoryButtonClicked);
	NormalButtonStyle = MajorCategoryButton->GetStyle();
}

void UMajorCategoryButtonUI::SetMajorCategory(EMajorCategoryType MajorCategory)
{
	MajorCategoryType = MajorCategory;
	// 4 글자만 보여주기
	// MajorCategoryText->SetText(FText::FromString(FCommonUtil::GetEnumDisplayName(MajorCategory).ToString().Left(4)));
	// MajorCategoryImage->SetBrushFromTexture(FCommonUtil::GetMajorCategoryIcon(MajorCategory));

	FText InCategoryName = CategoryNameTable->FindRow<FCategoryName>(FName(*FCommonUtil::GetEnumDisplayName(MajorCategory).ToString()), TEXT(""), true)->DisplayName;
	MajorCategoryText->SetText(InCategoryName);
}

void UMajorCategoryButtonUI::OnMajorCategoryButtonClicked()
{
	OnMajorButtonClicked.Broadcast(MajorCategoryType, false);
}

void UMajorCategoryButtonUI::ClearSelected()
{
	MajorCategoryButton->SetStyle(NormalButtonStyle);	
}

void UMajorCategoryButtonUI::SetSelected()
{
	FButtonStyle MajorButtonStyle = MajorCategoryButton->GetStyle();
	MajorButtonStyle.Normal = NormalButtonStyle.Pressed;
	MajorButtonStyle.Hovered = NormalButtonStyle.Hovered;
	MajorButtonStyle.Pressed = NormalButtonStyle.Pressed;
	MajorCategoryButton->SetStyle(MajorButtonStyle);
}
