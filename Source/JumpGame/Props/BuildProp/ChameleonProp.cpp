// Fill out your copyright notice in the Description page of Project Settings.


#include "ChameleonProp.h"

#include "JumpGame/MapEditor/Components/GridComponent.h"


// Sets default values
AChameleonProp::AChameleonProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChameleonProp::BeginPlay()
{
	Super::BeginPlay();

	if (MeshComp)
	{
		OriginalMesh = MeshComp->GetStaticMesh();
		OriginalMaterial = MeshComp->GetMaterial(0);
	}
}

// Called every frame
void AChameleonProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChameleonProp::CopyMeshAndMaterial()
{
	if (MeshComp)
	{
		FVector Start{GetActorLocation()};
		FVector End{Start - FVector(0, 0, 55)};

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			ABaseProp* BaseProp{Cast<ABaseProp>(HitResult.GetActor())};

			// SetSize 1, 1, 1 인 녀석만 일단
			if (BaseProp && BaseProp->MeshComp && BaseProp->GetGridComp()->GetSize() == FVector(1, 1, 1))
			{
				// Mesh 복사
				UStaticMesh* BottomMesh{BaseProp->MeshComp->GetStaticMesh()};
				if (BottomMesh)
				{
					MeshComp->SetStaticMesh(BottomMesh);
					// Scale 복사
					MeshComp->SetRelativeScale3D(BaseProp->MeshComp->GetRelativeScale3D());
				}

				// 머티리얼 복사
				int32 NumMaterials{BaseProp->MeshComp->GetNumMaterials()};
				for (int32 i{}; i < NumMaterials; ++i)
				{
					UMaterialInterface* Mat{BaseProp->MeshComp->GetMaterial(i)};
					if (Mat)
					{
						MeshComp->SetMaterial(i, Mat);
					}
				}
			}
		}
	}
}

void AChameleonProp::RecoverMeshAndMaterial()
{
	if (MeshComp && OriginalMesh)
	{
		MeshComp->SetStaticMesh(OriginalMesh);
		MeshComp->SetMaterial(0, OriginalMaterial);
	}
}

