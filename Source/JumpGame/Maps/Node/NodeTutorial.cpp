// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeTutorial.h"

#include "Components/WidgetComponent.h"
#include "JumpGame/UI/StageNode/NodeTutorialUI.h"


// Sets default values
ANodeTutorial::ANodeTutorial()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TutorialWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TutorialWidget"));
	TutorialWidget->SetupAttachment(RootComponent);
	TutorialWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TutorialWidget->SetWidgetClass(NodeTutorialUIClass);
}

// Called when the game starts or when spawned
void ANodeTutorial::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANodeTutorial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

