// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCameraComp.h"

#include "LobbyMainCamera.h"
#include "LobbyStageCamera.h"
#include "LobbySubCamera.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "JumpGame/UI/ClientRoomUI.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
ULobbyCameraComp::ULobbyCameraComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	OnChangeViewTarget.AddDynamic(this, &ULobbyCameraComp::SetViewTarget);
}


// Called when the game starts
void ULobbyCameraComp::BeginPlay()
{
	Super::BeginPlay();
	
	MainCamera = Cast<ALobbyMainCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ALobbyMainCamera::StaticClass()));
	SubCamera = Cast<ALobbySubCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ALobbySubCamera::StaticClass()));
	StageCamera = Cast<ALobbyStageCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ALobbyStageCamera::StaticClass()));

	PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetViewTarget(MainCamera);
	}
}


// Called every frame
void ULobbyCameraComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULobbyCameraComp::SetViewTarget(ECameraState NewState)
{
	if (!PC) return;

	AActor* CurrentTarget = PC->GetViewTarget();
	ACameraActor* TargetCam = nullptr;
	switch (NewState)
	{
	case ECameraState::Main:
		TargetCam = MainCamera;
		break;
	case ECameraState::Sub:
		TargetCam = SubCamera;
		break;
	case ECameraState::Stage:
		TargetCam = StageCamera;
		break;
	}

	if (TargetCam != CurrentTarget)
	{
		PC->SetViewTargetWithBlend(TargetCam, 0.5f);
	}
}
