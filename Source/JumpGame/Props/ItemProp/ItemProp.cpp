// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemProp.h"

#include "Components/BoxComponent.h"


// Sets default values
AItemProp::AItemProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemProp::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AItemProp::OnBeginOverlap);
	
	InitializeProp();
}

// Called every frame
void AItemProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemProp::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EatItem();
}

void AItemProp::InitializeProp()
{
	LastingEffect();
}

void AItemProp::EatItem()
{
	EatUsefulEffect();
	EatVisualEffect();
	
	RemoveItem();
}

void AItemProp::EatUsefulEffect()
{
}

void AItemProp::EatVisualEffect()
{
}

void AItemProp::LastingEffect()
{
}

void AItemProp::RemoveItem()
{
	Destroy();
}
