#include "PlayStopUI.h"

#include "EnhancedInputSubsystems.h"
#include "MapEditingHUD.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/UI/Character/JumpGaugeUI.h"

void UPlayStopUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FTransform SpawnTransform;
	SpawnTransform.SetScale3D({1.f, 1.f, 1.f});
	SpawnTransform.SetTranslation({-1000,-1000,-1000});
	PlayPawn = GetWorld()->SpawnActorDeferred<AFrog>(FrogClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (PlayPawn)
	{
		PlayPawn->SetMapEditingPawn(true);
		PlayPawn->SetActorHiddenInGame(true);
		PlayPawn->FinishSpawning(SpawnTransform);
	}

	UJumpGaugeUI* FrogJumpGauge{Cast<UJumpGaugeUI>(PlayPawn->JumpGaugeUIComponent->GetWidget())};
	FrogJumpGauge->DelegateBind(PlayPawn);

	PlayStopButton->OnClicked.AddDynamic(this, &UPlayStopUI::OnPlayStopButtonClicked);

	IMG_PlayStop->SetBrushFromTexture(PlayIcon);
}

void UPlayStopUI::OnPlayStopButtonClicked()
{
	bIsPlayMode = !bIsPlayMode;

	AMapEditorState* MapEditorState = Cast<AMapEditorState>(GetWorld()->GetGameState());
	if (!MapEditorState)
		return;
	UMapEditingHUD* MapEditingHUD = MapEditorState->GetMapEditingHUD();
	if (bIsPlayMode)
	{
		MapEditingHUD->HideCategory();
		IMG_PlayStop->SetBrushFromTexture(StopIcon);
		PlayPawn->SetActorHiddenInGame(false);
	}
	else
	{
		MapEditingHUD->ShowCategory();
		IMG_PlayStop->SetBrushFromTexture(PlayIcon);
		PlayPawn->SetActorHiddenInGame(true);
	}
	ChangePlayer();
}

void UPlayStopUI::ChangePlayer()
{
	if (bIsPlayMode)
	{
		ChangeToFrog();
	}
	else
	{
		ChangeToEditor();
	}
}

void UPlayStopUI::ChangeToFrog()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
		return;

	EditPawn = Cast<AMapEditingPawn>(PC->GetPawn());

	EditPawn->GetClickHandlerManager()->ResetControl();

	FVector CamLocation = PC->PlayerCameraManager->GetCameraLocation();
	FRotator CamRotation = PC->PlayerCameraManager->GetCameraRotation();

	PlayPawn->SetActorLocation(CamLocation);
	PlayPawn->SetActorRotation(CamRotation);
	
	PC->SetViewTargetWithBlend(PlayPawn, 0.2f);
	PC->Possess(PlayPawn);
	
	// Hide and deactivate editor pawn
	EditPawn->SetActorHiddenInGame(true);
	EditPawn->SetActive(false);

	// 🎯 Enhanced Input 갱신
	if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();

			// 👉 PlayPawn용 InputContext 등록
			Subsystem->AddMappingContext(PlayPawnInputContext, 0);
		}
	}
}

void UPlayStopUI::ChangeToEditor()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
		return;

	PlayPawn->SetActorHiddenInGame(true);

	FRotator CamRotation = PC->PlayerCameraManager->GetCameraRotation();

	EditPawn->SetActorLocation(PlayPawn->GetActorLocation());
	EditPawn->SetActorRotation(CamRotation);

	PlayPawn->SetActorLocation(FVector(-1000, -1000, -1000));
	PlayPawn->SetActorRotation(FRotator::ZeroRotator);

	PC->SetViewTargetWithBlend(EditPawn, 0.2f);
	PC->Possess(EditPawn);

	EditPawn->SetActorHiddenInGame(false);
	EditPawn->SetActive(true);

	// 🎯 Enhanced Input 갱신
	if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();

			// 👉 EditPawn용 InputContext 등록
			Subsystem->AddMappingContext(EditPawnInputContext, 0);
		}
	}
}
