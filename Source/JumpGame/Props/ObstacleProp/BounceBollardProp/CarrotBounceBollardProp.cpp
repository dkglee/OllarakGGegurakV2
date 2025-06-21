// Fill out your copyright notice in the Description page of Project Settings.


#include "CarrotBounceBollardProp.h"

#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
ACarrotBounceBollardProp::ACarrotBounceBollardProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM_BounceBollard.SM_BounceBollard"));
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

	SetSize(FVector(1, 1, 2));

	PropDataComponent->SetPropID(TEXT("2001"));
}

// Called when the game starts or when spawned
void ACarrotBounceBollardProp::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACarrotBounceBollardProp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CoolTimeHandle);
	}
	Super::EndPlay(EndPlayReason);
}

void ACarrotBounceBollardProp::OnBollardHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog)
	{
		if (HitFrogList.Contains(Frog))
		{
			return;
		}

		HitFrogList.Add(Frog);

		TWeakObjectPtr<AFrog> WeakFrog = Frog;
		TWeakObjectPtr<ACarrotBounceBollardProp> WeakBollard = this;
		GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([WeakFrog, WeakBollard]()
		{
			if (WeakFrog.IsValid() && WeakBollard.IsValid())
			{
				AFrog* Frog = WeakFrog.Get();
				ACarrotBounceBollardProp* Bollard = WeakBollard.Get();

				Bollard->ResetHitFrogList(Frog);
			}
		}), CoolTime, false);
	}
	
	if (HasAuthority() && Frog->IsLocallyControlled())
	{
		MulticastRPC_PlayEffect(this->GetActorLocation());
	}
	else if (!HasAuthority() && Frog->IsLocallyControlled())
	{
		Frog->ServerRPC_ProcessOverlap(this);
	}
	
	Super::OnBollardHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ACarrotBounceBollardProp::MulticastRPC_PlayEffect_Implementation(FVector Location, int32 Index)
{
	Super::MulticastRPC_PlayEffect_Implementation(Location, Index);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Location, 0.5f, 1.5f);
}

void ACarrotBounceBollardProp::ResetHitFrogList(AFrog* Frog)
{
	HitFrogList.Remove(Frog);
}

// Called every frame
void ACarrotBounceBollardProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

