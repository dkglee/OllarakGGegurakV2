// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyStageCamera.h"

#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALobbyStageCamera::ALobbyStageCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALobbyStageCamera::BeginPlay()
{
	Super::BeginPlay();
	TargetFrog = Cast<ALobbyFrog>(UGameplayStatics::GetActorOfClass(GetWorld(),ALobbyFrog::StaticClass()));
}

// Called every frame
void ALobbyStageCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetFrog) return;
	if (TargetFrog->CurrentState != ELobbyCharacterState::OnStageMap) return;

	FVector CamLocation = GetActorLocation();
	FVector CharLocation = TargetFrog->GetActorLocation();
	FVector Offset = CharLocation - CamLocation;

	FVector TargetCamLoc = CamLocation;

	// X 방향
	if (FMath::Abs(Offset.X) > ScreenThreshold.X)
	{
		TargetCamLoc.X = CharLocation.X;
	}

	// Y 방향
	if (FMath::Abs(Offset.Y) > ScreenThreshold.Y)
	{
		TargetCamLoc.Y = CharLocation.Y;
	}

	// 부드럽게 이동
	FVector NewCamLoc = FMath::VInterpTo(CamLocation, TargetCamLoc, DeltaTime, FollowSpeed);
	SetActorLocation(NewCamLoc);

	// 디버깅
	FVector BoxCenter = CamLocation;
	FVector BoxExtent = FVector(ScreenThreshold.X, ScreenThreshold.Y, 10.f);

	DrawDebugBox(
		GetWorld(),
		BoxCenter,
		BoxExtent,
		FColor::Green,
		false,
		-1.f,
		0,
		2.f
	);
}

