// Fill out your copyright notice in the Description page of Project Settings.


#include "Frog.h"

#include "CharacterTrajectoryComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "JumpGame/Props/LogicProp/RisingWaterProp.h"
#include "JumpGame/Props/ObstacleProp/ObstacleProp.h"
#include "JumpGame/UI/GameSettingUI.h"
#include "JumpGame/UI/Character/EmotionUI.h"
#include "JumpGame/UI/Character/JumpGaugeUI.h"
#include "Kismet/GameplayStatics.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFrog::AFrog()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	Super::SetReplicateMovement(true);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> FrogMesh
		(TEXT("/Game/Characters/Fat_Frog/SM_Frog17.SM_Frog17"));
	if (FrogMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(FrogMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -75), FRotator(0, -90, 0));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> FrogABP
		(TEXT("/Game/Characters/ABP_Frog.ABP_Frog_C"));
	if (FrogABP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(FrogABP.Class);
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> FrogIMC
		(TEXT("/Game/Characters/Input/IMC_Frog.IMC_Frog"));
	if (FrogIMC.Succeeded())
	{
		DefaultMappingContext = FrogIMC.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Move
		(TEXT("/Game/Characters/Input/IA_FrogMove.IA_FrogMove"));
	if (Frog_Move.Succeeded())
	{
		MoveAction = Frog_Move.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Jump
		(TEXT("/Game/Characters/Input/IA_FrogJump.IA_FrogJump"));
	if (Frog_Jump.Succeeded())
	{
		JumpAction = Frog_Jump.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Look
		(TEXT("/Game/Characters/Input/IA_FrogLook.IA_FrogLook"));
	if (Frog_Look.Succeeded())
	{
		LookAction = Frog_Look.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Crouch
		(TEXT("/Game/Characters/Input/IA_FrogCrouch.IA_FrogCrouch"));
	if (Frog_Crouch.Succeeded())
	{
		CrouchAction = Frog_Crouch.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Sprint
		(TEXT("/Game/Characters/Input/IA_FrogSprint.IA_FrogSprint"));
	if (Frog_Sprint.Succeeded())
	{
		SprintAction = Frog_Sprint.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_TongueAttack
		(TEXT("/Game/Characters/Input/IA_FrogTongueAttack.IA_FrogTongueAttack"));
	if (Frog_TongueAttack.Succeeded())
	{
		TongueAttackAction = Frog_TongueAttack.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_DebugMode
		(TEXT("/Game/Characters/Input/IA_DebugMode.IA_DebugMode"));
	if (Frog_Sprint.Succeeded())
	{
		DebugModeAction = Frog_DebugMode.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_PropActive
		(TEXT("/Game/Characters/Input/IA_PropActive.IA_PropActive"));
	if (Frog_Sprint.Succeeded())
	{
		PropActiveAction = Frog_PropActive.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_PropCheat
		(TEXT("/Game/Characters/Input/IA_PropCheat.IA_PropCheat"));
	if (Frog_Sprint.Succeeded())
	{
		PropCheatAction = Frog_PropCheat.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Emotion
		(TEXT("/Game/Characters/Input/IA_FrogEmotion.IA_FrogEmotion"));
	if (Frog_Emotion.Succeeded())
	{
		EmotionAction = Frog_Emotion.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Setting
		(TEXT("/Game/Characters/Input/IA_FrogSetting.IA_FrogSetting"));
	if (Frog_Setting.Succeeded())
	{
		SettingAction = Frog_Setting.Object;
	}


	ConstructorHelpers::FObjectFinder<USoundBase> JumpSoundObject
		(TEXT("/Game/Sounds/Ques/Jump_Cue.Jump_Cue"));
	if (JumpSoundObject.Succeeded())
	{
		JumpSound = JumpSoundObject.Object;
	}

	ConstructorHelpers::FClassFinder<UJumpGaugeUI> JumpGaugeUIWidget
		(TEXT("/Game/UI/Character/WBP_JumpGauge.WBP_JumpGauge_C"));
	if (JumpGaugeUIWidget.Succeeded())
	{
		JumpGaugeUIClass = JumpGaugeUIWidget.Class;
		JumpGaugeUIComponent = CreateDefaultSubobject<UWidgetComponent>(
			TEXT("JumpGaugeUIComponent"));
		JumpGaugeUIComponent->SetupAttachment(GetRootComponent());
		JumpGaugeUIComponent->SetRelativeLocation(FVector(0, 0.f, 0));
		JumpGaugeUIComponent->SetWidgetClass(JumpGaugeUIClass);
		JumpGaugeUIComponent->SetWidgetSpace(EWidgetSpace::Screen);
		JumpGaugeUIComponent->SetDrawSize(FVector2D(180, 150));
		JumpGaugeUIComponent->SetPivot(FVector2D(3.0, 0.3));
		JumpGaugeUIComponent->SetDrawAtDesiredSize(true);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> WaterPostProcessFinder
		(TEXT("/Game/PostProcess/MPP_InWater.MPP_InWater"));
	if (WaterPostProcessFinder.Succeeded())
	{
		WaterPostProcessMaterial = WaterPostProcessFinder.Object;
		WaterPostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(
			TEXT("WaterPostProcessComponent"));
		//WaterPostProcessComponent->Settings.AddBlendable(WaterPostProcessMaterial, 0.5);
		WaterPostProcessComponent->SetupAttachment(GetRootComponent());
		WaterPostProcessComponent->bEnabled = false;
	}

	SettingPostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("SettingPostProcessComponent"));
	SettingPostProcessComponent->SetupAttachment(GetRootComponent());

	FrogTongueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrogTongueMesh"));
	FrogTongueMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FrogTongueMesh->SetupAttachment(GetMesh(), FName(TEXT("TongueSocket")));
	FrogTongueMesh->SetRelativeScale3D(FVector(1.f, 0.f, 1.f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TongueMesh
		(TEXT("/Game/Characters/Tongue/sm_tongue2.sm_tongue2"));
	if (TongueMesh.Succeeded())
	{
		FrogTongueMesh->SetStaticMesh(TongueMesh.Object);
	}

	TongueCollision = CreateDefaultSubobject<USphereComponent>(TEXT("TongueCollision"));
	TongueCollision->SetupAttachment(GetMesh(), FName("TongueSocket"));
	TongueCollision->SetSphereRadius(10.f);
	TongueCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//TongueCollision->SetCollisionProfileName(TEXT("TongueCollision"));
	//TongueCollision->ComponentTags.Add(TEXT("TongueCollision"));

	TongueTipComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("TongueTipComponent"));
	TongueTipComponent->SetupAttachment(FrogTongueMesh, FName("TongueTipSocket"));
	TongueTipComponent->SetRelativeRotation(FRotator(0, 90.f, 0));

	// CapsuleComponent Settings
	GetCapsuleComponent()->InitCapsuleSize(43.f, 70.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// CharacterMovement Settings
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxAcceleration = 1'200.f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.f;
	GetCharacterMovement()->SetCrouchedHalfHeight(60.f);
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->GroundFriction = 5.f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->MaxStepHeight = 65.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 150.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1500.f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->PerchRadiusThreshold = 20.f;
	GetCharacterMovement()->PerchRadiusThreshold = 20.f;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 800.f, 0.f);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 150.f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = false;
	GetCharacterMovement()->FallingLateralFriction = 5.f;
	GetCharacterMovement()->GravityScale = 2.7;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CameraCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraCollision"));
	CameraCollision->SetupAttachment(FollowCamera);
	CameraCollision->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	CameraCollision->SetRelativeLocation(FVector(0, 0, -50.f));
	CameraCollision->SetCollisionProfileName(TEXT("CameraCollision"));
	CameraCollision->ComponentTags.Add(TEXT("CameraCollision"));

	// MotionMatching
	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(
		TEXT("TrajectoryComponent"));
	TrajectoryComponent->SetIsReplicated(true);

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLightComponent->SetupAttachment(CameraBoom);
	SpotLightComponent->SetIntensityUnits(ELightUnits::Candelas);
	SpotLightComponent->SetIntensity(10.f);
	SpotLightComponent->SetAttenuationRadius(630.f);
	SpotLightComponent->SetOuterConeAngle(29.f);
	SpotLightComponent->SetCastShadows(false);
	SpotLightComponent->SetRelativeRotation(FRotator(-10.f, 0.f, 0));
	SpotLightComponent->SetIsReplicated(true);

	GetCharacterMovement()->SetIsReplicated(true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("FrogCollision"));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_Yes;

	GetCapsuleComponent()->ComponentTags.Add(TEXT("FrogCapsule"));
	Tags.Add(TEXT("Frog"));

	// 동기화 좀 더 빨라지게
	GetCharacterMovement()->bNetworkSkipProxyPredictionOnNetUpdate = false;
	GetCharacterMovement()->bNetworkSmoothingComplete = false;
	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	GetCharacterMovement()->NetworkSimulatedSmoothLocationTime = 0.01f;
	GetCharacterMovement()->NetworkSimulatedSmoothRotationTime = 0.01f;
	SetNetUpdateFrequency(10'000);

	// 물 관련 상태
	bIsSwimming = false;
	CharacterWaterState = ECharacterStateEnum::None;
	TimeSpentInWater = 0.f;
	SurfaceStateForceTime = 5.f;
	bWaterStateForcedByTime = false;

	FrogSkinFinder();

	// 감정표현
	ConstructorHelpers::FClassFinder<UUserWidget> EmotionUIWidgetClass
		(TEXT("/Game/UI/Character/WBP_Emotion.WBP_Emotion_C"));
	if (EmotionUIWidgetClass.Succeeded())
	{
		EmotionUIClass = EmotionUIWidgetClass.Class;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempGreet(
		TEXT("/Game/Characters/Animation/EmotionMontage/WavingHands_Montage.WavingHands_Montage"));
	if (TempGreet.Succeeded())
	{
		GreetingMontage = TempGreet.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempAngry(
		TEXT("/Game/Characters/Animation/EmotionMontage/Angry_Montage.Angry_Montage"));
	if (TempAngry.Succeeded())
	{
		AngryMontage = TempAngry.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempSad(
		TEXT("/Game/Characters/Animation/EmotionMontage/Sad_Montage.Sad_Montage"));
	if (TempSad.Succeeded())
	{
		SadMontage = TempSad.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempMerong(
		TEXT("/Game/Characters/Animation/EmotionMontage/Merong_Montage.Merong_Montage"));
	if (TempMerong.Succeeded())
	{
		MerongMontage = TempMerong.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempWinner(
		TEXT("/Game/Characters/Animation/EmotionMontage/EndingDance_Montage.EndingDance_Montage"));
	if (TempWinner.Succeeded())
	{
		WinnerMontage = TempWinner.Object;
	}

	// 설정
	ConstructorHelpers::FClassFinder<UUserWidget> TempGameSettingUIClass
		(TEXT("/Game/UI/Game/WBP_Setting.WBP_Setting_C"));
	if (TempGameSettingUIClass.Succeeded())
	{
		GameSettingUIClass = TempGameSettingUIClass.Class;
	}
}

void AFrog::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController{Cast<APlayerController>(Controller)})
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem{
			ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())
		})
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called when the game starts or when spawned
void AFrog::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && IsLocallyControlled() && GetWorld()->GetMapName().Contains("InGame"))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("This Character: %p"), this);
	}

	if (WaterPostProcessComponent && WaterPostProcessMaterial)
	{
		WaterPostProcessDynamicMaterial = UMaterialInstanceDynamic::Create(
			WaterPostProcessMaterial, this);

		WaterPostProcessComponent->Settings.AddBlendable(WaterPostProcessDynamicMaterial, 1.f);
		//WaterPostProcessComponent->Settings.Blendables.Add(WaterPostProcessDynamicMaterial);
	}
	
	// 로컬 플레이어만 카메라 오버랩 이벤트 바인딩
	if (IsLocallyControlled() && CameraCollision != nullptr)
	{
		CameraCollision->OnComponentBeginOverlap.AddDynamic(this, &AFrog::OnCameraBeginOverlapWater);
		CameraCollision->OnComponentEndOverlap.AddDynamic(this, &AFrog::OnCameraEndOverlapWater);
	}

	if (TongueCollision != nullptr && HasAuthority())
	{
		TongueCollision->OnComponentBeginOverlap.AddDynamic(this, &AFrog::OnTongueBeginOverlap);
	}

	// 감정표현 UI
	EmotionUI = CreateWidget<class UEmotionUI>(GetWorld(), EmotionUIClass);
	if (EmotionUI && IsLocallyControlled())
	{
		EmotionUI->AddToViewport();
	}

	// 설정 UI
	GameSettingUI = CreateWidget<class UGameSettingUI>(GetWorld(), GameSettingUIClass);
	if (GameSettingUI && IsLocallyControlled())
	{
		GameSettingUI->AddToViewport();
		GameSettingUI->SetVisibility(ESlateVisibility::Collapsed);
	}

	InitJumpGaugeUIComponent();
	InitFrogState();
}

void AFrog::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorldTimerManager().ClearTimer(TongueTimer);
		GetWorldTimerManager().ClearTimer(CrouchTimer);
		GetWorldTimerManager().ClearTimer(LaunchCooldownTimer);
		GetWorldTimerManager().ClearTimer(DoubleTapTimer);
		GetWorldTimerManager().ClearTimer(JumpBackHandle);
		GetWorldTimerManager().ClearTimer(ReturnCollisionTimer);
	}

	Super::EndPlay(EndPlayReason);
}

void AFrog::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitJumpGaugeUIComponent();
}

// Called every frame
void AFrog::Tick(float DeltaTime)
{
	if (bIsLobbyFrog)
	{
		return;
	}

	Super::Tick(DeltaTime);

	//FLog::Log("", GetCharacterMovement()->JumpZVelocity);

	// 점프 버퍼
	if (bJumpBuffered)
	{
		JumpBufferTimeFlow -= DeltaTime;
		if (CanJump())
		{
			Jump();
			bJumpBuffered = false;
		}
		else if (JumpBufferTimeFlow <= 0.f)
		{
			bJumpBuffered = false;
		}
	}

	// 코요테 타임
	if (bCoyoteActive)
	{
		CoyoteTimeFlow -= DeltaTime;
		if (CoyoteTimeFlow <= 0.f)
		{
			bCoyoteActive = false;
		}
	}

	// 공중에 있을 때는 회전 잘 안되게
	if (GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	}
	else
	{
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);
	}

	PrevVelocityZLength = GetVelocity().Z * -1;

	// Pitch, Yaw 계산
	if (HasAuthority())
	{
		// GetBaseAimRotation()는 서버 카메라의 회전값 사용하므로 클라와 서버의 카메라 방향이 다르면 문제가 생김
		// --> 상대값으로 해야함 ( meshRotation - baseRotation )
		const FRotator MeshRotation = GetMesh()->GetComponentRotation();
		const FRotator AimRotation = GetBaseAimRotation();
		// Pitch: 메시 기준 상대값 계산
		Pitch = (AimRotation - MeshRotation).GetNormalized().Pitch;
		// 상대 Yaw
		const float CharacterYaw{static_cast<float>(GetActorRotation().Yaw)};
		// 절대 Yaw
		const float AimYaw{static_cast<float>(AimRotation.Yaw)};
		// AimYaw - CharacterYaw
		const float RelativeYaw{FMath::FindDeltaAngleDegrees(CharacterYaw, AimYaw)};
		//카메라로 캐릭터 정면 볼 때 고려
		if (RelativeYaw >= 90.f)
		{
			Yaw = 90.f - (RelativeYaw - 90.f);
		}
		else if (RelativeYaw <= -90.f)
		{
			Yaw = -90 - (RelativeYaw + 90.f);
		}
		else
		{
			Yaw = RelativeYaw;
		}
	}
	// 서버에서만 물 관련 로직 처리
	if (HasAuthority())
	{
		HandleInWaterLogic(DeltaTime);
	}

	if (IsLocallyControlled() && OverlapWaterComponent.IsValid())
	{
		// 정렬 시키고 검사
		CameraCollision->SetWorldRotation(FRotator::ZeroRotator);
		CalculateWaterCameraOverlapRatio(DeltaTime);
	}

	GetCharacterMovement()->GravityScale = FrogGravity;
	JumpCurrentCount = FrogJumpCount;
	//GetCharacterMovement()->MovementMode = FrogMovementMode;
}

// Called to bind functionality to input
void AFrog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent{
		Cast<UEnhancedInputComponent>(
			PlayerInputComponent)
	})
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
		                                   &AFrog::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &AFrog::StopJumping);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this,
		                                   &AFrog::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this,
		                                   &AFrog::StopCrouch);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this,
		                                   &AFrog::WPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this,
		                                   &AFrog::WReleased);

		EnhancedInputComponent->BindAction(TongueAttackAction, ETriggerEvent::Started, this,
		                                   &AFrog::TongueAttack);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFrog::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFrog::Look);

		EnhancedInputComponent->BindAction(PropActiveAction, ETriggerEvent::Started, this, &AFrog::PropActive);
		EnhancedInputComponent->BindAction(PropCheatAction, ETriggerEvent::Started, this, &AFrog::PropCheat);
		EnhancedInputComponent->BindAction(DebugModeAction, ETriggerEvent::Started, this, &AFrog::DebugMode);

		EnhancedInputComponent->BindAction(EmotionAction, ETriggerEvent::Started, this, &AFrog::OnPressCKey);
		EnhancedInputComponent->BindAction(EmotionAction, ETriggerEvent::Completed, this, &AFrog::OnReleasedCKey);

		EnhancedInputComponent->BindAction(SettingAction, ETriggerEvent::Started, this, &AFrog::OnPressESCKey);
	}
}


