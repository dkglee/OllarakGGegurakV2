// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectEyeHunterProp.h"

#include "Components/BoxComponent.h"
#include "Components/Image.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/UI/Obstacle/EyeTrackingUI.h"
#include "JumpGame/UI/Obstacle/FlyingObjectUI.h"
#include "JumpGame/UI/Obstacle/TimeRemainUI.h"
#include "JumpGame/UI/Obstacle/LookCenter.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

AObjectEyeHunterProp::AObjectEyeHunterProp()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UEyeTrackingUI> EyeTrackingUIWidget
		(TEXT("/Game/UI/Obstacle/WBP_EyeTracking.WBP_EyeTracking_C"));
	if (EyeTrackingUIWidget.Succeeded())
	{
		EyeTrackingUIClass = EyeTrackingUIWidget.Class;
	}

	ConstructorHelpers::FClassFinder<UFlyingObjectUI> FlyingObjectWidget
		(TEXT("/Game/UI/Obstacle/WBP_FlyingObject.WBP_FlyingObject_C"));
	if (FlyingObjectWidget.Succeeded())
	{
		FlyingObjectUIClass = FlyingObjectWidget.Class;
	}

	// ConstructorHelpers::FClassFinder<UTimeRemainUI> TimeRemainWidget
	// 	(TEXT("/Game/UI/Obstacle/WBP_TimeRemain.WBP_TimeRemain_C"));
	// if (TimeRemainWidget.Succeeded())
	// {
	// 	TimeRemainUIClass = TimeRemainWidget.Class;
	// }

	ConstructorHelpers::FClassFinder<ULookCenter> LookCenterWidget
		(TEXT("/Game/UI/Obstacle/WBP_LookCenter.WBP_LookCenter_C"));
	if (LookCenterWidget.Succeeded())
	{
		LookCenterUIClass = LookCenterWidget.Class;
	}

	MissionLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MissionLocation"));
	if (MissionLocation)
	{
		MissionLocation->SetupAttachment(RootComponent);
		MissionLocation->SetRelativeLocation(FVector(0, 0, 100.f));
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundAsset
		(TEXT("/Game/Sounds/Ques/Bounce_Cue.Bounce_Cue"));
	if (SoundAsset.Succeeded())
	{
		LaunchSound = Cast<USoundCue>(SoundAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> PlantMoveSoundAsset
		(TEXT("/Game/Sounds/Ques/PlantMove_Cue.PlantMove_Cue"));
	if (PlantMoveSoundAsset.Succeeded())
	{
		PlantMoveSound = Cast<USoundCue>(PlantMoveSoundAsset.Object);
	}

	CollisionComp->SetRelativeLocation(FVector(0, 0, 50.f));
	MeshComp->SetRelativeLocation(FVector(0, 0, -100.f));

	bReplicates = true;

	PropDataComponent->SetPropID(TEXT("2006"));
}

void AObjectEyeHunterProp::BeginPlay()
{
	Super::BeginPlay();

	InitializeMission();
}


void AObjectEyeHunterProp::InitializeMission()
{
	// 위젯 미리 생성
	FlyingObjectUI = CreateWidget<UFlyingObjectUI>(GetWorld(), FlyingObjectUIClass);
	TrackingUI = CreateWidget<UEyeTrackingUI>(GetWorld(), EyeTrackingUIClass);
	//TimeRemainUI = CreateWidget<UTimeRemainUI>(GetWorld(), TimeRemainUIClass);
	LookCenterUI = CreateWidget<ULookCenter>(GetWorld(), LookCenterUIClass);

	if (FlyingObjectUI)
	{
		// 미션 타이머 종료 바인드
		FlyingObjectUI->WBP_TimeRemain->OnMissionTimerEnd.AddDynamic(this, &AObjectEyeHunterProp::MissionTimeEnd);
	}

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
}

void AObjectEyeHunterProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                            AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                            bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	//Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AFrog* OverlappingFrog{Cast<AFrog>(OtherActor)};
	if (OverlappingFrog)
	{
		if (OverlappingFrog->IsLocallyControlled())
		{
			Frog = OverlappingFrog;

			StopCharacter();
			DisplayMessage();
			GetWorldTimerManager().SetTimer(StartTimerHandle, this, &AObjectEyeHunterProp::StartMission, 2.f, false);
			//StartMission();
		}

		if (HasAuthority())
		{
			SetOwner(OverlappingFrog);
			HitSound = PlantMoveSound;
			MulticastRPC_PlayEffect(this->GetActorLocation());
		}
	}

	// if (OtherActor->ActorHasTag(TEXT("Frog")))
	// {
	// 	Frog = Cast<AFrog>(OtherActor);
	//
	// 	StartMission();
	// }
}

void AObjectEyeHunterProp::StartMission()
{
	FLog::Log("StartMission");

	Super::SendEyeTrackingStart();

	if (LookCenterUI)
	{
		LookCenterUI->RemoveFromParent();
	}

	//StopCharacter();

	if (FlyingObjectUI)
	{
		FlyingObjectUI->InitializeParameters();
		// 10초 후 미션 종료 ( 타이머 설정 )
		FlyingObjectUI->WBP_TimeRemain->StartMissionTimer(20.f);
	}

	// if (TimeRemainUI)
	// {
	// 	TimeRemainUI->AddToViewport();
	// }

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Object가 움직일 경로 (비율로 저장)
	ChooseTargetPositionsRatio();

	// 초기 목표 위치 계산
	SetTargetPositionsByViewport();

	CurrentTargetIndex = 0;
	BezierAlpha = 0.f;
	ObjectSpeed = 0.3f;

	if (FlyingObjectUI && TrackingUI)
	{
		FlyingObjectUI->AddToViewport();
		TrackingUI->AddToViewport();

		// 내 Screen 좌표로 변환
		const float ScreenX{static_cast<float>(ViewportSize.X / 2)};
		const float ScreenY{static_cast<float>(ViewportSize.Y / 2)};

		ObjectScreenLocation = {ScreenX, ScreenY};

		CurrentEyePosition = ObjectScreenLocation;
		TargetEyePosition = ObjectScreenLocation;

		FlyingObjectUI->SetPositionInViewport(ObjectScreenLocation);
		TrackingUI->SetPositionInViewport(ObjectScreenLocation);
	}

	// 미션 시작
	bIsStartHunt = true;

	// 시작 위치 설정
	StartPosition = ObjectScreenLocation;
	if (TargetPositions.IsValidIndex(CurrentTargetIndex))
	{
		// 초기 목표 위치 설정
		TargetPosition = TargetPositions[CurrentTargetIndex];
		// 초기 제어점들 생성, 랜덤 반경 설정
		ControlPoint1 = GenerateRandomControlPoint(StartPosition, TargetPosition, 300.0f);
		ControlPoint2 = GenerateRandomControlPoint(StartPosition, TargetPosition, 300.0f);
	}
}

void AObjectEyeHunterProp::StopCharacter()
{
	if (Frog && Frog->IsLocallyControlled())
	{
		Frog->CameraMissionMode();
		Frog->StopMovementAndResetRotation(GetActorRotation());
		Frog->SetCrouchEnabled(false);

		Frog->ServerRPC_PrepareMission(MissionLocation->GetComponentLocation(),
		                               MissionLocation->GetComponentRotation());

		// FLog::Log("ClientLoc", GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
		// FLog::Log("ClientRoc", GetActorRotation().Yaw, GetActorRotation().Pitch);
	}
}

void AObjectEyeHunterProp::ResumeCharacter()
{
	if (Frog && Frog->IsLocallyControlled())
	{
		Frog->ResumeMovement();
		Frog->CameraMovementMode();
		Frog->SetCrouchEnabled(true);

		Frog->ServerRPC_FinishMission();
	}
}

void AObjectEyeHunterProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                          AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Super::OnMyEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	AFrog* OverlappingFrog{Cast<AFrog>(OtherActor)};
	if (OverlappingFrog)
	{
		if (OverlappingFrog->IsLocallyControlled())
		{
			if (bIsStartHunt)
			{
				EndMission(false);
			}

			Frog = nullptr;

			SetOwner(nullptr);
		}
	}


	// if (OtherActor->ActorHasTag(TEXT("Frog")))
	// {
	// 	if (bIsStartHunt)
	// 	{
	// 		EndMission(false);
	// 	}
	// }
}

