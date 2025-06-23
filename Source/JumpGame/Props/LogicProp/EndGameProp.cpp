// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameProp.h"

#include "Components/BoxComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Core/GameState/MapGameState.h"
#include "JumpGame/Core/PlayerController/InGamePlayerController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEndGameProp::AEndGameProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
	CollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

// Called when the game starts or when spawned
void AEndGameProp::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEndGameProp::OnMyBeginOverlap);
}

// Called every frame
void AEndGameProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEndGameProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AFrog* Frog{Cast<AFrog>(OtherActor)})
	{
		AInGamePlayerController* PC{Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(this, 0))};
		if (PC)
		{
			PC->ShowOutToMainUI();
		}
	}
}