void AFrog::Move(const struct FInputActionValue& Value)
{
	FVector2D MovementVector{Value.Get<FVector2D>()};

	if (Controller && GetCanMove())
	{
		CancelEmotion();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFrog::Look(const struct FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller && GetCanMove())
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// 웅크리면서 점프 가능하게
bool AFrog::CanJumpInternal_Implementation() const
{
	// 원래 점프 로직
	bool bCanJump{Super::CanJumpInternal_Implementation()};

	// 앉은 상태라면 ?
	if (!bCanJump && GetCharacterMovement()->IsCrouching())
	{
		// 현재 지면에 서 있는지
		// 더 점프할 수 있는지
		// 공중이 아닌지
		bCanJump = GetCharacterMovement()->IsMovingOnGround()
			&& JumpCurrentCount < JumpMaxCount
			&& !GetCharacterMovement()->IsFalling();
	}

	bCanJump |= bCoyoteActive;
	//FLog::Log("", JumpCurrentCount, JumpMaxCount);

	return bCanJump;
}

void AFrog::Falling()
{
	Super::Falling();
	// JumpCurrentCount
	//FLog::Log("fall");
	if (!bIsJumping)
	{
		bCoyoteActive = true;
		CoyoteTimeFlow = CoyoteTime;
	}
}

void AFrog::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	bCoyoteActive = false;
}

void AFrog::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bIsJumping = false;
	bCoyoteActive = false;

	if (HasAuthority())
	{
		FrogJumpCount = 0;

		ResetSuperJumpRatio();

		if (bIsPressedSprint)
		{
			SetSprintSpeed();
		}
		else
		{
			SetWalkSpeed();
		}

		if (bIsPressedCrouch)
		{
			FrogMovementMode = MOVE_Walking;
			GetCharacterMovement()->MovementMode = MOVE_Walking;

			MulticastRPC_StartCrouch();
		}

		MulticastRPC_Landed();
	}
	else
	{
		ServerRPC_Landed();
	}
}

