// Fill out your copyright notice in the Description page of Project Settings.


#include "StageOutSign.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


// Sets default values
AStageOutSign::AStageOutSign()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SignMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SignMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSign (TEXT("/Game/Fab/SM_MERGED_StaticMeshActor_293.SM_MERGED_StaticMeshActor_293"));
	if (TempSign.Succeeded())
	{
		SignMesh->SetStaticMesh(TempSign.Object);
	}
	SetRootComponent(SignMesh);
	SignMesh->SetRelativeScale3D(FVector(0.14f));
	
	
	ConstructorHelpers::FObjectFinder<UParticleSystem> TempParticle (TEXT("/Game/_Resource/FX/VFX_Toolkit_V1/ParticleSystems/356Days/Par_FightCloud_01.Par_FightCloud_01"));
	if (TempParticle.Succeeded())
	{
		SignEffect = Cast<UParticleSystem>(TempParticle.Object);
	}
}

// Called when the game starts or when spawned
void AStageOutSign::BeginPlay()
{
	Super::BeginPlay();
	
	SignMesh->SetVisibility(false);
}

void AStageOutSign::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

// Called every frame
void AStageOutSign::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStageOutSign::SpawnSign(bool bIsZero)
{
	FVector SpawnLocation = SignMesh->GetComponentLocation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SignEffect, SpawnLocation, FRotator::ZeroRotator, FVector(0.5f), true);

	// 이펙트 시간
	const float EffectDuration = 0.7f;
	
	TWeakObjectPtr<AStageOutSign> WeakThis(this);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,
		FTimerDelegate::CreateLambda([WeakThis, bIsZero]() {
			if (!WeakThis.IsValid()) return;

			AStageOutSign* StrongThis = WeakThis.Get();
			if (StrongThis->SignMesh)
			{
				StrongThis->SignMesh->SetVisibility(!bIsZero);
			}
		}), EffectDuration, false);
}
