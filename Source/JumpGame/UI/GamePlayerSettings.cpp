// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerSettings.h"

#include "EngineUtils.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PostProcessComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "Kismet/GameplayStatics.h"

void UGamePlayerSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	// 1. 밝기/대비 적용
	float GainA = FMath::Clamp(Brightness, 0.1f, 1.8f);
	if (const UWorld* World = GetWorld())
	{
		// 플레이어의 component 찾기
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC)
		{
			AFrog* Frog = Cast<AFrog>(PC->GetPawn());
			if (Frog)
			{
				Frog->SettingPostProcessComponent->Settings.ColorGain.Set(1,1,1,GainA);
				Frog->SettingPostProcessComponent->Settings.bOverride_ColorGain = true; // 설정 즉시 반
			}
		}
	}

	// 2. 사운드 볼륨 적용
	USoundMix* MasterSoundMix = LoadObject<USoundMix>(nullptr, TEXT("/Game/Sounds/Class/SCM_TotalSound.SCM_TotalSound"));
	const float VolumeClamp = 1.0f;
	
	// 람다
	TWeakObjectPtr<UGamePlayerSettings> WeakThis{this};
	TWeakObjectPtr<USoundMix> WeakMasterSoundMix{MasterSoundMix};
	auto ApplyVolume = [WeakThis, WeakMasterSoundMix](const FString& ClassName, float Volume)
	{
		if (!WeakMasterSoundMix.IsValid() || !WeakThis.IsValid())
		{
			return;
		}
		UGamePlayerSettings* StrongThis{WeakThis.Get()};
		USoundMix* StrongMasterSoundMix{WeakMasterSoundMix.Get()};
		
		USoundClass* SoundClass = LoadObject<USoundClass>(nullptr, *ClassName);
		if (SoundClass && StrongMasterSoundMix)
		{
			UGameplayStatics::SetSoundMixClassOverride(StrongThis->GetWorld(), StrongMasterSoundMix, SoundClass, Volume, 1.0f, 0.1f);
		}
	};

	float MasterVolumeFloat = (MasterVolume == 1) ? 1.0f : 0.0f;
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_Master.SC_Master"), MasterVolumeFloat);
	
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_Background.SC_Background"), FMath::Clamp(BgmVolume, 0.f, VolumeClamp));
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_Nature.SC_Nature"), FMath::Clamp(WeatherVolume, 0.f, VolumeClamp));
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_UI.SC_UI"), FMath::Clamp(UIVolume, 0.f, VolumeClamp));
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_InGameEffect.SC_InGameEffect"), FMath::Clamp(CharacterVolume, 0.f, VolumeClamp));
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_Obstacle.SC_Obstacle"), FMath::Clamp(ObstacleVolume, 0.f, VolumeClamp));

	// 3. 안티앨리어싱 적용
	GEngine->Exec(GetWorld(), *FString::Printf(TEXT("r.AntiAliasingMethod %d"), AntiValue));

	// 4. 색각 모드 적용
	switch (static_cast<EColorVisionDeficiency>(ColorMode))
	{
	case EColorVisionDeficiency::NormalVision:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::NormalVision, 1.0f, true, false);
		break;
	case EColorVisionDeficiency::Deuteranope:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::Deuteranope, 1.0f, true, false);
		break;
	case EColorVisionDeficiency::Protanope:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::Protanope, 1.0f, true, false);
		break;
	case EColorVisionDeficiency::Tritanope:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::Tritanope, 1.0f, true, false);
		break;
	}
}