void AObjectEyeHunterProp::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AObjectEyeHunterProp, Frog);
}

void AObjectEyeHunterProp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorldTimerManager().ClearTimer(EndTimerHandle);
		GetWorldTimerManager().ClearTimer(StartTimerHandle);
		GetWorldTimerManager().ClearTimer(DynamicTimerHandle);
	}
	
	Super::EndPlay(EndPlayReason);
}

void AObjectEyeHunterProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsStartHunt)
	{
		return;
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
	{
		if (!bIsDebug)
		{
			bIsDebug = true;
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
		else
		{
			bIsDebug = false;
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		}
	}

	// 현재 위치를 이전 위치로 (회전 계산 위해)
	PreviousPosition = ObjectScreenLocation;
	FlyingObjectMovement(DeltaTime);

	if (bIsDebug && TrackingUI)
	{
		if (GEngine && GEngine->GameViewport)
		{
			FVector2D MousePosition;
			GEngine->GameViewport->GetMousePosition(MousePosition);

			EyeScreenLocation = {MousePosition.X, MousePosition.Y};

			TrackingUI->SetPositionInViewport(EyeScreenLocation);
		}
	}

	// Viewport 크기 게임 중 바뀔 수 있으니까
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D CurrentViewportSize;
		GEngine->GameViewport->GetViewportSize(CurrentViewportSize);

		if (CurrentViewportSize != ViewportSize)
		{
			SetTargetPositionsByViewport();
			ViewportSize = CurrentViewportSize;
		}
	}

	if (!bIsDebug && TrackingUI)
	{
		RecvEyeTrackingInfo();

		CurrentEyePosition = FMath::Lerp(CurrentEyePosition, TargetEyePosition, 0.2);
		TrackingUI->SetPositionInViewport(CurrentEyePosition);
	}
}