void AFrog::StartJump()
{
	if (!GetCanMove())
	{
		return;
	}

	// if (!AFrog::CanJumpInternal_Implementation())
	// {
	// 	// 점프 불가 -> 버퍼 시작
	// 	bJumpBuffered = true;
	// 	JumpBufferTimeFlow = JumpBufferTime;
	//
	// 	return;
	// }

	bIsJumping = true;

	if (CharacterWaterState == ECharacterStateEnum::Surface)
	{
		FVector LaunchVelocity{GetActorForwardVector() * 100.f + FVector::UpVector * 1'700.f};

		// 클라이언트 예측 실행
		if (IsLocallyControlled())
		{
			CancelEmotion();

			ACharacter::LaunchCharacter(LaunchVelocity, true, true);
			ActivateRecentlyLaunchedFlag();
			UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation(), 1, 1, 4.39f);

			// 서버에 실제 점프 실행
			ServerRPC_ExecuteWaterSurfaceJump(LaunchVelocity);
			ForceNetUpdate();
		}
	}
	else if (!AFrog::CanJumpInternal_Implementation())
	{
		// 점프 불가 -> 버퍼 시작
		bJumpBuffered = true;
		JumpBufferTimeFlow = JumpBufferTime;
	}
	// 슈퍼 점프
	else if (GetCharacterMovement()->IsCrouching())
	{
		if (bIsSuperJump)
		{
			SetJumpAvailableBlock(3);
			bIsSuperJump = false;
		}
		else if (SuperJumpRatio >= 0.5f)
		{
			SetJumpAvailableBlock(2);
		}

		StopCrouch();

		if (CanJump())
		{
			CancelEmotion();
			Jump();
			bJumpBuffered = false;
			MulticastRPC_PlayEffect(GetActorLocation(), 0);
		}
	}
	// 일반 점프
	else
	{
		if (CanJump())
		{
			CancelEmotion();
			Jump();
			bJumpBuffered = false;
			MulticastRPC_PlayEffect(GetActorLocation(), 1);
		}
	}
}

void AFrog::MulticastRPC_PlayEffect_Implementation(FVector Location, int32 Index)
{
	//FLog::Log();
	switch (Index)
	{
	case 0:
		PlayHitEffect();
		UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation(), 1.5f, 1, 4.39f);
		break;
	case 1:
		UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation(), 1, 1, 4.39f);
		break;
	default:
		break;
	}
}

void AFrog::StopJump()
{
	StopJumping();
}

void AFrog::WPressed(const struct FInputActionValue& Value)
{
	float CurrentTime{static_cast<float>(GetWorld()->GetTimeSeconds())};

	if (CurrentTime - WPressedTime < 0.25f)
	{
		bIsSprint = true;
		StartSprint();
	}

	WPressedTime = CurrentTime;
}

void AFrog::WReleased(const struct FInputActionValue& Value)
{
	if (bIsSprint)
	{
		bIsSprint = false;
		StopSprint();
	}
}

