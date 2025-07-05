// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeResultUI.h"
#include "Components/Image.h"
#include "JumpGame/Core/SaveGame/JumpSaveGame.h"
#include "JumpGame/StageSystem/FieldTableRow.h"
#include "JumpGame/StageSystem/StageSystemSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UNodeResultUI::UpdateNodeStarImages(int32 StarCount)
{
	StarCount = FMath::Clamp(StarCount, 0, 3);
	TArray<UImage*> StarImages = { Image_Star1, Image_Star2, Image_Star3 };
	
	for (int32 i = 0; i < 3; ++i)
	{
		if (StarImages[i])
		{
			if (i < StarCount)
			{
				// 노란 별 텍스처 세팅
				StarImages[i]->SetBrushFromTexture(StarYellowTex); 
			}
			else
			{
				// 검은 별 텍스처 세팅
				StarImages[i]->SetBrushFromTexture(StarBlackTex);
			}
		}
	}
}
