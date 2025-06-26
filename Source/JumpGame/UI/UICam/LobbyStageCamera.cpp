// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyStageCamera.h"

#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


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

	// Dead Zone 박스
	FVector BoxCenter = CamLocation;
	FVector BoxExtent = FVector(DeadZoneHalfWidth, DeadZoneHalfHeight, 100.f);

	// 박스 안에 캐릭터가 있으면 정지
	if (UKismetMathLibrary::IsPointInBox(CharLocation, BoxCenter, BoxExtent)) return;

	// 벗어나면 카메라를 캐릭터 쪽으로 이동
	FVector NewLocation = FMath::VInterpTo(CamLocation, CharLocation, DeltaTime, FollowSpeed);
	SetActorLocation(NewLocation);
}