void AFrog::StartSprint()
{
	bIsPressedSprint = true;

	if (!GetCanMove())
	{
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	SetSprintSpeed();
}

void AFrog::StopSprint()
{
	bIsPressedSprint = false;

	SetWalkSpeed();
}

void AFrog::SetCrouchEnabled(bool bEnabled)
{
	bCanCrouch = bEnabled;

	// if (JumpGaugeUIComponent != nullptr)
	// {
	// 	SetJumpGaugeVisibility(bCanCrouch);
	// }
}

void AFrog::StartCrouch()
{
	bIsPressedCrouch = true;

	if (!GetCanMove())
	{
		return;
	}

	if (HasAuthority())
	{
		MulticastRPC_StartCrouch();
	}
	else
	{
		ServerRPC_StartCrouch();
	}
}

// Todo: 클라 점프 중 웅크리기, 서버에 안보임
void AFrog::StopCrouch()
{
	bIsPressedCrouch = false;

	MulticastRPC_StopCrouch();
	if (HasAuthority())
	{
		MulticastRPC_StopCrouch();
	}
	else
	{
		ServerRPC_StopCrouch();
	}
}

void AFrog::TongueAttack()
{
	if (HasAuthority())
	{
		ServerRPC_StartTongueAttack_Implementation();
	}
	else
	{
		ServerRPC_StartTongueAttack();
	}
}

void AFrog::TongueAttackEnd()
{
	GetWorldTimerManager().ClearTimer(TongueTimer);

	TongueLengthRatio = 0.f;
	bIsTongueGrow = false;

	OnRep_TongueLengthRatio();
	OnRep_IsTongueGrow();

	// 혹시 모르니까 다시 설정
	TongueCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 혓바닥 공격 쿨타임
	TWeakObjectPtr<AFrog> WeakThis{this};
	FTimerDelegate TongueAttackCoolTimeDelegate{
		FTimerDelegate::CreateLambda([WeakThis]() {
			if (!WeakThis.IsValid())
			{
				return;
			}

			AFrog* StrongThis = WeakThis.Get();
			StrongThis->bCanTongAttack = true;
			StrongThis->OnRep_CanTongAttack();
		})
	};

	GetWorldTimerManager().SetTimer(TongueTimer, TongueAttackCoolTimeDelegate, 1.f, false);
}

void AFrog::DebugMode()
{
	//
}

void AFrog::PropActive()
{
	//
}

void AFrog::PropCheat()
{
	//
}

void AFrog::ServerRPC_Launch_Implementation(const FVector& LaunchVelocity)
{
	if (HasAuthority())
	{
		GetCharacterMovement()->Launch(LaunchVelocity);
	}

	MulticastRPC_Launch(LaunchVelocity);
	ForceNetUpdate();
}

void AFrog::MulticastRPC_Launch_Implementation(const FVector& LaunchVelocity)
{
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		GetCharacterMovement()->Launch(LaunchVelocity);
		ActivateRecentlyLaunchedFlag();
	}
}

void AFrog::ServerRPC_ExecuteWaterSurfaceJump_Implementation(const FVector& LaunchVelocity)
{
	if (CharacterWaterState == ECharacterStateEnum::Surface)
	{
		UCapsuleComponent* CapComp{GetCapsuleComponent()};
		if (CapComp)
		{
			CapComp->SetCollisionProfileName(TEXT("Jumping"));
		}

		// 서버에서 캐릭터 발사
		if (HasAuthority())
		{
			GetCharacterMovement()->Launch(LaunchVelocity);
			ActivateRecentlyLaunchedFlag();
		}

		MulticastRPC_Launch(LaunchVelocity);

		// 1초 후 서버에서 충돌 프로필 되돌리기
		TWeakObjectPtr<AFrog> WeakThis{this};
		FTimerDelegate RestoreCollisionDelegate{
			FTimerDelegate::CreateLambda([WeakThis]() {
				if (!WeakThis.IsValid())
				{
					return;
				}

				AFrog* StrongThis = WeakThis.Get();
				UCapsuleComponent* MyCapComp{StrongThis->GetCapsuleComponent()};
				if (MyCapComp)
				{
					MyCapComp->SetCollisionProfileName(TEXT("FrogCollision"));
				}
			})
		};

		GetWorldTimerManager().SetTimer(ReturnCollisionTimer, RestoreCollisionDelegate, 1.f, false);
	}
}

void AFrog::ServerRPC_StartCrouch_Implementation()
{
	MulticastRPC_StartCrouch();
}

void AFrog::ServerRPC_StopCrouch_Implementation()
{
	MulticastRPC_StopCrouch();
}

void AFrog::MulticastRPC_StartCrouch_Implementation()
{
	if (!bCanCrouch)
	{
		return;
	}

	// 공중에 있거나 수영 중이면 리턴
	if (GetCharacterMovement()->IsFalling() || bIsSwimming)
	{
		return;
	}

	bIsCrouching = true;
	SetJumpGaugeVisibility(true);
	Crouch();

	// 슈퍼 점프 게이지 충전
	TWeakObjectPtr<AFrog> WeakThis{this};
	FTimerDelegate CrouchDelegate{
		FTimerDelegate::CreateLambda([WeakThis]() {
			if (!WeakThis.IsValid())
			{
				return;
			}

			AFrog* StrongThis = WeakThis.Get();
			StrongThis->CrouchTime += StrongThis->GetWorld()->GetDeltaSeconds();

			if (StrongThis->CrouchTime >= StrongThis->SuperJumpValue)
			{
				StrongThis->bIsSuperJump = true;
			}

			StrongThis->SuperJumpRatio = FMath::Clamp(StrongThis->CrouchTime / StrongThis->SuperJumpValue, 0.f, 1.f);
			StrongThis->OnSuperJumpRatioChanged.Broadcast(StrongThis->SuperJumpRatio);

			//FLog::Log("Ratio", SuperJumpRatio);
		})
	};
	GetWorldTimerManager().SetTimer(CrouchTimer, CrouchDelegate, GetWorld()->GetDeltaSeconds(),
	                                true);
}

void AFrog::MulticastRPC_StopCrouch_Implementation()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	bIsCrouching = false;
	SetJumpGaugeVisibility(false);
	UnCrouch();

	GetWorldTimerManager().ClearTimer(CrouchTimer);
	CrouchTime = 0.f;

	ResetSuperJumpRatio();
}

void AFrog::ServerRPC_StartSprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AFrog::ServerRPC_StopSprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AFrog::ServerRPC_StartTongueAttack_Implementation()
{
	if (!bCanTongAttack)
	{
		return;
	}

	bCanTongAttack = false;
	bIsTongueGrow = true;
	TongueLengthRatio = 0.f;

	// 서버에서 OnRep 함수를 직접 호출하여 서버 화면에서도 즉시 상태 변경이 반영되도록 함
	OnRep_CanTongAttack();
	OnRep_IsTongueGrow();
	OnRep_TongueLengthRatio();

	TongueCollision->SetCollisionProfileName(FName("FrogTongue"));
	TongueCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TWeakObjectPtr<AFrog> WeakThis{this};
	FTimerDelegate TongueAttackDelegate{
		FTimerDelegate::CreateLambda([WeakThis]() {
			if (!WeakThis.IsValid())
			{
				return;
			}

			AFrog* StrongThis = WeakThis.Get();
			if (StrongThis->bIsTongueGrow)
			{
				StrongThis->TongueLengthRatio += StrongThis->GetWorld()->GetDeltaSeconds() * 8.f;

				if (StrongThis->TongueLengthRatio > 1.f)
				{
					StrongThis->TongueLengthRatio = 1.f;
					StrongThis->bIsTongueGrow = false;
					StrongThis->TongueCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

					StrongThis->OnRep_IsTongueGrow();
				}
			}
			else
			{
				StrongThis->TongueLengthRatio -= StrongThis->GetWorld()->GetDeltaSeconds() * 8.f;

				if (StrongThis->TongueLengthRatio < 0.f)
				{
					StrongThis->TongueLengthRatio = 0.f;
					StrongThis->TongueAttackEnd();
					return;
				}
			}

			StrongThis->OnRep_TongueLengthRatio();
		})
	};

	GetWorldTimerManager().SetTimer(TongueTimer, TongueAttackDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void AFrog::SetSprintSpeed()
{
	if (HasAuthority())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		if (IsLocallyControlled())
		{
			ServerRPC_StartSprint();
		}
	}
}

void AFrog::SetWalkSpeed()
{
	if (HasAuthority())
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		if (IsLocallyControlled())
		{
			ServerRPC_StopSprint();
		}
	}
}

void AFrog::ServerRPC_Landed_Implementation()
{
	MulticastRPC_Landed();
}

void AFrog::MulticastRPC_Landed_Implementation()
{
	FrogJumpCount = 0;

	ResetSuperJumpRatio();

	if (bIsPressedSprint)
	{
		SetSprintSpeed();
	}
	else
	{
		SetWalkSpeed();
	}

	if (bIsPressedCrouch)
	{
		FrogMovementMode = MOVE_Walking;
		GetCharacterMovement()->MovementMode = MOVE_Walking;

		MulticastRPC_StartCrouch();
	}
}

void AFrog::SetLightIntensity(float Alpha)
{
	SpotLightComponent->SetIntensity(10.f * (1 - Alpha));

	if (IsLocallyControlled())
	{
		ServerRPC_SetLight(Alpha);
	}
}

