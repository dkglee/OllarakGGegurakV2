// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientRoomLogoUI.h"

#include "JumpGame/Core/GameInstance/JumpGameInstance.h"

void UClientRoomLogoUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 게임 인스턴스
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	
	// 로고 애니메이션
	Btn_GoMainStart->OnClicked.AddDynamic(this, &UClientRoomLogoUI::OnClickGoMainStart);
	InitLogoMaterial();
	PlayLogoAnim(true);
}

void UClientRoomLogoUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsRadiusAnimating)
	{
		RadiusAnimating();
	}
}

void UClientRoomLogoUI::OnClickGoMainStart()
{
	PlayLogoAnim(false);
	RadiusStartTime = GetWorld()->GetTimeSeconds();
	bIsRadiusAnimating = true;
}

void UClientRoomLogoUI::RadiusAnimating()
{
	if (bIsRadiusAnimating && DynMaterial)
	{
		float Elapsed = GetWorld()->GetTimeSeconds() - RadiusStartTime;
		float Alpha = FMath::Clamp(Elapsed / RadiusDuration, 0.f, 1.f);
		float NewRadius = FMath::Lerp(3.0f, 0.0f, Alpha);

		DynMaterial->SetScalarParameterValue(FName("Radius"), NewRadius);
		Btn_GoMainStart->SetRenderOpacity(0.f);
		Btn_GoMainStart->SetIsEnabled(false);

		if (Alpha >= 1.f)
		{
			bIsRadiusAnimating = false;
			if (Image_Circle)
			{
				Image_Circle->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UClientRoomLogoUI::InitLogoMaterial()
{
	UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/UI/Image/LogoAnim/MI_RingMask.MI_RingMask"));
	if (BaseMaterial)
	{
		DynMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		if (DynMaterial && Image_Circle)
		{
			DynMaterial->SetScalarParameterValue(FName("Radius"), 3.0f);

			FSlateBrush Brush;
			Brush.SetResourceObject(DynMaterial);
			Image_Circle->SetBrush(Brush);
		}
	}
}

void UClientRoomLogoUI::PlayLogoAnim(bool bIsForward)
{
	if (bIsForward)
	{
		PlayAnimationForward(LogoAnimation);
	}
	else
	{
		PlayAnimationReverse(LogoAnimation);
	}
}
