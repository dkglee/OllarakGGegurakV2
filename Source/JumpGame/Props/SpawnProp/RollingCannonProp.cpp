// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingCannonProp.h"

#include "RollingBallProp.h"
#include "Components/BoxComponent.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ARollingCannonProp::ARollingCannonProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Tags.Add("Cannon");
	
	ObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ObjectPool"));

	// 콜리전 없음
	// CollisionComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	// CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
	// CollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetCollisionProfileName(TEXT("Prop"));
	MeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
}

void ARollingCannonProp::OnProjectileReturn()
{
	// 서버에서만 처리
	// OnObjectReturned은 모든 클라이언트에서 호출되어, FireRollingBall() 중복될 수도 있음
	if (HasAuthority())
	{
		// 알림 받으면 다시 발사!
		FireRollingBall();
	}
}

// Called when the game starts or when spawned
void ARollingCannonProp::BeginPlay()
{
	Super::BeginPlay();

	// 서버에서만
	if (HasAuthority() && ObjectPool)
	{
		// 블루 프린트에서 호출 (애니메이션 재생 후 발사)
		FireRollingBall();
		ObjectPool->OnObjectReturn.AddDynamic(this, &ARollingCannonProp::OnProjectileReturn);
	}
}

void ARollingCannonProp::Destroyed()
{
	ObjectPool->DestoryObjectPool();
	
	Super::Destroyed();
}

void ARollingCannonProp::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);
}

// Called every frame
void ARollingCannonProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 블루 프린트에서 호출
void ARollingCannonProp::FireRollingBall()
{
	if (!ObjectPool) return;

	this->MulticastRPC_PlayEffect(FVector::ZeroVector, 0);
}

void ARollingCannonProp::FireRollingBallAfterAnimation()
{
	ARollingBallProp* Projectile = ObjectPool->GetRollingBallProp();
	if (Projectile)
	{
		Projectile->SetActorLocation(GetActorLocation());
		Projectile->SetActorRotation(GetActorRotation());
		Projectile->SetActive(true);
		Projectile->LaunchProjectile();
		MulticastRPC_PlayEffect(GetActorLocation(), 1);
	}
}