void AObjectEyeHunterProp::RecvEyeTrackingInfo()
{
	Super::RecvEyeTrackingInfo();

	if (QuizID == 0)
	{
		return;
	}

	// TODO : 받아오는 값으로 수정
	TrackLocation({2880, 1800}, FVector2f(X, Y));
	//TrackLocation(FVector2f(Width, Height), FVector2f(X, Y))
}

void AObjectEyeHunterProp::DisplayMessage_Implementation()
{
	// 메시지 띄우기
	if (LookCenterUI)
	{
		LookCenterUI->AddToViewport();
		LookCenterUI->TextMoveAnimation();
	}
}

void AObjectEyeHunterProp::FlyingObjectMovement(float DeltaTime)
{
	if (FlyingObjectUI && bIsStartHunt)
	{
		if (!FlyingObjectUI->IsInViewport())
		{
			FlyingObjectUI->AddToViewport();
		}

		if (TargetPositions.IsValidIndex(CurrentTargetIndex))
		{
			BezierAlpha += ObjectSpeed * DeltaTime;

			// 도착하면
			if (BezierAlpha >= 1.f)
			{
				// 다시 초기화
				BezierAlpha = 0.f;
				// 목표 위치에 도달
				ObjectScreenLocation = TargetPosition;
				// 시작점 설정
				StartPosition = ObjectScreenLocation;
				SetNextTargetPosition();

				// 새로운 제어점 생성
				ControlPoint1 = GenerateRandomControlPoint(StartPosition, TargetPosition, 300.f);
				ControlPoint2 = GenerateRandomControlPoint(StartPosition, TargetPosition, 300.f);
			}
			else
			{
				// 베지어 곡선 따라 현재 위치 계산
				ObjectScreenLocation = GetBezierPoint(StartPosition, ControlPoint1, ControlPoint2,
				                                      TargetPosition,
				                                      BezierAlpha);
			}

			FlyingObjectUI->SetPositionInViewport(ObjectScreenLocation);
			UpdateObjectRotation(DeltaTime);
		}

		// 겹쳐있으면 Value 바꾸기
		if (IsObjectAndEyeOverlap(ObjectScreenLocation, EyeScreenLocation) && bIsStartHunt)
		{
			ChangeValue(true);
		}
		else
		{
			ChangeValue(false);
		}
	}
}

