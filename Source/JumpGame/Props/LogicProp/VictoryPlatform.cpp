// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryPlatform.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AVictoryPlatform::AVictoryPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 밑바닥
	MeshComp->SetRelativeLocation(FVector(0, 0, -90));
	MeshComp->SetRelativeScale3D(FVector(4,4,0.5));

	// 콜리전
	GridInnerCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	// 카메라
	VictoryCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VictoryCamera"));
	VictoryCamera->SetupAttachment(CollisionComp);
	VictoryCamera->FieldOfView = 55.0f;
	VictoryCamera->SetRelativeLocation(FVector(0, 600, 80));
	VictoryCamera->SetRelativeRotation(FRotator(0, -90, 0));
	OriginalCameraPos = VictoryCamera->GetRelativeLocation();

	// 배경
	// VictoryPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VictoryPlane"));
	// VictoryPlane->SetupAttachment(MeshComp);
	// VictoryPlane->SetRelativeLocation(FVector(0, -50, 500));
	// VictoryPlane->SetRelativeRotation(FRotator(0, 0, 90));
	// VictoryPlane->SetRelativeScale3D(FVector(4, 2, 1));

	// 무덤
	VictoryTomb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VictoryTomb"));
	VictoryTomb->SetupAttachment(CollisionComp);
	VictoryTomb->SetRelativeLocation(FVector(200, 22, 630)); //-3이 원위치
	VictoryTomb->SetRelativeRotation(FRotator(0,-268, 0));
	VictoryTomb->SetRelativeScale3D(FVector(1, 1, 1));

	// 꽃, 당근
	VictoryFlower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VictoryFlower"));
	VictoryFlower->SetupAttachment(CollisionComp);
	VictoryFlower->SetRelativeLocation(FVector(90, 58, -53));
	VictoryFlower->SetRelativeRotation(FRotator(-80, 24, 15));
	
	VictoryCarrot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VictoryCarrot"));
	VictoryCarrot->SetRelativeLocation(FVector(185, 75, -56));
	VictoryCarrot->SetRelativeRotation(FRotator(43, -124, -3));
	VictoryCarrot->SetRelativeScale3D(FVector(0.8));
	VictoryCarrot->SetupAttachment(CollisionComp);

	// 나무
	VictoryTreeOne = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VictoryTreeOne"));
	VictoryTreeOne->SetupAttachment(CollisionComp);
	VictoryTreeOne->SetRelativeLocation(FVector(-195, -70, -50));
	VictoryTreeOne->SetRelativeScale3D(FVector(0.7f));
	
	VictoryTreeTwo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VictoryTreeTwo"));
	VictoryTreeTwo->SetupAttachment(CollisionComp);
	VictoryTreeTwo->SetRelativeLocation(FVector(-240, 57, -50));
	VictoryTreeTwo->SetRelativeScale3D(FVector(0.7f));

	// 1등 정렬 화살표
	VictoryArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("VictoryArrow"));
	VictoryArrow->SetupAttachment(MeshComp);
	VictoryArrow->SetRelativeRotation(FRotator(0, 90, 0));

	// 조명
	VictoryLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("VictoryLight"));
	VictoryLight->SetupAttachment(PivotScene);
	VictoryLight->SetRelativeLocation(FVector(0,-30,650));
	VictoryLight->SetRelativeRotation(FRotator(-90,0, 0));
	VictoryLight->SetIntensity(10);
	VictoryLight->SetIntensityUnits(ELightUnits::Lumens);
	VictoryLight->AttenuationRadius = 900;
	VictoryLight->InnerConeAngle = 20;
	VictoryLight->OuterConeAngle = 40;
	
	// 메쉬
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Game/Fab/LowPolySeparate/platform_02.platform_02"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempTomb(TEXT("/Game/_Resource/Model/SM_MomTomb.SM_MomTomb"));
	if (TempTomb.Succeeded())
	{
		VictoryTomb->SetStaticMesh(TempTomb.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempFlower(TEXT("/Game/UI/Story/SM_StoryFlower.SM_StoryFlower"));
	if (TempFlower.Succeeded())
	{
		VictoryFlower->SetStaticMesh(TempFlower.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempCarrot(TEXT("/Game/UI/Story/SM_StoryCarrot.SM_StoryCarrot"));
	if (TempCarrot.Succeeded())
	{
		VictoryCarrot->SetStaticMesh(TempCarrot.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempTreeOne(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Tree-01.SM-Tree-01"));
	if (TempTreeOne.Succeeded())
	{
		VictoryTreeOne->SetStaticMesh(TempTreeOne.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempTreeTwo(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Tree-03.SM-Tree-03"));
	if (TempTreeTwo.Succeeded())
	{
		VictoryTreeTwo->SetStaticMesh(TempTreeTwo.Object);
	}
	// ConstructorHelpers::FObjectFinder<UStaticMesh> TempPlane(TEXT("/Game/Props/SM_ObstaclePlane.SM_ObstaclePlane"));
	// if (TempPlane.Succeeded())
	// {
	// 	VictoryPlane->SetStaticMesh(TempPlane.Object);
	// }

	// // 재질적용
	// static ConstructorHelpers::FObjectFinder<UMaterialInterface> PlaneMaterial(TEXT("/Game/UI/Image/LogoAnim/MI_RingMask.MI_RingMask"));
	// if (PlaneMaterial.Succeeded())
	// {
	// 	VictoryPlane->SetMaterial(0, PlaneMaterial.Object);
	// }
	
	Tags.Add("VictoryPlatform");
}

// Called when the game starts or when spawned
void AVictoryPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicates(true);
	SetReplicateMovement(true);
}

void AVictoryPlatform::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TombDropHandle);
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AVictoryPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldShake && VictoryCamera)
	{
		ElapsedShakeTime += DeltaTime;

		if (ElapsedShakeTime > ShakeDuration)
		{
			bShouldShake = false;
			VictoryCamera->SetRelativeLocation(OriginalCameraPos);
			return;
		}

		ShakeTime += DeltaTime;

		float ZOffset = FMath::Sin(ShakeTime * ShakeFrequency * 2 * PI) * ShakeAmplitude;

		FVector NewLocation = OriginalCameraPos + FVector(0.f, 0.f, ZOffset);
		VictoryCamera->SetRelativeLocation(NewLocation);
	}
}

FVector AVictoryPlatform::SpawnVictoryCharacter()
{
	FVector SpawnLocation = MeshComp->GetComponentLocation() + FVector(5.f, 5.f, 100.f);
	return SpawnLocation;
}

void AVictoryPlatform::DropProps()
{
	// 무덤 위치 떨어뜨리기
	TWeakObjectPtr<AVictoryPlatform> WeakThis{this};
	GetWorldTimerManager().SetTimer(TombDropHandle, [WeakThis]()
	{
		if (!WeakThis.IsValid())
		{
			return;
		}
		AVictoryPlatform* StrongThis = WeakThis.Get();

		StrongThis->bShouldShake = true;
		StrongThis->VictoryTomb->SetRelativeLocation(FVector(200, 22, -3));
		// 카메라 쉐이크 인척
		StrongThis->StartCameraShake();
	}, 1.0f, false);
}

void AVictoryPlatform::StartCameraShake()
{
	if (!VictoryCamera) return;

	UE_LOG(LogTemp, Log, TEXT("Starting camera shake @@@@@@@@@@@@"));
	
	ShakeTime = 0.f;
	ElapsedShakeTime = 0.f;
	bShouldShake = true;
}