void AFrog::ServerRPC_SetLight_Implementation(float Alpha)
{
	MulticastRPC_SetLight(Alpha);
}

void AFrog::MulticastRPC_SetLight_Implementation(float Alpha)
{
	SpotLightComponent->SetIntensity(10.f * (1 - Alpha));
}

void AFrog::InitJumpGaugeUIComponent()
{
	if (bMapEditingPawn)
	{
		if (SettingPostProcessComponent)
		{
			SettingPostProcessComponent->DestroyComponent();
			SettingPostProcessComponent = nullptr;
		}
	}

	// 로컬 클라만 점프 게이지 보이게
	if (IsLocallyControlled() || bMapEditingPawn)
	{
		SetJumpGaugeVisibility(false);
	}
	else
	{
		// 다른 클라에서 삭제
		if (JumpGaugeUIComponent)
		{
			JumpGaugeUIComponent->DestroyComponent();
			JumpGaugeUIComponent = nullptr;
		}
		if (SettingPostProcessComponent)
		{
			SettingPostProcessComponent->DestroyComponent();
			SettingPostProcessComponent = nullptr;
		}
	}
}

void AFrog::InitFrogState()
{
	//	SetJumpAvailableBlock(1);
	SetJumpAvailableBlock(2);

	ResetSuperJumpRatio();

	if (HasAuthority())
	{
		FrogGravity = 2.7f;
		FrogMovementMode = EMovementMode::MOVE_Walking;
	}
}

void AFrog::SetJumpAvailableBlock(int32 Block)
{
	float Height{Block * 100.f + 10.f};
	float Value{FMath::Sqrt(Height * FMath::Abs(GetCharacterMovement()->GetGravityZ()) * 2)};
	GetCharacterMovement()->JumpZVelocity = Value;

	//FLog::Log("Block", Block);

	// 점프력 높게 설정하면 다시 원래대로 점프력 돌아오게
	if (Block != 1)
	{
		TWeakObjectPtr<AFrog> WeakThis{this};
		FTimerDelegate JumpDelegate{
			FTimerDelegate::CreateLambda([WeakThis]() {
				if (!WeakThis.IsValid())
				{
					return;
				}
				AFrog* StrongThis = WeakThis.Get();
				StrongThis->SetJumpAvailableBlock(1);
			})
		};
		GetWorldTimerManager().SetTimer(JumpBackHandle, JumpDelegate, 0.2f, false);
	}

	if (IsLocallyControlled())
	{
		ServerRPC_SetJumpAvailableBlock(Block);
	}
}

void AFrog::ResetSuperJumpRatio()
{
	SuperJumpRatio = 0.f;
	OnSuperJumpRatioChanged.Broadcast(0.f);
}

void AFrog::StopMovementAndResetRotation(const FRotator& Rot)
{
	bCanMove = false;
	SetActorRotation(Rot);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	// FLog::Log("ServerLoc", GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	// FLog::Log("ServerRoc", GetActorRotation().Yaw, GetActorRotation().Pitch);
}

void AFrog::ResumeMovement()
{
	bCanMove = true;
}

bool AFrog::GetCanMove() const
{
	return bCanMove;
}

void AFrog::CameraMissionMode()
{
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetRelativeLocationAndRotation(FVector(0, 0, 150.f), FRotator(-20.f, 0, 0));
	CameraBoom->TargetArmLength = 1'000.f;
}

void AFrog::CameraMovementMode()
{
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	CameraBoom->TargetArmLength = 400.f;
}

void AFrog::ServerRPC_PrepareMission_Implementation(const FVector& Loc, const FRotator& Rot)
{
	if (HasAuthority())
	{
		SetActorLocation(Loc);
		StopMovementAndResetRotation(Rot);
		SetCrouchEnabled(false);
		SetJumpGaugeVisibility(false);
		MulticastRPC_SetMissionCamera();
	}
}

void AFrog::MulticastRPC_SetMissionCamera_Implementation()
{
	CameraMissionMode();
}

void AFrog::ServerRPC_FinishMission_Implementation()
{
	if (HasAuthority())
	{
		ResumeMovement();
		SetCrouchEnabled(true);

		MulticastRPC_SetMovementCamera();
	}
}

void AFrog::MulticastRPC_SetMovementCamera_Implementation()
{
	CameraMovementMode();
}

void AFrog::SetJumpGaugeVisibility(bool bVisibility)
{
	if (JumpGaugeUIComponent != nullptr)
	{
		JumpGaugeUIComponent->SetVisibility(bVisibility);
	}
}

void AFrog::OnRep_SuperJumpRatio()
{
	if (IsLocallyControlled())
	{
		SuperJumpRatio = FMath::Clamp(CrouchTime / SuperJumpValue, 0.f, 1.f);
		OnSuperJumpRatioChanged.Broadcast(SuperJumpRatio);
	}
}

void AFrog::ServerRPC_SetJumpAvailableBlock_Implementation(int32 Block)
{
	float Height{Block * 100.f + 10.f};
	float Value{FMath::Sqrt(Height * FMath::Abs(GetCharacterMovement()->GetGravityZ()) * 2)};
	GetCharacterMovement()->JumpZVelocity = Value;

	// 점프력 높게 설정하면 다시 원래대로 점프력 돌아오게
	if (Block != 1)
	{
		TWeakObjectPtr<AFrog> WeakThis{this};
		FTimerDelegate JumpDelegate{
			FTimerDelegate::CreateLambda([WeakThis]() {
				if (!WeakThis.IsValid())
				{
					return;
				}

				AFrog* StrongThis = WeakThis.Get();
				StrongThis->SetJumpAvailableBlock(1);
			})
		};
		GetWorldTimerManager().SetTimer(JumpBackHandle, JumpDelegate, 0.2f, false);
	}
}

void AFrog::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFrog, Pitch);
	DOREPLIFETIME(AFrog, Yaw);
	DOREPLIFETIME(AFrog, bIsCrouching);
	DOREPLIFETIME(AFrog, bIsSuperJump);
	DOREPLIFETIME(AFrog, CrouchTime);
	DOREPLIFETIME(AFrog, TimeSpentInWater);
	DOREPLIFETIME(AFrog, bWaterStateForcedByTime);
	DOREPLIFETIME(AFrog, bCanMove);
	DOREPLIFETIME(AFrog, bCanCrouch);
	// COND_SkipOwner : 소유자 클라이언트는 업데이트 안해서 중복 실행 방지
	DOREPLIFETIME_CONDITION(AFrog, SuperJumpRatio, COND_SkipOwner);
	// 항상 복제
	DOREPLIFETIME_CONDITION(AFrog, CharacterWaterState, COND_None);
	DOREPLIFETIME_CONDITION(AFrog, bIsSwimming, COND_None);

	DOREPLIFETIME(AFrog, TongueLengthRatio);
	DOREPLIFETIME(AFrog, bIsTongueGrow);
	DOREPLIFETIME(AFrog, bCanTongAttack);

	DOREPLIFETIME(AFrog, SkinIndex);

	DOREPLIFETIME(AFrog, bRecentlyLaunched);

	DOREPLIFETIME(AFrog, FrogGravity);
	DOREPLIFETIME(AFrog, bIsPressedSprint);
	DOREPLIFETIME(AFrog, bIsPressedCrouch);
	DOREPLIFETIME(AFrog, FrogMovementMode);
	DOREPLIFETIME(AFrog, FrogJumpCount);
}

void AFrog::ServerRPC_UpdateOverallWaterState_Implementation(bool bNowInWater, class ARisingWaterProp* WaterVolume)
{
	if (bIsSwimming == bNowInWater && CurrentWaterVolume == WaterVolume)
	{
		// 이미 같은 상태면 무시
		return;
	}

	bIsSwimming = bNowInWater;

	// 서버에서도 OnRep 함수 호출해서 로직 일관성 유지 또는 필요한 초기화 수행
	OnRep_bIsSwimming();

	if (bIsSwimming)
	{
		bCanPlayEmotion = false;
		CancelEmotion();

		// 현재 상호작용 중인 물 저장
		CurrentWaterVolume = WaterVolume;
		TimeSpentInWater = 0.f;
		bWaterStateForcedByTime = false;
		SetJumpGaugeVisibility(false);

		ResetSuperJumpRatio();
	}
	else
	{
		bCanPlayEmotion = true;
		CancelEmotion();

		// 물에서 벗어나면 null로 설정
		CurrentWaterVolume = nullptr;
		CharacterWaterState = ECharacterStateEnum::None;
		OnRep_CharacterWaterState();
		ResetSuperJumpRatio();
	}
}