void AObjectEyeHunterProp::TrackLocation(FVector2f Resolution, FVector2f ScreenLoc)
{
	// 정규화
	float NormalizedX{ScreenLoc.X / Resolution.X};
	float NormalizedY{ScreenLoc.Y / Resolution.Y};

	// 내 Screen 좌표로 변환
	float ScreenX{static_cast<float>(NormalizedX * ViewportSize.X)};
	float ScreenY{static_cast<float>(NormalizedY * ViewportSize.Y)};

	//FLog::Log("Loc", ScreenX, ScreenY);

	if (TrackingUI && bIsStartHunt)
	{
		// 뷰포트에 없으면 추가
		if (!TrackingUI->IsInViewport())
		{
			TrackingUI->AddToViewport();
		}

		// 위치 갱신
		EyeScreenLocation = FVector2D{ScreenX, ScreenY};
		TargetEyePosition = EyeScreenLocation;
		//TrackingUI->SetPositionInViewport(EyeScreenLocation);
	}
}

bool AObjectEyeHunterProp::IsObjectAndEyeOverlap(FVector2D ObjectLocation, FVector2D EyeLocation)
{
	if (TrackingUI && FlyingObjectUI)
	{
		float Length{
			static_cast<float>(FMath::Abs(FVector2D::Distance(EyeLocation, ObjectLocation)))
		};

		if (Length <= 120.f)
		{
			FlyingObjectUI->Overlapping();
			return true;
		}
	}

	FlyingObjectUI->NotOverlapping();
	return false;
}

void AObjectEyeHunterProp::ChangeValue(bool bIsOverlap)
{
	if (bIsOverlap)
	{
		FlowTime = FMath::Clamp(FlowTime + (GetWorld()->GetDeltaSeconds() * 2), 0.f, SuccessTime);
	}
	else
	{
		FlowTime = FMath::Clamp(FlowTime - (GetWorld()->GetDeltaSeconds() / 2), 0.f, SuccessTime);
	}

	SuccessRatio = FMath::Clamp(FlowTime / SuccessTime, 0.f, 1.f);
	FlyingObjectUI->ChangeGaugeValue(SuccessRatio);

	if (FMath::IsNearlyEqual(FlyingObjectUI->GetGaugeValue(), 1.f))
	{
		EndMission(true);
	}
}

void AObjectEyeHunterProp::MissionTimeEnd()
{
	EndMission(false);
}

