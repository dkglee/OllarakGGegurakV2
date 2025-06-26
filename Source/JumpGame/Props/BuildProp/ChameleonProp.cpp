// Fill out your copyright notice in the Description page of Project Settings.


#include "ChameleonProp.h"

#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "Kismet/GameplayStatics.h"


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

    // CopyMeshAndMaterial();
	if (bInGame)
	{
		CopyMeshAndMaterial();
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

		for (int32 i{}; i < 20; ++i)
		{
			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			bool bHit{GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params)};

			if (!bHit)
			{
				return;
			}

			// ChameleonProp 인지 확인
			AChameleonProp* HitChameleon{Cast<AChameleonProp>(HitResult.GetActor())};
			if (HitChameleon)
			{
				// 더 아래로 End를 내리고 계속 검사
				Start = End;
				End = Start - FVector(0, 0, 50);
				continue;
			}

			// BaseProp 인지 확인
			ABaseProp* BaseProp{Cast<ABaseProp>(HitResult.GetActor())};
			if (BaseProp && BaseProp->MeshComp && BaseProp->GetGridComp()->GetSize() == FVector(1, 1, 1))
			{
				// Mesh 복사
				UStaticMesh* BottomMesh{BaseProp->MeshComp->GetStaticMesh()};
				if (BottomMesh)
				{
					MeshComp->SetStaticMesh(BottomMesh);
					MeshComp->SetRelativeTransform(BaseProp->MeshComp->GetRelativeTransform());
				}

				// 머티리얼 복사
				int32 NumMaterials{BaseProp->MeshComp->GetNumMaterials()};
				for (int32 j{}; j < NumMaterials; ++j)
				{
					UMaterialInterface* Mat{BaseProp->MeshComp->GetMaterial(j)};
					if (Mat)
					{
						MeshComp->SetMaterial(j, Mat);
					}
				}
			}
			
			break;
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

