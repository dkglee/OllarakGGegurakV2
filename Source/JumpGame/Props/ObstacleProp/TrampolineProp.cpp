// Fill out your copyright notice in the Description page of Project Settings.


#include "TrampolineProp.h"

#include <filesystem>

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/Utils/FastLogger.h"


// Sets default values
ATrampolineProp::ATrampolineProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Trampoline");

	CollisionComp->SetCollisionProfileName("OverlapProp");
}

// Called when the game starts or when spawned
void ATrampolineProp::BeginPlay()
{
	Super::BeginPlay();

	// 콜리전 설정 (Block)
	// CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called every frame
void ATrampolineProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATrampolineProp::CalculateForce(AFrog* Character)
{
	FFastLogger::LogScreen(FColor::Red, TEXT("Trampoline"));
	//FVector CharacterDir = Character->GetVelocity().GetSafeNormal();
	//FVector Direction = CharacterDir + FVector::UpVector;
	
	FVector Direction{CollisionComp->GetUpVector()};
	
	float Force;
	if (temp)
	{
		Force = FMath::Clamp(static_cast<float>(Character->PrevVelocityZLength), 1'300.f, 2'700.f);
	}
	else
	{
		Force = FMath::Clamp(static_cast<float>(Character->PrevVelocityZLength), 1'300.f, 1'300.f);
	}
	
	Force *= 1.2;

	FFastLogger::LogScreen(FColor::Emerald, TEXT("Force: %.1f"), Force);
	
	if (bDebug)
	{
		FLog::Log("Z Speed", Character->PrevVelocityZLength);
	}
	
	Super::LaunchCharacter(Character, Direction, Force, false, true);
}