void AObjectEyeHunterProp::EndMission_Implementation(bool bIsSuccess)
{
	if (bIsSuccess)
	{
		// 미션 성공 : 발사 !
		FLog::Log("Success");

		if (Frog)
		{
			FVector Direction{FVector::UpVector};
			float Force{2'000.f};

			Frog->ServerRPC_CallLaunchCharacter(Direction, Force, false, false);
			//Super::LaunchCharacter(Frog, Direction, Force);
		}

		FlyingObjectUI->SuccessMission();
	}
	else
	{
		// 미션 실패 및 비정상 종료
		FLog::Log("Failure");

		if (Frog)
		{
			FVector Direction{FVector::UpVector};
			float Force{800};

			Frog->ServerRPC_CallLaunchCharacter(Direction, Force, false, false);
			//Super::LaunchCharacter(Frog, Direction, Force);
		}

		FlyingObjectUI->FailMission();
	}

	HitSound = LaunchSound;

	if (HasAuthority())
	{
		MulticastRPC_PlayEffect(this->GetActorLocation(), 1);
	}
	else
	{
		ServerRPC_PlayEffect(this->GetActorLocation(), 1);
	}

	ResetMission();
}

void AObjectEyeHunterProp::ResetMission()
{
	ResumeCharacter();

	// 미션 끝
	bIsStartHunt = false;

	FlyingObjectUI->WBP_TimeRemain->StopMissionTimer();

	if (TrackingUI)
	{
		TrackingUI->RemoveFromParent();
	}

	// if (TimeRemainUI)
	// {
	// 	FlyingObjectUI->WBP_TimeRemain->RemoveFromParent();
	// }

	FlowTime = 0.f;
	SuccessRatio = 0.f;

	bIsDebug = false;

	ObjectScreenLocation = FVector2D::Zero();
	EyeScreenLocation = FVector2D::Zero();

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	Super::SendEyeTrackingEnd();

	Frog = nullptr;

	GetWorldTimerManager().SetTimer(EndTimerHandle, FlyingObjectUI, &UFlyingObjectUI::VanishMission,
	                                0.5f, false);
	
	CollisionComp->OnComponentBeginOverlap.RemoveDynamic(this, &AObstacleProp::OnMyBeginOverlap);
	CollisionComp->OnComponentEndOverlap.RemoveDynamic(this, &AObstacleProp::OnMyEndOverlap);

	GetWorldTimerManager().SetTimer(DynamicTimerHandle, this, &AObjectEyeHunterProp::AddDynamicOverlap,
								1.f, false);
}

void AObjectEyeHunterProp::SetNextTargetPosition()
{
	// 끝 도달하면 다시 0으로
	++CurrentTargetIndex;
	if (CurrentTargetIndex >= TargetPositions.Num())
	{
		CurrentTargetIndex = 0;
	}

	if (TargetPositions.IsValidIndex(CurrentTargetIndex))
	{
		TargetPosition = TargetPositions[CurrentTargetIndex];
	}
}

void AObjectEyeHunterProp::UpdateObjectRotation(float DeltaTime)
{
	if (FlyingObjectUI)
	{
		FVector2D Direction{(ObjectScreenLocation - PreviousPosition).GetSafeNormal()};

		// 방향 벡터가 (0, 0)이 아닌 경우에만 회전 계산
		if (Direction.SizeSquared() > 0.f)
		{
			// Radian 계산
			float AngleRad{static_cast<float>(FMath::Atan2(Direction.Y, Direction.X))};
			// Degree로 변환
			float AngleDeg{FMath::RadiansToDegrees(AngleRad)};

			if (UFlyingObjectUI* FlyUIWidget{Cast<UFlyingObjectUI>(FlyingObjectUI)})
			{
				if (UImage* TargetImage{FlyUIWidget->TargetLocationImage})
				{
					// 이미지 회전
					TargetImage->SetRenderTransformAngle(AngleDeg + 180.f);
				}
			}
		}
	}
}

FVector2D AObjectEyeHunterProp::GetBezierPoint(FVector2D P0, FVector2D P1, FVector2D P2,
                                               FVector2D P3, float t)
{
	float u{1.f - t};
	float tt{t * t};
	float uu{u * u};
	float uuu{uu * u};
	float ttt{tt * t};

	// B(t) = P0(1 − t)^3 + 3 * P1 * t(1 - t)^2 + 3 * P2 * t^2(1 - t) + P3t^3

	// P0 * (1-t)^3
	FVector2D p{P0 * uuu};
	// 3 * P1 * (1-t)^2 * t
	p += 3.f * P1 * uu * t;
	// 3 * P2 * (1-t) * t^2
	p += 3.f * P2 * u * tt;
	// P3 * t^3
	p += P3 * ttt;

	return p;
}

FVector2D AObjectEyeHunterProp::GenerateRandomControlPoint(FVector2D StartPos, FVector2D EndPos,
                                                           float RandomRadius)
{
	FVector2D MidPoint{(StartPos + EndPos) / 2.0f};
	FVector2D RandomOffset{FMath::RandPointInCircle(RandomRadius)};

	// 시작, 끝 사이 무작위 지점 리턴
	return MidPoint + RandomOffset;
}

void AObjectEyeHunterProp::SetTargetPositionsByViewport()
{
	TargetPositions.Empty();
	for (const FVector2D& Ratio : TargetPositionsRatio)
	{
		TargetPositions.Add(FVector2D(ViewportSize.X * Ratio.X, ViewportSize.Y * Ratio.Y));
	}
}

void AObjectEyeHunterProp::ChooseTargetPositionsRatio()
{
	int32 ChooseNum{FMath::RandRange(0, 3)};

	switch (ChooseNum)
	{
	case 0:
		// 오른쪽 상단
		TargetPositionsRatio.Add(FVector2D(0.15f, 0.15f));
	// 오른쪽 하단
		TargetPositionsRatio.Add(FVector2D(0.15f, 0.85f));
	// 중간
		TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
	// 왼쪽 상단
		TargetPositionsRatio.Add(FVector2D(0.85f, 0.15f));
	// 왼쪽 하단
		TargetPositionsRatio.Add(FVector2D(0.85f, 0.85f));
	// 중간
		TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
		break;
	case 1:
		// 왼쪽 상단
		TargetPositionsRatio.Add(FVector2D(0.85f, 0.15f));
	// 왼쪽 하단
		TargetPositionsRatio.Add(FVector2D(0.85f, 0.85f));
	// 중간
		TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
	// 오른쪽 상단
		TargetPositionsRatio.Add(FVector2D(0.15f, 0.15f));
	// 오른쪽 하단
		TargetPositionsRatio.Add(FVector2D(0.15f, 0.85f));
	// 중간
		TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
		break;
	case 2:
		// 오른쪽 하단
		TargetPositionsRatio.Add(FVector2D(0.15f, 0.85f));
	// 오른쪽 상단
		TargetPositionsRatio.Add(FVector2D(0.15f, 0.15f));
	// 중간
		TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
	// 왼쪽 하단
		TargetPositionsRatio.Add(FVector2D(0.85f, 0.85f));
	// 왼쪽 상단
		TargetPositionsRatio.Add(FVector2D(0.85f, 0.15f));
	// 중간
		TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
		break;
	case 3:
		// 왼쪽 하단
		TargetPositionsRatio.Add(FVector2D(0.85f, 0.85f));
	// 왼쪽 상단
		TargetPositionsRatio.Add(FVector2D(0.85f, 0.15f));
	// 중간
		TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
	// 오른쪽 하단
		TargetPositionsRatio.Add(FVector2D(0.15f, 0.85f));
	// 오른쪽 상단
		TargetPositionsRatio.Add(FVector2D(0.15f, 0.15f));
	// 중간
		TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
		break;
	default:
		// 오른쪽 상단
		TargetPositionsRatio.Add(FVector2D(0.15f, 0.15f));
	// 오른쪽 하단
		TargetPositionsRatio.Add(FVector2D(0.15f, 0.85f));
	// 중간
		TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
	// 왼쪽 상단
		TargetPositionsRatio.Add(FVector2D(0.85f, 0.15f));
	// 왼쪽 하단
		TargetPositionsRatio.Add(FVector2D(0.85f, 0.85f));
	// 중간
		TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
		break;
	}
}

void AObjectEyeHunterProp::AddDynamicOverlap()
{
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AObstacleProp::OnMyBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AObstacleProp::OnMyEndOverlap);
}

void AObjectEyeHunterProp::ServerRPC_PlayEffect_Implementation(const FVector& Vector, int32 Index)
{
	MulticastRPC_PlayEffect(Vector, Index);
}