bool AFrog::ServerRPC_UpdateOverallWaterState_Validate(bool bNowInWater, class ARisingWaterProp* WaterVolume)
{
	return true;
}

void AFrog::ServerRPC_SetSpecificWaterState_Implementation(ECharacterStateEnum NewState)
{
	// 물 속에 있을 때만 변경
	if (!bIsSwimming && NewState != ECharacterStateEnum::None)
	{
		return;
	}

	if (CharacterWaterState == NewState)
	{
		// 이미 같은 상태면 무시
		return;
	}

	CharacterWaterState = NewState;
	OnRep_CharacterWaterState();
}

bool AFrog::ServerRPC_SetSpecificWaterState_Validate(ECharacterStateEnum NewState)
{
	return true;
}

void AFrog::ServerRPC_TeleportToLocation_Implementation(FVector TargetLocation)
{
	SetActorLocation(TargetLocation);
	// 텔레포트 후 상태 초기화
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetCharacterMovement()->StopMovementImmediately();
}


bool AFrog::ServerRPC_TeleportToLocation_Validate(FVector TargetLocation)
{
	return true;
}

void AFrog::OnRep_CharacterWaterState()
{
	// 추후 개발 가능
	// CharacterWaterState가 서버에서 변경되어 클라이언트로 복제 완료 시 호출
	// 클라이언트에서 상태에 따른 시각적 효과, 애니메이션 변경 등을 처리
}

void AFrog::OnRep_bIsSwimming()
{
	if (IsLocallyControlled())
	{
		SetCrouchEnabled(!bIsSwimming);
	}

	if (!bIsSwimming)
	{
		if (CharacterWaterState != ECharacterStateEnum::None)
		{
			CharacterWaterState = ECharacterStateEnum::None;
		}
	}
}

void AFrog::HandleInWaterLogic(float DeltaTime)
{
	UCharacterMovementComponent* MoveComp{GetCharacterMovement()};
	// 혹시 모르니 확인
	if (!MoveComp)
	{
		return;
	}

	if (bIsSwimming)
	{
		MoveComp->SetMovementMode(MOVE_Flying);
		//SetCrouchEnabled(false);

		// 일정 시간 물 속에 들어갔는데 Surface 상태가 아니면
		if (!bWaterStateForcedByTime)
		{
			TimeSpentInWater += DeltaTime;

			if (TimeSpentInWater >= SurfaceStateForceTime)
			{
				if (CharacterWaterState != ECharacterStateEnum::Surface)
				{
					ServerRPC_SetSpecificWaterState(ECharacterStateEnum::Surface);
				}
				// 한 번 강제 변경되었음을 표시
				bWaterStateForcedByTime = true;
			}
		}
		// CharacterWaterState에 따른 물리 효과 적용
		switch (CharacterWaterState)
		{
		case ECharacterStateEnum::None:
			if (HasAuthority())
			{
				FrogGravity = 0.5f;
			}

			break;
		case ECharacterStateEnum::Deep:
			if (HasAuthority())
			{
				FrogGravity = 0.f;
			}

			if (MoveComp->Velocity.Z < 800.f)
			{
				MoveComp->Velocity.Z = 800.f;
			}

			break;
		case ECharacterStateEnum::Shallow:
			if (HasAuthority())
			{
				FrogGravity = 0.f;
			}

			if (MoveComp->Velocity.Z < 500.f)
			{
				MoveComp->Velocity.Z = 500.f;
			}

			break;
		case ECharacterStateEnum::Surface:
			if (HasAuthority())
			{
				FrogGravity = 0.05f;
			}

			bool bIsJumpingOrLaunched{MoveComp->IsFalling() || bRecentlyLaunched};

		// 물이 상승중이라면
			if (CurrentWaterVolume.IsValid())
			{
				float WaterSurfaceZ{CurrentWaterVolume->GetWaterSurfaceZ()};
				float WaterSurfaceVelocityZ{0.f};
				if (CurrentWaterVolume->WaterState == EWaterStateEnum::Rise)
				{
					WaterSurfaceVelocityZ = CurrentWaterVolume->CurrentRisingSpeed;
				}

				// 캐릭터가 물 표면에 거의 닿도록 목표 Z 설정
				float TargetCharacterCenterZ{
					WaterSurfaceZ + GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.9f
				};
				// 현재 캐릭터의 Z 위치와 목표 Z 위치 사이의 차이
				float DeltaZ{static_cast<float>(TargetCharacterCenterZ - GetActorLocation().Z)};

				// 점프 중이 아닐 때만
				if (!bIsJumpingOrLaunched)
				{
					// 목표 Z 위치로 향하는 교정 속도 계산
					float CorrectiveVelocityZ{DeltaZ * 1.f};
					// 물 상승 속도를 기본으로 하고, 표면 위치 맞추기 위해 보정 속도 추가
					float DesiredZVelocity{WaterSurfaceVelocityZ + CorrectiveVelocityZ};
					MoveComp->Velocity.Z = FMath::Lerp(MoveComp->Velocity.Z, DesiredZVelocity,
					                                   FMath::Clamp(DeltaTime * 5.0f, 0.f, 1.f));

					// Z 속도 제한
					MoveComp->Velocity.Z = FMath::Clamp(MoveComp->Velocity.Z, -50.f, WaterSurfaceVelocityZ + 100.f);

					// 물이 상승 중이 아닐 때, 캐릭터가 물 표면 근처에 있고 거의 움직이지 않는다면 Z 속도를 0으로
					if (FMath::Abs(DeltaZ) < 2.0f && WaterSurfaceVelocityZ == 0.f && FMath::Abs(MoveComp->Velocity.Z) <
						5.f)
					{
						MoveComp->Velocity.Z = FMath::FInterpTo(MoveComp->Velocity.Z, 0.f, DeltaTime, 5.0f);
					}
				}

				// 수평 움직임
				MoveComp->Velocity.X *= FMath::FInterpTo(1.0f, 0.9f, DeltaTime, 2.0f);
				MoveComp->Velocity.Y *= FMath::FInterpTo(1.0f, 0.9f, DeltaTime, 2.0f);
			}

			break;
		}
	}
	else
	{
		if (HasAuthority())
		{
			// 점프 최대 높이일 때 중력 영향 감소
			// --> 땅에 닿기까지 시간 더 줘서 움직임 더 쉽게 조정
			if (bIsJumping && GetCharacterMovement()->Velocity.Z < 10.f)
			{
				// FLog::Log();
				FrogGravity = 2.1f;
			}
			else
			{
				FrogGravity = 2.7f;
			}
		}

		if (MoveComp->IsFlying() || MoveComp->IsSwimming())
		{
			MoveComp->SetMovementMode(MOVE_Walking);
		}

		// 땅에서는 앉기 다시 활성화
		//SetCrouchEnabled(true);

		// 물 밖에 나오면 초기화
		TimeSpentInWater = 0.f;
		bWaterStateForcedByTime = false;
	}
}

void AFrog::ServerRPC_CallLaunchCharacter_Implementation(const FVector& Dir, float Force, bool bXY, bool bZ)
{
	FVector LaunchVelocity{Dir.GetSafeNormal() * Force};

	if (HasAuthority())
	{
		LaunchCharacter(LaunchVelocity, bXY, bZ);
	}
}

void AFrog::OnCameraBeginOverlapWater(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	if (IsLocallyControlled() && WaterPostProcessComponent != nullptr && OtherActor && OtherActor->
		ActorHasTag(TEXT("Water")) && OtherComp && OtherComp->ComponentHasTag(TEXT("CameraWater")))
	{
		WaterPostProcessComponent->bEnabled = true;

		OverlapWaterComponent = OtherComp;
		//OtherComp->AddToRoot();
	}
}

