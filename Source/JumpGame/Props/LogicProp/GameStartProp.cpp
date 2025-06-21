// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartProp.h"

#include "Components/BoxComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"


// Sets default values
AGameStartProp::AGameStartProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("GameStart");
	
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleBaseCube.SM_ObstacleBaseCube'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	MeshComp->SetRelativeScale3D(FVector(5.f,20.f,1.f));
	CollisionComp->SetBoxExtent(FVector(250,1000,50));
	
	for (int32 i = 0; i < 6; i++)
	{
		FString Name = FString::Printf(TEXT("StartPos%i"),i);
		USceneComponent* StartPoint = CreateDefaultSubobject<USceneComponent>(*Name);
		StartPoint->SetupAttachment(RootComponent);
		
		// 위치 지정
		FVector Offset = FVector(0.f, i * Gap - 550, 0.f) + FVector(0,0,100.f);
		StartPoint->AddLocalOffset(Offset);

		StartPoints.Add(StartPoint);
	}

	Super::SetSize(FVector(12,20,5));
}

// Called when the game starts or when spawned
void AGameStartProp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameStartProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FTransform AGameStartProp::PlayerStartTransform()
{
	PlayerIdx %= StartPoints.Num();
		
	// 스타트 포인트 값을 반환
	FFastLogger::LogScreen(FColor::Red, TEXT("StartPoint: %s"), *StartPoints[PlayerIdx]->GetComponentTransform().ToString());
	return StartPoints[PlayerIdx++]->GetComponentTransform();
}

/*FTransform AGameStartProp::SinglePlayerStart()
{
	// 스타트 포인트 값을 반환
	return StartPoints[0]->GetComponentTransform();
}*/