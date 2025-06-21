// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateSpikeProp.h"

#include "ToolContextInterfaces.h"
#include "Components/BoxComponent.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "CableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ARotateSpikeProp::ARotateSpikeProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("RotateSpike");

	MeshComp->SetupAttachment(RootComponent);
	
	Hammer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hammer"));
	Hammer->SetupAttachment(PivotScene);
	Hammer->SetRelativeLocation(FVector(0, 180, -40));
	
	CollisionComp->SetBoxExtent(FVector(116,40,42));
	CollisionComp->SetRelativeLocation(FVector(-3,-6,40));
	CollisionComp->SetupAttachment(Hammer);

	// 메쉬랑 부딪힘 (CollisionComp랑은 충돌하지 않는다)
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	MeshComp->SetCollisionProfileName(TEXT("Prop"));
	Hammer->SetCollisionProfileName(TEXT("Prop"));
	Hammer->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	CableComp = CreateDefaultSubobject<UCableComponent>(TEXT("CableComp"));
	CableComp->SetupAttachment(PivotScene);
	CableComp->bAttachStart = true;
	CableComp->bAttachEnd = true;
	CableComp->CableLength = 50;
	CableComp->NumSegments = 2;
	CableComp->EndLocation = FVector(0.000000,32.261419,-3.143982);
	CableComp->SetRelativeLocation(FVector(-0.000000,144.798420,-3.422057));
	CableComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CableMesh"));
	CableMesh->SetupAttachment(CableComp);
	CableMesh->SetRelativeLocation(FVector(0.000004,29.778368,-6.061648));
	CableMesh->SetRelativeScale3D(FVector(0.075000,0.075000,0.150000));
	CableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CableTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CableTop"));
	CableTop->SetupAttachment(MeshComp);
	CableTop->SetRelativeLocation(FVector(0.000008,0.137836,57.444227));
	CableTop->SetRelativeScale3D(FVector(0.150000,0.150000,0.250000));
	CableTop->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Propeller = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Propeller"));
	Propeller->SetupAttachment(RootComponent);
	Propeller->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Propeller->SetAbsolute(true, true, true);

	PropDataComponent->SetPropID(TEXT("2008"));

	static ConstructorHelpers::FObjectFinder<USoundCue> TempSound
	(TEXT("/Game/Sounds/Ques/SwingWind_Cue.SwingWind_Cue"));
	if (TempSound.Succeeded())
	{
		HitSound = TempSound.Object;
	}
}

// Called when the game starts or when spawned
void ARotateSpikeProp::BeginPlay()
{
	Super::BeginPlay();

	SwingHammer();
}

void ARotateSpikeProp::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARotateSpikeProp, HammerAngle);
}

void ARotateSpikeProp::MulticastRPC_PlayEffect_Implementation(FVector Location, int32 Index)
{
	// Super::MulticastRPC_PlayEffect_Implementation(Location);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Location, 0.5f, 2.0f);
}

// Called every frame
void ARotateSpikeProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float PropellerSpeed = 500.f;
	float DeltaAngle = PropellerSpeed * DeltaTime;
	FRotator NewRot = FRotator(0, DeltaAngle, 0);
	Propeller->AddLocalRotation(NewRot);

	// 헤머의 회전이 Pivot 기준으로 돌고 있기 때문에 물체랑 충돌이 나도 무시됨.
	// 그래서 그냥 한번 더 SetLocation을 해서 강제로 Sweep을 해버리자!
	if (HasAuthority())
	{
		MeshComp->SetWorldLocation(MeshComp->GetComponentLocation(), true, nullptr, ETeleportType::None);
	}
}

void ARotateSpikeProp::CalculateForce(AFrog* Character)
{
	Super::CalculateForce(Character);
}

void ARotateSpikeProp::OnRep_SwingHammer()
{
	PivotScene->SetRelativeRotation(FRotator(0, HammerAngle, 0));
}

void ARotateSpikeProp::SwingHammer_Implementation()
{
}

void ARotateSpikeProp::ReverseSwingHammer_Implementation()
{
}

inline void ARotateSpikeProp::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);
	
	if (bEnable)
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Hammer->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// Spike->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Hammer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// Spike->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// TODO: Material 불투명하게 바꿔주기
	}
}
