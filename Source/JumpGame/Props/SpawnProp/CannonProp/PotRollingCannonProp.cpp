// Fill out your copyright notice in the Description page of Project Settings.


#include "PotRollingCannonProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"


// Sets default values
APotRollingCannonProp::APotRollingCannonProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Pot-02.SM-Pot-02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset
	(TEXT("/Game/_Resource/FX/VFX_Toolkit_V1/ParticleSystems/356Days/Par_ToonKaboom_01.Par_ToonKaboom_01"));
	if (ParticleAsset.Succeeded())
	{
		HitEffectComp->SetTemplate(ParticleAsset.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundAsset
	(TEXT("/Game/Sounds/Ques/Bounce_CueV2.Bounce_CueV2"));
	if (SoundAsset.Succeeded())
	{
		HitSound = Cast<USoundCue>(SoundAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("2004"));
}

// Called when the game starts or when spawned
void APotRollingCannonProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APotRollingCannonProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APotRollingCannonProp::MulticastRPC_PlayEffect_Implementation(FVector Location, int32 Index)
{
	// Super::MulticastRPC_PlayEffect_Implementation(Location, Index);
	switch (Index)
	{
	case 0:
		this->PlayHitEffect(Index);
		break;
	case 1:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Location, 0.5f, 1.5f);
		HitEffectComp->ActivateSystem();
	default:
		break;
	}
}

void APotRollingCannonProp::PlayHitEffect_Implementation(int32 Index)
{
	Super::PlayHitEffect_Implementation(Index);
}
