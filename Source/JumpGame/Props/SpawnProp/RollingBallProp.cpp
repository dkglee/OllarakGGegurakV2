// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingBallProp.h"

#include "ObjectPoolComponent.h"
#include "Components/ArrowComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"


// Sets default values
ARollingBallProp::ARollingBallProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Tags.Add("RollingBall");

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	PivotScene = CreateDefaultSubobject<USceneComponent>(TEXT("PivotScene"));
	PivotScene->SetupAttachment(RootComponent);
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Dir"));
	Arrow->SetupAttachment(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetRelativeLocation(FVector(-30, 0, 0));
	BoxComp->SetBoxExtent(FVector(35));
	BoxComp->SetCollisionProfileName(TEXT("RollingBall"));
	BoxComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Ignore);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSphere(
		TEXT("/Script/Engine.StaticMesh'/Game/Fab/RollingBall/SM_Acorn.SM_Acorn'"));
	if (TempSphere.Succeeded())
	{
		MeshComp->SetStaticMesh(TempSphere.Object);
	}
	MeshComp->SetRelativeScale3D(FVector(1.f));
	MeshComp->SetCollisionProfileName(TEXT("RollingBall"));
	MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundAsset
	(TEXT("/Game/Sounds/Ques/Bounce_Cue.Bounce_Cue"));
	if (SoundAsset.Succeeded())
	{
		HitSound = Cast<USoundCue>(SoundAsset.Object);
	}

	HitEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffectComp"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset
	(TEXT("/Game/_Resource/FX/VFX_Toolkit_V1/ParticleSystems/356Days/Par_FightCloud_01.Par_FightCloud_01"));
	if (ParticleAsset.Succeeded())
	{
		HitEffectComp->SetTemplate(ParticleAsset.Object);
	}
	HitEffectComp->SetupAttachment(RootComponent);
	HitEffectComp->SetAutoActivate(false);
	// HitEffectComp->SetRelativeScale3D({0.25, 0.25, 0.25});
	// HitEffectComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
}

void ARollingBallProp::OnMyRollingBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                          const FHitResult& Hit)
{
	/*FVector HitLocation = Hit.Location;
	FLog::Log("OnMyRollingBallHit");
	
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog)
	{
		FVector LaunchVelocity = LaunchDir.GetSafeNormal() * 100;
		Frog->LaunchCharacter(LaunchVelocity, false, false);
		FLog::Log("OnMyRollingBallHit Frog");
	}

	// 타이머 초기화
	GetWorld()->GetTimerManager().ClearTimer(PoolTimerHandle);
	ReturnSelf();
	
	if (HasAuthority())
	{
		MulticastRPC_PlayEffect(HitLocation);
	}*/
}

void ARollingBallProp::OnMyRollingBallOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	FVector HitLocation = GetActorLocation();
	FLog::Log("OnMyRollingBallOverlap");
	
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog)
	{
		FVector LaunchVelocity = LaunchDir.GetSafeNormal() * 1'800;
		Frog->LaunchCharacter(LaunchVelocity, true, true);
		FLog::Log("OnMyRollingBallOverlap Frog");
	}

	if (HasAuthority())
	{
		MulticastRPC_PlayEffect(HitLocation);
	}
	// 타이머 초기화
	GetWorld()->GetTimerManager().ClearTimer(PoolTimerHandle);
	float Value = BackTime - LaunchTime;
	Value = FMath::Clamp<float>(Value, 0.f, 4.f);
	ReturnSelf(Value);
}

// Called when the game starts or when spawned
void ARollingBallProp::BeginPlay()
{
	Super::BeginPlay();

	// 동작 동기화
	SetReplicateMovement(true);

	// MeshComp->OnComponentHit.AddDynamic(this, &ARollingBallProp::OnMyRollingBallHit);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARollingBallProp::OnMyRollingBallOverlap);
}

void ARollingBallProp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(PoolTimerHandle);
	}
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ARollingBallProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		RollingBall();
	}

	if (bActive)
	{
		// 발사된 후 시간 계산
		LaunchTime += DeltaTime;
	}
}

void ARollingBallProp::ReturnSelf(float RemainTime)
{
	// 소속 풀 없으면 함수 나가자
	if (ObjectPool == nullptr) return;

	if (HasAuthority())
	{
		LaunchTime = 0;
		SetActive(false);
		ObjectPool->ReturnObject(this, RemainTime);
	}
}

void ARollingBallProp::SetActive(bool bIsActive)
{
	// 활성, 비활성화 여부
	bActive = bIsActive;
	SetActorHiddenInGame(!bIsActive);
	SetActorTickEnabled(bIsActive);
	if (bIsActive)
	{
		MeshComp->SetCollisionProfileName(TEXT("RollingBall"));
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

		BoxComp->SetCollisionProfileName(TEXT("RollingBall"));
		BoxComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}
	else
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ARollingBallProp::LaunchProjectile()
{
	LaunchDir = Arrow->GetForwardVector().GetSafeNormal();
	
	// 발사 후 다시 복귀하는 타이밍
	// 바닥에 닿지않으면, 4초후에 복귀하자
	TWeakObjectPtr<ARollingBallProp> WeakThis(this);
	GetWorld()->GetTimerManager().SetTimer(PoolTimerHandle, FTimerDelegate::CreateLambda([WeakThis]()
	{
		if (!WeakThis.IsValid())
		{
			return;
		}
		ARollingBallProp* This = WeakThis.Get();
		if (This->HasAuthority())
		{
			// 있던 자리에서 터지기
			This->MulticastRPC_PlayEffect(This->GetActorLocation());
		}
		This->ReturnSelf(0);
	}), BackTime, false);
	//
}

void ARollingBallProp::RollingBall()
{
	// 1. 앞으로 이동
	FVector MeshPos = GetActorLocation();
	// 프레임당 이동거리
	FVector MovePos = LaunchDir * (RollingSpeed * GetWorld()->GetDeltaSeconds());
	SetActorLocation(MeshPos + MovePos, true);

	// 2. 회전
	float Distance = MovePos.Size(); 
	float Radius = 60.0f; // 반지름
	float RotationDegrees = FMath::RadiansToDegrees(Distance / Radius);
	
	FVector RotationAxis = FVector::CrossProduct(FVector::RightVector, FVector::UpVector);
	FQuat DeltaQuat = FQuat(RotationAxis, FMath::DegreesToRadians(RotationDegrees));
	// DrawDebugCoordinateSystem(GetWorld(), GetActorLocation(), GetActorRotation(), 100.f, false, -1.f, 0, 2.f);

	// 회전 적용
	MeshComp->AddLocalRotation(DeltaQuat, false, nullptr, ETeleportType::None);
}

void ARollingBallProp::MulticastRPC_PlayEffect_Implementation(FVector Location)
{
	// 클라이언트 전부에서 호출됨 (서버 포함)
	// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Location, FRotator::ZeroRotator, FVector(0.25, 0.25, 0.25), true);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffectComp->Template, Location, FRotator::ZeroRotator, FVector(0.25, 0.25, 0.25), true);
	
	// FFastLogger::LogScreen(FColor::Red, TEXT("Play Effect"));
	// HitEffectComp->SetWorldLocation(Location);
	// HitEffectComp->ActivateSystem();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Location);
}

