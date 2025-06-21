// Fill out your copyright notice in the Description page of Project Settings.


#include "DecorationProp.h"

#include "Components/BoxComponent.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"


// Sets default values
ADecorationProp::ADecorationProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
	CollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComp->SetCollisionProfileName(TEXT("OverlapProp"));
	MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// 그리드 충돌 허용
	bEnableCollision = false;
	CollisionComp->bReplicatePhysicsToAutonomousProxy = false;
	GridInnerCollision->bReplicatePhysicsToAutonomousProxy = false;
	GridOuterCollision->bReplicatePhysicsToAutonomousProxy = false;
	MeshComp->bReplicatePhysicsToAutonomousProxy = false;
	RotateGizmo->bReplicatePhysicsToAutonomousProxy = false;
	GizmoPrimary->bReplicatePhysicsToAutonomousProxy = false;
	GizmoOne->bReplicatePhysicsToAutonomousProxy = false;
	GizmoTwo->bReplicatePhysicsToAutonomousProxy = false;
	GizmoThree->bReplicatePhysicsToAutonomousProxy = false;
	GizmoFour->bReplicatePhysicsToAutonomousProxy = false;
	GizmoFive->bReplicatePhysicsToAutonomousProxy = false;
	GizmoSix->bReplicatePhysicsToAutonomousProxy = false;
}

// Called when the game starts or when spawned
void ADecorationProp::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		TWeakObjectPtr<ADecorationProp> WeakThis = this;
		GetWorld()->GetTimerManager().SetTimer(ReplicateTimerHandle, FTimerDelegate::CreateLambda([WeakThis]()
		{
			if (!WeakThis.IsValid())
			{
				return ;
			}
			ADecorationProp* StrongThis = WeakThis.Get();
			StrongThis->SetReplicates(false);
			StrongThis->SetReplicateMovement(false);
		}), 60.0f, false);
	}
}

// Called every frame
void ADecorationProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

