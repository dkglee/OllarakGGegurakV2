// Fill out your copyright notice in the Description page of Project Settings.


#include "LotusLeafTrampolineProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"


// Sets default values
ALotusLeafTrampolineProp::ALotusLeafTrampolineProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/Trampoline/Green_Leaf_Curve_0509064638_texture.Green_Leaf_Curve_0509064638_texture"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundAsset
	(TEXT("/Game/Sounds/Ques/Bounce_Cue.Bounce_Cue"));
	if (SoundAsset.Succeeded())
	{
		HitSound = Cast<USoundCue>(SoundAsset.Object);
	}

	SetSize(FVector(2, 2, 2));

	PropDataComponent->SetPropID(TEXT("2002"));
}

// Called when the game starts or when spawned
void ALotusLeafTrampolineProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALotusLeafTrampolineProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

