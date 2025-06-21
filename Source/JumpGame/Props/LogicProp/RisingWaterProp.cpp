// Fill out your copyright notice in the Description page of Project Settings.


#include "RisingWaterProp.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

ARisingWaterProp::ARisingWaterProp()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMesh(
		TEXT("/Game/Props/Logic/SM_Water.SM_Water"));
	if (WaterMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(WaterMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(12.f, 12.f, 27.f));
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundAsset
(TEXT("/Game/Sounds/Ques/JumpToWater_Cue.JumpToWater_Cue"));
	if (SoundAsset.Succeeded())
	{
		HitSound = Cast<USoundCue>(SoundAsset.Object);
	}
	
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionComp->SetBoxExtent(FVector(600.f, 600.f, 1300.f));
	CollisionComp->SetCollisionProfileName(TEXT("Water"));
	CollisionComp->ComponentTags.Add(TEXT("CameraWater"));

	DeepCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DeepCollision"));
	DeepCollision->SetupAttachment(RootComponent);
	DeepCollision->SetBoxExtent(FVector(500.f, 500.f, 700.f));
	DeepCollision->SetRelativeLocation(FVector(0, 0, -20.f));
	DeepCollision->SetCollisionProfileName(TEXT("Water"));

	ShallowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShallowCollision"));
	ShallowCollision->SetupAttachment(RootComponent);
	ShallowCollision->SetBoxExtent(FVector(500.f, 500.f, 300.f));
	ShallowCollision->SetRelativeLocation(FVector(0, 0, 980.f));
	ShallowCollision->SetCollisionProfileName(TEXT("Water"));

	SurfaceCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SurfaceCollision"));
	SurfaceCollision->SetupAttachment(RootComponent);
	SurfaceCollision->SetBoxExtent(FVector(500.f, 500.f, 10.f));
	SurfaceCollision->SetRelativeLocation(FVector(0, 0, 1290.f));
	SurfaceCollision->SetCollisionProfileName(TEXT("Water"));

	DeadZoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DeadZoneCollision"));
	DeadZoneCollision->SetupAttachment(RootComponent);
	DeadZoneCollision->SetBoxExtent(FVector(500.f, 500.f, 100.f));
	DeadZoneCollision->SetRelativeLocation(FVector(0, 0, -820.f));
	DeadZoneCollision->SetCollisionProfileName(TEXT("Water"));

	RespawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RespawnPoint"));
	RespawnPoint->SetupAttachment(RootComponent);
	RespawnPoint->SetRelativeLocation(FVector(0, 0, 480.f));

	bReplicates = true;
	SetNetUpdateFrequency(100.f);
	SetMinNetUpdateFrequency(33.f);
	SetReplicatingMovement(true);

	WaterState = EWaterStateEnum::None;
	CurrentRisingSpeed = 15.f;
}

void ARisingWaterProp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARisingWaterProp, CurrentRisingSpeed);
	DOREPLIFETIME(ARisingWaterProp, WaterState);
}

void ARisingWaterProp::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(TEXT("Water"));
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ARisingWaterProp::OnEndOverlap);

	DeepCollision->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginDeepOverlap);

	DeadZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginDeadZoneOverlap);

	if (HasAuthority())
	{
		StopRising();
		//GetWorldTimerManager().SetTimer(WaterStartTimer, this, &ARisingWaterProp::StartRising, 120.f, false);
	}
}

void ARisingWaterProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 서버에서만 물 상승
	if (HasAuthority())
	{
		if (WaterState == EWaterStateEnum::Rise)
		{
			RiseWater(DeltaTime);
		}
	}
	
	// if (!Frog)
	// {
	// 	return;
	// }
	//
	// // 일정 시간이 흘렀는데 상태가 "None"인 경우, 강제로 "Surface"로 수정
	// if (!bIsChanged && Frog->bIsSwimming)
	// {
	// 	FlowTime += DeltaTime;
	// 	if (FlowTime > JumpTime && Frog->CharacterState == ECharacterStateEnum::None)
	// 	{
	// 		FlowTime = 0.f;
	// 		Frog->CharacterState = ECharacterStateEnum::Surface;
	// 		bIsChanged = true;
	// 	}
	// }
	//
	// UCharacterMovementComponent* MovementComponent{Frog->GetCharacterMovement()};
	// if (MovementComponent)
	// {
	// 	switch (Frog->CharacterState)
	// 	{
	// 	case ECharacterStateEnum::None:
	// 		// 물 밖 중력 1
	// 		MovementComponent->GravityScale = 1.f;
	// 		break;
	// 	case ECharacterStateEnum::Deep:
	// 		// 물 속 중력 0
	// 		MovementComponent->GravityScale = 0.f;
	// 	// 위쪽으로 강한 힘
	// 		Frog->AddMovementInput(FVector::UpVector, 1000.0f * DeltaTime);
	// 		break;
	// 	case ECharacterStateEnum::Shallow:
	// 		// 물 속 중력 0
	// 		MovementComponent->GravityScale = 0.f;
	// 	// 위쪽으로 약한 힘
	// 		Frog->AddMovementInput(FVector::UpVector, 300.0f * DeltaTime);
	// 		break;
	// 	case ECharacterStateEnum::Surface:
	// 		// 약한 중력 ( 둥둥 떠다니는 느낌 )
	// 		MovementComponent->GravityScale = 0.1f;
	// 	// Z축 속도를 천천히 줄여서 위아래 움직임 댐핑
	// 		MovementComponent->Velocity.Z *= 0.9f;
	// 		break;
	// 	}
	// }
}

void ARisingWaterProp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorldTimerManager().ClearTimer(ResumeRisingTimerHandle);
		GetWorldTimerManager().ClearTimer(TimerHandle);
		GetWorldTimerManager().ClearTimer(WaterStartTimer);
	}
	
	Super::EndPlay(EndPlayReason);
}

void ARisingWaterProp::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	AFrog* OverlappingFrog{Cast<AFrog>(OtherActor)};
	if (OverlappingFrog && OtherComp && OtherComp->ComponentHasTag(TEXT("FrogCapsule")))
	{
		// 서버에서만 처리
		if (HasAuthority())
		{
			//FLog::Log("Speed", OverlappingFrog->GetCharacterMovement()->Velocity.Length());
			// 안빠르면 가라앉지 않게
			if (OverlappingFrog->GetCharacterMovement()->Velocity.Length() < 1'950.f)
			{
				OverlappingFrog->CharacterWaterState = ECharacterStateEnum::Surface;
			}

			// 잠시 가라앉고 올라오게
			TWeakObjectPtr<ARisingWaterProp> WeakThis{this};
			FTimerDelegate MovementModeDelegate{
				FTimerDelegate::CreateLambda([WeakThis]() {
					if (!WeakThis.IsValid())
					{
						return;
					}

					ARisingWaterProp* StrongThis = WeakThis.Get();
					// 기존 바인딩을 제거해서 중복을 방지 후, 새로 바인딩
					StrongThis->ShallowCollision->OnComponentBeginOverlap.RemoveDynamic(StrongThis, &ARisingWaterProp::OnBeginShallowOverlap);
					StrongThis->ShallowCollision->OnComponentBeginOverlap.AddDynamic(StrongThis, &ARisingWaterProp::OnBeginShallowOverlap);
					
					StrongThis->SurfaceCollision->OnComponentBeginOverlap.RemoveDynamic(StrongThis, &ARisingWaterProp::OnBeginSurfaceOverlap);
					StrongThis->SurfaceCollision->OnComponentBeginOverlap.AddDynamic(StrongThis, &ARisingWaterProp::OnBeginSurfaceOverlap);
				})
			};
			GetWorldTimerManager().SetTimer(TimerHandle, MovementModeDelegate, 1.f, false);
			
			OverlappingFrog->ServerRPC_UpdateOverallWaterState(true, this);

			MulticastRPC_PlayEffect(OverlappingFrog->GetActorLocation());
		}
	}
}

void ARisingWaterProp::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFrog* OverlappingFrog = Cast<AFrog>(OtherActor);
	if (OverlappingFrog && OtherComp && OtherComp->ComponentHasTag(TEXT("FrogCapsule")))
	{
		if (HasAuthority())
		{
			GetWorldTimerManager().ClearTimer(TimerHandle);
			
			ShallowCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ARisingWaterProp::OnBeginShallowOverlap);
			SurfaceCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ARisingWaterProp::OnBeginSurfaceOverlap);
			
			OverlappingFrog->ServerRPC_UpdateOverallWaterState(false, this);
		}
	}
}