void AFrog::OnCameraEndOverlapWater(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsLocallyControlled() && WaterPostProcessComponent != nullptr && OtherActor && OtherActor->
		ActorHasTag(TEXT("Water")) && OtherComp && OtherComp->ComponentHasTag(TEXT("CameraWater")))
	{
		WaterPostProcessComponent->bEnabled = false;

		//OtherComp->RemoveFromRoot();
		OverlapWaterComponent = nullptr;
	}
}

void AFrog::OnTongueBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	// 서버에서만 충돌 처리
	if (!HasAuthority())
	{
		return;
	}

	if (OtherComp->ComponentHasTag(FName("Shield")))
	{
		bIsTongueGrow = false;
		return;
	}

	AFrog* OverlappingFrog{Cast<AFrog>(OtherActor)};
	if (OverlappingFrog && OverlappingFrog != this)
	{
		bIsTongueGrow = false;

		// 충돌체 비활성화, 중복 충돌 방지
		//TongueCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TongueCollision->SetCollisionResponseToChannels(ECR_Ignore);

		FVector Dir{TongueTipComponent->GetForwardVector() + FVector::UpVector};
		Dir.Normalize();

		OverlappingFrog->LaunchCharacter(Dir * 300.f, true, true);
	}
}

void AFrog::OnRep_TongueLengthRatio()
{
	SetTongueLength(TongueLengthRatio);

	// if (TongueCollision && TongueTipComponent)
	// {
	// 	TongueCollision->SetWorldTransform(TongueTipComponent->GetComponentTransform());
	// }
}

void AFrog::OnRep_IsTongueGrow()
{
	// bIsTongueGrow 상태 변화에 따른 클라이언트 액션
}

void AFrog::OnRep_CanTongAttack()
{
	// bCanTongAttack 상태 변화에 따른 클라이언트 액션
}

void AFrog::OnRep_GravityChange()
{
	GetCharacterMovement()->GravityScale = FrogGravity;
}

void AFrog::OnRep_JumpCount()
{
	JumpCurrentCount = FrogJumpCount;
}

void AFrog::OnRep_FrogMovement()
{
	GetCharacterMovement()->MovementMode = FrogMovementMode;
}

void AFrog::SetTongueLength(float Value)
{
	Value = FMath::Clamp(Value, 0.f, 1.f);
	FrogTongueMesh->SetRelativeScale3D(FVector(1.f, Value, 1.f));

	if (HasAuthority() && TongueCollision && TongueTipComponent)
	{
		TongueCollision->SetWorldLocationAndRotation(FVector(TongueTipComponent->GetComponentLocation()),
		                                             FRotator(TongueTipComponent->GetComponentRotation()));
	}
}


void AFrog::CalculateWaterCameraOverlapRatio(float dt)
{
	if (!OverlapWaterComponent.IsValid())
		return;

	FBox CameraBox{CameraCollision->Bounds.GetBox()};
	FBox WaterBox{OverlapWaterComponent->Bounds.GetBox()};
	FBox OverlapBox{CameraBox.Overlap(WaterBox)};

	if (OverlapBox.IsValid && WaterPostProcessDynamicMaterial)
	{
		float SubmergedPercent{static_cast<float>(OverlapBox.GetVolume() / CameraBox.GetVolume())};

		// SubmergedPercent: 0 ~ 1
		// Water_Height: 1 ~ -1
		float WaterHeightValue{1.f - 2.f * SubmergedPercent};
		//FLog::Log("", SubmergedPercent, WaterHeightValue);
		WaterPostProcessDynamicMaterial->SetScalarParameterValue(
			TEXT("Water_Height"),
			WaterHeightValue
		);
	}
}

void AFrog::ServerRPC_SetSkin_Implementation(int32 NewIndex)
{
	SkinIndex = NewIndex;
	OnRep_SkinIndex();
}

void AFrog::OnRep_SkinIndex_Implementation()
{
	if (GetMesh())
	{
		UMaterialInstanceDynamic* DynMat{GetMesh()->CreateAndSetMaterialInstanceDynamic(0)};
		if (DynMat && SkinTextures.IsValidIndex(SkinIndex))
		{
			DynMat->SetTextureParameterValue("Skin", SkinTextures[SkinIndex]);
		}
	}
}

// 플레이어 감정표현
void AFrog::OnPressCKey()
{
	if (EmotionState == EEmotionState::PlayingWinnerEmotion)
		return;

	if (EmotionState == EEmotionState::None || EmotionState == EEmotionState::PlayingEmotion)
	{
		ShowEmotionUI(true);
		EmotionState = EEmotionState::WaitingForInput;
	}
}

void AFrog::OnReleasedCKey()
{
	if (EmotionState == EEmotionState::PlayingWinnerEmotion)
		return;

	if (EmotionState == EEmotionState::WaitingForInput)
	{
		EmotionUI->ConfirmEmotionSelection();
		EmotionState = EEmotionState::PlayingEmotion;
		ShowEmotionUI(false);
	}
}

void AFrog::OnSelectionEmotionIndex(int32 EmotionIndex)
{
	if (EmotionState == EEmotionState::PlayingWinnerEmotion)
		return;

	// 선택된 감정 처리
	ShowEmotionUI(false);
	if (bCanPlayEmotion)
	{
		PlayEmotion(EmotionIndex);
	}

	FFastLogger::LogScreen(FColor::Red, TEXT("선택인덱스: %d"), EmotionIndex);
}

void AFrog::ShowEmotionUI(bool bIsShow)
{
	if (!EmotionUI)
		return;
	if (EmotionState == EEmotionState::PlayingWinnerEmotion)
		return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (bIsShow)
	{
		EmotionUI->PlayShowAnim(true);

		if (!bIsBind)
		{
			EmotionUI->OnEmotionSelected.BindUObject(this, &AFrog::OnSelectionEmotionIndex);
			bIsBind = true;
		}

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(InputMode);
	}
	else
	{
		EmotionUI->PlayShowAnim(false);

		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void AFrog::CancelEmotion()
{
	// 로컬 플레이어인 경우에만 서버에 요청을 보냄
	AFrog* MyCharacter = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (MyCharacter)
	{
		// 서버에 요청할 때 어떤 캐릭터가 요청했는지 명확히 전달
		ServerRPC_CancelEmotion(MyCharacter);
	}
}

void AFrog::ServerRPC_CancelEmotion_Implementation(AFrog* Character)
{
	// 유효한 캐릭터인지 확인
	if (Character && Character->GetController() == this->GetController())
	{
		// 특정 캐릭터에서만 감정표현 실행
		Character->MulticastRPC_CancelEmotion();
	}
}

void AFrog::MulticastRPC_CancelEmotion_Implementation()
{
	if (EmotionState == EEmotionState::PlayingEmotion && CurrentEmotionMontage)
	{
		StopAnimMontage(CurrentEmotionMontage);
		ChangeEyeMaterial(0);
		CurrentEmotionMontage = nullptr;
	}

	EmotionState = EEmotionState::None;
}

void AFrog::PlayEmotion(int32 EmotionIndex)
{
	// 로컬 플레이어인 경우에만 서버에 요청을 보냄
	AFrog* MyCharacter = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (MyCharacter)
	{
		// 서버에 요청할 때 어떤 캐릭터가 요청했는지 명확히 전달
		ServerRPC_PlayEmotion(MyCharacter, EmotionIndex);
	}
}

void AFrog::ServerRPC_PlayEmotion_Implementation(AFrog* Character, int32 EmotionIndex)
{
	// 유효한 캐릭터인지 확인
	if (Character && Character->GetController() == this->GetController())
	{
		// 특정 캐릭터에서만 감정표현 실행
		Character->MulticastRPC_PlayEmotion(EmotionIndex);
	}
}

void AFrog::MulticastRPC_PlayEmotion_Implementation(int32 EmotionIndex)
{
	UMaterialInterface* CurrentEyeMaterial = nullptr;

	switch (EmotionIndex)
	{
	case 0:
		CurrentEmotionMontage = GreetingMontage;
		break;
	case 1:
		CurrentEmotionMontage = SadMontage;
		break;
	case 2:
		CurrentEmotionMontage = AngryMontage;
		break;
	case 3:
		CurrentEmotionMontage = MerongMontage;
		break;
	default:
		return;
	}

	if (CurrentEmotionMontage)
	{
		float Duration = PlayAnimMontage(CurrentEmotionMontage);
		if (Duration > 0.f)
		{
			EmotionState = EEmotionState::PlayingEmotion;
			ChangeEyeMaterial(EmotionIndex);

			SaveCurrentMontage = CurrentEmotionMontage;
			SaveCurrentIndex = EmotionIndex;
		}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 콜백 델리게이트 생성
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AFrog::OnEmotionMontageEnd);

		// 몽타주가 끝나면 델리게이트 호출
		AnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentEmotionMontage);
	}
}

