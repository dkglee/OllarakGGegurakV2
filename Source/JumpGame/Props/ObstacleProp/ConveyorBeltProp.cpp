// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBeltProp.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AConveyorBeltProp::AConveyorBeltProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("ConveyorBelt");

	CollisionComp->SetBoxExtent(FVector(100.f, 50.f, 70.f));
	CollisionComp->SetRelativeLocation(FVector(0, 0, 0));
	MeshComp->SetRelativeScale3D(FVector(2, 1, 1));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(110, 0, 0));

	// 컨베이어 벨트 콜리전과 overlap
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));

	Super::SetSize(FVector(2, 1, 1));

	PropDataComponent->SetPropID(TEXT("2005"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> ConveyorWaterFinder
	(TEXT("/Game/Props/Obstacle/SM_ConveyorWater.SM_ConveyorWater"));
	if (ConveyorWaterFinder.Succeeded())
	{
		WaterMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterMeshComp"));
		WaterMeshComp->SetStaticMesh(ConveyorWaterFinder.Object);
		WaterMeshComp->SetupAttachment(MeshComp);
		WaterMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WaterMeshComp->SetRelativeLocation(FVector(0, 0, 60.f));
		WaterMeshComp->SetRelativeScale3D(FVector(1, 2.f, 1));
		WaterMeshComp->SetRelativeRotation(FRotator(0, 90.f, 0));
	}
	
	ConstructorHelpers::FObjectFinder<UMaterialInterface> ConveyorMaterialFinder
		(TEXT("/Game/Props/Logic/M_ConveyorBeltWater_Inst.M_ConveyorBeltWater_Inst"));
	if (ConveyorMaterialFinder.Succeeded())
	{
		ConveyorMaterial = ConveyorMaterialFinder.Object;
	}
}

void AConveyorBeltProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog && !OverlappingFrogs.Contains(Frog))
	{
		OverlappingFrogs.Add(Frog);
	}

	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AConveyorBeltProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog)
	{
		OverlappingFrogs.Remove(Frog);
	}

	Super::OnMyEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

// Called when the game starts or when spawned
void AConveyorBeltProp::BeginPlay()
{
	Super::BeginPlay();
	
	ConveyorDynamicMaterial = UMaterialInstanceDynamic::Create(ConveyorMaterial, this);
	WaterMeshComp->SetMaterial(0, ConveyorDynamicMaterial);
	
	UpdateWaterMaterialRotation();
}

// Called every frame
void AConveyorBeltProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 클라 서버 둘다 각자 실행
	BeltDir = Arrow->GetForwardVector().GetSafeNormal();
	ConveyorMove();
}

void AConveyorBeltProp::ConveyorMove()
{
	CopyOverlapFrogs = OverlappingFrogs;
	int index = 0;
	// 유효한 Frog들만 처리
	for (AFrog* Frog : CopyOverlapFrogs)
	{
		if (!IsValid(Frog))
			continue;

		FVector DeltaPos = BeltSpeed * GetWorld()->DeltaTimeSeconds * BeltDir;

		// TODO: 택1
		Frog->AddActorWorldOffset(DeltaPos, true, nullptr, ETeleportType::TeleportPhysics);
		// Frog->SetActorLocation(Frog->GetActorLocation() + DeltaPos);		
	}
}

void AConveyorBeltProp::ChangeAllMaterialsToUnSelect()
{
	Super::ChangeAllMaterialsToUnSelect();
	
	WaterMeshComp->SetMaterial(0, ConveyorDynamicMaterial);
	WaterMeshComp->SetForceDisableNanite(false);
}

void AConveyorBeltProp::ChangeAllMaterialsToSelect()
{
	Super::ChangeAllMaterialsToSelect();

	WaterMeshComp->SetForceDisableNanite(true);
	WaterMeshComp->SetMaterial(0, SelectedObjectMaterial);
}

void AConveyorBeltProp::ChangeAllMaterialsToOnCollision()
{
	Super::ChangeAllMaterialsToOnCollision();

	WaterMeshComp->SetForceDisableNanite(true);
	WaterMeshComp->SetMaterial(0, OnCollisionObjectMaterial);
}

void AConveyorBeltProp::RotateAllGizmos()
{
	Super::RotateAllGizmos();

	UpdateWaterMaterialRotation();
}

void AConveyorBeltProp::UpdateWaterMaterialRotation()
{
	float YawRotation{static_cast<float>(GetActorRotation().Yaw)};

	if (YawRotation > 400.0f)
	{
		YawRotation -= 360.0f;
	}
	if (YawRotation < -30.f)
	{
		YawRotation += 360.0f;
	}

	float RotateValue{1.f - ((YawRotation + 90.f) / 90.f) * 0.25f};
	FLog::Log("YawRotation, RotateValue", YawRotation, RotateValue);
	ConveyorDynamicMaterial->SetScalarParameterValue(TEXT("Rotator"), RotateValue);
}
