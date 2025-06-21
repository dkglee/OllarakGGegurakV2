// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProp.h"

#include "Components/BoxComponent.h"


// Sets default values
ABaseProp::ABaseProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Super::SetReplicateMovement(true);
	
	Tags.Add("Prop");

	PivotScene = CreateDefaultSubobject<USceneComponent>(TEXT("PivotScene"));
	PivotScene->SetupAttachment(RootComponent);
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(50.f));
	CollisionComp->SetupAttachment(PivotScene);
	CollisionComp->SetCollisionProfileName(TEXT("Prop"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionProfileName(TEXT("Prop"));
}

// Called when the game starts or when spawned
void ABaseProp::BeginPlay()
{
	Super::BeginPlay();
	
	// 동작 동기화
	SetReplicateMovement(true);

	// UnSelectedObjectMaterial = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
	for (int32 i = 0; i < MeshComp->GetNumMaterials(); i++)
	{
		UMaterialInstanceDynamic* DynamicMaterial = MeshComp->CreateAndSetMaterialInstanceDynamic(i);
		if (DynamicMaterial)
		{
			UnSelectedObjectMaterials.Add(DynamicMaterial);
		}
	}

	bInGame = GetWorld()->GetMapName().Contains(TEXT("InGameLevel"));

	
}

void ABaseProp::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);
	
	// 장애물에 추가 메쉬가 있는 경우에 override
	if (bEnable)
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// 추가 메쉬 설정
		ChangeAllMaterialsToUnSelect();
		MeshComp->SetRenderCustomDepth(false);
	}
	else
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 추가 메쉬 설정
		// TODO: Material 불투명하게 바꿔주기
		ChangeAllMaterialsToSelect();
		MeshComp->SetRenderCustomDepth(true);
	}
}


// Called every frame
void ABaseProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (!bInGame)
	// {
	// 	MaterialChangeOnTick();
	// }
}

void ABaseProp::SetPropActive(bool Value)
{
	bIsActive = Value;
}

void ABaseProp::SetPropCheatMode(bool Value)
{
	bCheatMode = Value;
}

void ABaseProp::MaterialChangeOnCollision()
{
	Super::MaterialChangeOnCollision();

	if (bSelected && bIsOnCollision)
	{
		ChangeAllMaterialsToOnCollision();
		return ;
	}
	if (bSelected)
	{
		ChangeAllMaterialsToSelect();
		return ;
	}
	if (bIsOnCollision)
	{
		ChangeAllMaterialsToOnCollision();
		return ;
	}
	ChangeAllMaterialsToUnSelect();
}

void ABaseProp::ChangeAllMaterialsToUnSelect()
{
	Super::ChangeAllMaterialsToUnSelect();

	for (int32 i = 0; i < MeshComp->GetNumMaterials(); i++)
	{
		if (UnSelectedObjectMaterials.IsValidIndex(i))
		{
			MeshComp->SetMaterial(i, UnSelectedObjectMaterials[i]);
		}
	}
	MeshComp->SetForceDisableNanite(false);
}

void ABaseProp::ChangeAllMaterialsToSelect()
{
	Super::ChangeAllMaterialsToSelect();

	MeshComp->SetForceDisableNanite(true);
	for (int32 i = 0; i < MeshComp->GetNumMaterials(); i++)
	{
		MeshComp->SetMaterial(i, SelectedObjectMaterial);
	}
}

void ABaseProp::ChangeAllMaterialsToOnCollision()
{
	Super::ChangeAllMaterialsToOnCollision();

	MeshComp->SetForceDisableNanite(true);
	for (int32 i = 0; i < MeshComp->GetNumMaterials(); i++)
	{
		MeshComp->SetMaterial(i, OnCollisionObjectMaterial);
	}
}