void ARisingWaterProp::OnBeginDeepOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	AFrog* OverlappingFrog = Cast<AFrog>(OtherActor);
	if (OverlappingFrog && OtherComp && OtherComp->ComponentHasTag(TEXT("FrogCapsule")))
	{
		if (HasAuthority())
		{
			OverlappingFrog->ServerRPC_SetSpecificWaterState(ECharacterStateEnum::Deep);
		}
	}
}

void ARisingWaterProp::OnBeginShallowOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	AFrog* OverlappingFrog = Cast<AFrog>(OtherActor);
	if (OverlappingFrog && OtherComp && OtherComp->ComponentHasTag(TEXT("FrogCapsule")))
	{
		if (HasAuthority())
		{
			OverlappingFrog->ServerRPC_SetSpecificWaterState(ECharacterStateEnum::Shallow);
		}
	}
}

void ARisingWaterProp::OnBeginSurfaceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	AFrog* OverlappingFrog = Cast<AFrog>(OtherActor);
	if (OverlappingFrog && OtherComp && OtherComp->ComponentHasTag(TEXT("FrogCapsule")))
	{
		if (HasAuthority())
		{
			OverlappingFrog->ServerRPC_SetSpecificWaterState(ECharacterStateEnum::Surface);
		}
	}
}

void ARisingWaterProp::OnBeginDeadZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                              const FHitResult& SweepResult)
{
	AFrog* OverlappingFrog = Cast<AFrog>(OtherActor);
	if (OverlappingFrog && OtherComp && OtherComp->ComponentHasTag(TEXT("FrogCapsule")))
	{
		if (HasAuthority())
		{
			if (RespawnPoint)
			{
				OverlappingFrog->ServerRPC_TeleportToLocation(RespawnPoint->GetComponentLocation());
			}
			// 리스폰 후 상태
			OverlappingFrog->ServerRPC_SetSpecificWaterState(ECharacterStateEnum::Deep);
		}
	}
}

void ARisingWaterProp::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);
	
	// DeepCollision->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	// ShallowCollision->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	// SurfaceCollision->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	// DeadZoneCollision->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void ARisingWaterProp::RiseWater(float DeltaTime)
{
	const FVector CurrentLocation = GetActorLocation();
	const FVector NewLocation = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z + (CurrentRisingSpeed * DeltaTime));
	SetActorLocation(NewLocation);
}

void ARisingWaterProp::StopRising(float Time)
{
	// 서버만 실행
	if (HasAuthority())
	{
		WaterState = EWaterStateEnum::None;
		GetWorldTimerManager().ClearTimer(ResumeRisingTimerHandle);

		if (Time > 0.f)
		{
			GetWorldTimerManager().SetTimer(ResumeRisingTimerHandle, this, &ARisingWaterProp::StartRising, Time, false);
		}
	}
}

void ARisingWaterProp::StartRising()
{
	if (HasAuthority())
	{
		WaterState = EWaterStateEnum::Rise;
		GetWorldTimerManager().ClearTimer(ResumeRisingTimerHandle);
	}
}

void ARisingWaterProp::SetRisingSpeed(float Speed)
{
	// 서버에서만 속도 변경 ( CurrentRisingSpeed는 Replicated 속성이므로 클라이언트에 자동 전달됨 )
	if (HasAuthority())
	{
		CurrentRisingSpeed = Speed;
	}
}

float ARisingWaterProp::GetWaterSurfaceZ()
{
	return SurfaceCollision->GetComponentLocation().Z + SurfaceCollision->GetScaledBoxExtent().Z;
}

void ARisingWaterProp::MulticastRPC_PlayEffect_Implementation(FVector Location)
{
	auto SurfaceSound{UGameplayStatics::CreateSound2D(GetWorld(), HitSound)};
	SurfaceSound->Play(0.f);
	//SurfaceSound->FadeOut(1.f, 0.f);
	// UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Location, 1.f, 1.f);

}

void ARisingWaterProp::OnRep_WaterState()
{
	// 추후 추가 가능
	
	// WaterState가 서버에서 변경되어 클라이언트로 복제 완료 시 호출됨
	// 클라이언트에서 물 상태 변경에 따른 시각적 효과 처리
}