void AFrog::OnEmotionMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	// 다른 몽타주가 끝난 것이라면 무시
	if (Montage != SaveCurrentMontage)
		return;

	EmotionState = EEmotionState::None;
	ChangeEyeMaterial(0);
	SaveCurrentMontage = nullptr;
	SaveCurrentIndex = -1;
}

void AFrog::ChangeEyeMaterial(int32 MatIndex)
{
	if (GetMesh())
	{
		UMaterialInstanceDynamic* DynMat{GetMesh()->CreateAndSetMaterialInstanceDynamic(1)};
		if (DynMat && EyeTextures.IsValidIndex(MatIndex))
		{
			DynMat->SetTextureParameterValue("Eye", EyeTextures[MatIndex]);
		}
	}
}

void AFrog::ActivateRecentlyLaunchedFlag()
{
	bRecentlyLaunched = true;

	GetWorldTimerManager().ClearTimer(LaunchCooldownTimer);
	
	TWeakObjectPtr<AFrog> WeakThis{this};
	GetWorldTimerManager().SetTimer(LaunchCooldownTimer, [WeakThis]() {
		if (!WeakThis.IsValid())
		{
			return;
		}
		AFrog* StrongThis = WeakThis.Get();
		StrongThis->bRecentlyLaunched = false;
	}, 0.2f, false);
}

void AFrog::ServerRPC_ProcessOverlap_Implementation(class AObstacleProp* ObstacleProp)
{
	if (!ObstacleProp || this->IsLocallyControlled())
	{
		return;
	}
	ObstacleProp->MulticastRPC_PlayEffect(ObstacleProp->GetActorLocation());
}

void AFrog::SetFrogVignetteIntensity_PP(float Value)
{
	SettingPostProcessComponent->Settings.VignetteIntensity = Value;
}

void AFrog::SetFrogGlobalGain_PP(float Value)
{
	Value *= 2;
	Value = FMath::Clamp(Value, 0.1f, 1.8f);

	SettingPostProcessComponent->Settings.ColorGain.Set(1, 1, 1, Value);
	FLog::Log(TEXT("SetFrogGlobalGain_PP: Value"), Value);

	//SettingPostProcessComponent->Settings.ColorGainMidtones.Set(1, 1, 1, Value);
	//SettingPostProcessComponent->Settings.ColorGainHighlights.Set(1, 1, 1, Value);
	//SettingPostProcessComponent->Settings.ColorGainShadows.Set(1, 1, 1, Value);
}

void AFrog::FrogSkinFinder()
{
	ConstructorHelpers::FObjectFinder<UTexture2D> Texture1
		(TEXT("/Game/Characters/Fat_Frog/textures/frog_body_BaseColor_1.frog_body_BaseColor_1"));
	if (Texture1.Succeeded())
	{
		SkinTextures.Add(Texture1.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> Texture2
		(TEXT("/Game/Characters/Fat_Frog/textures/frog_body_BaseColor_2.frog_body_BaseColor_2"));
	if (Texture2.Succeeded())
	{
		SkinTextures.Add(Texture2.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> Texture3
		(TEXT("/Game/Characters/Fat_Frog/textures/frog_body_BaseColor_3.frog_body_BaseColor_3"));
	if (Texture3.Succeeded())
	{
		SkinTextures.Add(Texture3.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> Texture4
		(TEXT("/Game/Characters/Fat_Frog/textures/frog_body_BaseColor_4.frog_body_BaseColor_4"));
	if (Texture4.Succeeded())
	{
		SkinTextures.Add(Texture4.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> Texture5
		(TEXT("/Game/Characters/Fat_Frog/textures/frog_body_BaseColor_5.frog_body_BaseColor_5"));
	if (Texture5.Succeeded())
	{
		SkinTextures.Add(Texture5.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> Texture6
		(TEXT("/Game/Characters/Fat_Frog/textures/frog_body_BaseColor_6.frog_body_BaseColor_6"));
	if (Texture6.Succeeded())
	{
		SkinTextures.Add(Texture6.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> Texture7
		(TEXT("/Game/Characters/Fat_Frog/textures/frog_body_BaseColor_7.frog_body_BaseColor_7"));
	if (Texture7.Succeeded())
	{
		SkinTextures.Add(Texture7.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> Texture8
		(TEXT("/Game/Characters/Fat_Frog/textures/frog_body_BaseColor_8.frog_body_BaseColor_8"));
	if (Texture8.Succeeded())
	{
		SkinTextures.Add(Texture8.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> Texture9
		(TEXT("/Game/Characters/Fat_Frog/textures/frog_body_BaseColor_9.frog_body_BaseColor_9"));
	if (Texture9.Succeeded())
	{
		SkinTextures.Add(Texture9.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> Texture10
		(TEXT("/Game/Characters/Fat_Frog/textures/frog_body_BaseColor_10.frog_body_BaseColor_10"));
	if (Texture10.Succeeded())
	{
		SkinTextures.Add(Texture10.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> EyeTexture1
		(TEXT("/Script/Engine.Texture2D'/Game/Characters/Fat_Frog/EyeTextures/T_EyeBasic.T_EyeBasic'"));
	if (EyeTexture1.Succeeded())
	{
		EyeTextures.Add(EyeTexture1.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> EyeTexture2
		(TEXT("/Script/Engine.Texture2D'/Game/Characters/Fat_Frog/EyeTextures/T_EyeAngry.T_EyeAngry'"));
	if (EyeTexture2.Succeeded())
	{
		EyeTextures.Add(EyeTexture2.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> EyeTexture3
		(TEXT("/Script/Engine.Texture2D'/Game/Characters/Fat_Frog/EyeTextures/T_EyeSleep.T_EyeSleep'"));
	if (EyeTexture3.Succeeded())
	{
		EyeTextures.Add(EyeTexture3.Object);
	}
	ConstructorHelpers::FObjectFinder<UTexture2D> EyeTexture4
		(TEXT("/Script/Engine.Texture2D'/Game/Characters/Fat_Frog/EyeTextures/T_EyeCute.T_EyeCute'"));
	if (EyeTexture4.Succeeded())
	{
		EyeTextures.Add(EyeTexture4.Object);
	}

	ConstructorHelpers::FObjectFinder<UTexture2D> EyeTexture5
		(TEXT("/Script/Engine.Texture2D'/Game/Characters/Fat_Frog/EyeTextures/T_EyeConfuse.T_EyeConfuse'"));
	if (EyeTexture5.Succeeded())
	{
		EyeTextures.Add(EyeTexture5.Object);
	}
}

void AFrog::OnPressESCKey()
{
	if (GameSettingUI)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (!bIsPress)
		{
			GameSettingUI->PlaySettingAnim(true);

			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(GameSettingUI->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			InputMode.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputMode);

			PC->SetIgnoreLookInput(true);
			PC->SetIgnoreMoveInput(true);
			PC->bShowMouseCursor = true;

			bIsPress = true;
		}
		else
		{
			GameSettingUI->PlaySettingAnim(false);

			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);

			PC->SetIgnoreLookInput(false);
			PC->SetIgnoreMoveInput(false);
			PC->bShowMouseCursor = false;

			bIsPress = false;
		}
	}
}
