// Fill out your copyright notice in the Description page of Project Settings.


#include "BounceBollardProp.h"

#include "Components/BoxComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ABounceBollardProp::ABounceBollardProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("BounceBollard");

	// NOTE: scale로 0해버리면 밑의 자식들 안보임 Extent로 하자
	CollisionComp->SetBoxExtent(FVector(FVector::ZeroVector));


	// 메쉬랑 부딪힘 (CollisionComp랑은 충돌하지 않는다)
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	MeshComp->SetCollisionProfileName(TEXT("Prop"));

}

void ABounceBollardProp::OnBollardHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AFrog* Frog = Cast<AFrog>(OtherActor);
	
	CalculateForce(Frog);
}

// Called when the game starts or when spawned
void ABounceBollardProp::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// 콜리전 설정 (Block)
	// MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeshComp->OnComponentHit.AddDynamic(this, &ABounceBollardProp::OnBollardHit);
}

// Called every frame
void ABounceBollardProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABounceBollardProp::CalculateForce(AFrog* Character)
{
	FVector CharacterDir = Character->GetVelocity().GetSafeNormal();
	// Z축은 무시하자
	CharacterDir.Z = 0;
	FVector Direction = CharacterDir*(-1);
	// DrawDebugLine(GetWorld(),GetActorLocation(),GetActorLocation()+Direction*200,FColor::Red, false, 3.0f, 0, 2.f);
	float Force = 1'500;
	
	Super::LaunchCharacter(Character, Direction, Force, true, true);
}

