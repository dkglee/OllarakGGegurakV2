// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMode.h"

#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/PlayerController/InGamePlayerController.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Props/SaveLoad/SaveMapComponent.h"

AInGameMode::AInGameMode()
{
	PlayerControllerClass = AInGamePlayerController::StaticClass();
}

void AInGameMode::BeginPlay()
{
	Super::BeginPlay();

	UJumpGameInstance* JumpGameInstance = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	if (!JumpGameInstance)
	{
		return;
	}
	FString FileName = JumpGameInstance->GetMapFilePath();
	if (FileName.IsEmpty())
	{
		return;
	}
	LoadMapComponent->LoadMapWithString(FileName);
}

void AInGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority() && NewPlayer->GetPawn())
	{
		AFrog* Frog{Cast<AFrog>(NewPlayer->GetPawn())};
		if (Frog)
		{
			if (AvailableSkinIndices.IsEmpty())
			{
				AvailableSkinIndices = {1, 2, 3, 4, 5, 6, 7, 8, 9};

				int32 LastIndex{AvailableSkinIndices.Num() - 1};
				for (int32 i{}; i <= LastIndex; ++i)
				{
					int32 Index{FMath::RandRange(i, LastIndex)};
					if (i != Index)
					{
						AvailableSkinIndices.Swap(i, Index);
					}
				}
			}

			if (!AvailableSkinIndices.IsEmpty())
			{
				const int32 SelectedIndex{AvailableSkinIndices.Pop()};
				Frog->ServerRPC_SetSkin(SelectedIndex);
			}
			else
			{
				Frog->ServerRPC_SetSkin(0);
			}
		}
	}
}
