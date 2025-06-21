// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveResultUI.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USaveResultUI::PlayResultAnim()
{
	UUMGSequencePlayer* SCPlayer = PlayAnimation(SaveResultAnim);
	if (SCPlayer)
	{
		auto& FinishedEvent = SCPlayer->OnSequenceFinishedPlaying();
		TWeakObjectPtr<USaveResultUI> WeakThis = this;
		FinishedEvent.AddLambda([WeakThis](UUMGSequencePlayer& Player) {
			if (!WeakThis.IsValid())
			{
				return;
			}
			USaveResultUI* StrongThis = WeakThis.Get();
			StrongThis->RemoveFromParent();
		});
	}
}

void USaveResultUI::SetResultImage(bool IsSave)
{
	UTexture2D* ResultTexture = IsSave ? SaveSuccess : SaveFail;
	if (ResultTexture)
	{
		SaveResultImg->SetBrushFromTexture(ResultTexture, true);
	}
}

void USaveResultUI::SetResultText(const FString& ResultTest)
{
	SaveResultText->SetText(FText::FromString(ResultTest));
}
