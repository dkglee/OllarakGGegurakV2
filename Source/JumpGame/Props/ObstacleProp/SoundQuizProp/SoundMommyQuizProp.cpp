// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMommyQuizProp.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/UI/Obstacle/SoundQuizClear.h"
#include "JumpGame/UI/Obstacle/SoundQuizFail.h"
#include "JumpGame/UI/Obstacle/SoundQuizUI.h"
#include "JumpGame/UI/Obstacle/StartSoundUI.h"
#include "JumpGame/UI/Obstacle/TimeRemainUI.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"


// Sets default values
ASoundMommyQuizProp::ASoundMommyQuizProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/SoundMommy/SM_Radio.SM_Radio"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("2003"));

	static ConstructorHelpers::FObjectFinder<USoundCue> ClearSoundAsset
	(TEXT("/Game/Sounds/Ques/SQ_QuizClear.SQ_QuizClear"));
	if (ClearSoundAsset.Succeeded())
	{
		ClearSound = Cast<USoundCue>(ClearSoundAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> FailSoundAsset
	(TEXT("/Game/Sounds/Ques/SQ_QuizFail.SQ_QuizFail"));
	if (FailSoundAsset.Succeeded())
	{
		FailSound = Cast<USoundCue>(FailSoundAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> TempClearEffect
	(TEXT("/Game/_Resource/FX/VFX_Toolkit_V1/ParticleSystems/356Days/Par_RainBlow_01.Par_RainBlow_01"));
	if (TempClearEffect.Succeeded())
	{
		ClearEffect = Cast<UParticleSystem>(TempClearEffect.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> TempFailEffect
	(TEXT("/Game/_Resource/FX/VFX_Toolkit_V1/ParticleSystems/356Days/Par_Galaxoid_01.Par_Galaxoid_01"));
	if (TempFailEffect.Succeeded())
	{
		FailEffect = Cast<UParticleSystem>(TempFailEffect.Object);
	}
}

// Called when the game starts or when spawned
void ASoundMommyQuizProp::BeginPlay()
{
	Super::BeginPlay();

	StartSoundUI = CreateWidget<UStartSoundUI>(GetWorld(), StartSoundUIClass);
	SoundQuizUI = CreateWidget<USoundQuizUI>(GetWorld(), SoundQuizUIClass);
	SoundQuizUI->SetVoiceRecorderComponent(VoiceRecorderComponent);
	SoundQuizFail = CreateWidget<USoundQuizFail>(GetWorld(), SoundQuizFailUIClass);
	SoundQuizClear = CreateWidget<USoundQuizClear>(GetWorld(), SoundQuizClearUIClass);
	if (SoundQuizUI)
	{
		SoundQuizUI->WBP_TimeRemain->SetBarThickness(100.f);
		SoundQuizUI->WBP_TimeRemain->SetVisibility(ESlateVisibility::Hidden);
	}
	
	// 미션 종료 시 실행할 함수 바인딩
	SoundQuizUI->WBP_TimeRemain->OnMissionTimerEnd.AddDynamic(this, &ASoundMommyQuizProp::StopRecord);
}

void ASoundMommyQuizProp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ASoundMommyQuizProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoundMommyQuizProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                        SweepResult);
	
	if (!PC) return;
	// 각각의 화면에서 UI를 띄우자
	// 그렇게 해야 remove했을때 본인의 화면에서 사라짐
	if (PC->IsLocalPlayerController() && !bIsOverlap)
	{
		if (StartSoundUI)
		{
			StartSoundUI->AddToViewport();
			StartSoundUI->PlayStartSoundAnim();
		}
		bIsOverlap = true;
	}
}

void ASoundMommyQuizProp::ReceiveSoundQuizMessage()
{
	Super::ReceiveSoundQuizMessage();
	
	// 20번 넘으면 자동 게임 종료, 디버프를 받는다 (못맞춤)
	if (SendResponseIdx >= 20)
	{
		// UI 지우자
		SoundQuizUI->RemoveFromParent();
		// 실패...
		if (SoundQuizFail)
		{
			SoundQuizFail->AddToViewport();
			SoundQuizFail->PlayQuizFailAnim();
		}

		FVector SpawnLocation = Character->GetActorLocation() + FVector(100, 0, 120);
		UParticleSystemComponent* SpawnedEffect = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),	FailEffect,	SpawnLocation, FRotator::ZeroRotator, FVector(5), true);

		// 타이머로 5초 후 수동 Destroy
		if (SpawnedEffect)
		{
			TWeakObjectPtr<UParticleSystemComponent> WeakThis(SpawnedEffect);
			GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateLambda([WeakThis]()
			{
				if (!WeakThis.IsValid())
				{
					return;
				}
				
				UParticleSystemComponent* StrongThis = WeakThis.Get();
				StrongThis->DestroyComponent(); // 파티클 제거
			}), 5.0f, false);
		}
		
		bIsClear = false;
		// 퀴즈 끝났다고 알리자!
		SendEndSoundQuizNotify();

		GetWorld()->GetTimerManager().SetTimer(UIRemoveTimerHandle, this, &ASoundMommyQuizProp::RemoveSoundQuizUI, 3.0f, false);
		return;
	}

	// TODO: 정답과 일치할때로 변경해야함
	// 20번 안에, Fin되는 경우 -> 유사도가 높을때
	if (SendResponseIdx < 20)
	{
		if (bSuccess == 1 || Similarity*100 >= 89)
		{
			// UI 지우자
			SoundQuizUI->RemoveFromParent();
			// 성공!
			if (SoundQuizClear)
			{
				SoundQuizClear->AddToViewport();
				SoundQuizClear->PlayQuizAnim();
			}

			FVector SpawnLocation = Character->GetActorLocation() + FVector(100, 0, 100);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ClearEffect, SpawnLocation, FRotator(0, 90, 0), FVector(0.5f), true);

			bIsClear = true;
			// 퀴즈 끝났다고 알리자!
			SendEndSoundQuizNotify();

			GetWorld()->GetTimerManager().SetTimer(UIRemoveTimerHandle, this, &ASoundMommyQuizProp::RemoveSoundQuizUI, 3.0f, false);
			return;
		}
	}

	SoundQuizUI->UpdateFromResponse(Similarity*100, MessageStr);
}

void ASoundMommyQuizProp::SendSoundQuizMessage()
{
	Super::SendSoundQuizMessage();
	
}

void ASoundMommyQuizProp::SendEndSoundQuizNotify()
{
	Super::SendEndSoundQuizNotify();
	
	if (StartSoundUI)
	{
		StartSoundUI->RemoveFromParent();
	}
	SoundQuizUI->RemoveFromParent();

	Character = Cast<AFrog>(PC->GetPawn());
	FVector CharcaterPos = Character->GetActorLocation();
	
	if (bIsClear)
	{
		// 성공 음성
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ClearSound, CharcaterPos, 0.8f, 1.0f);
	}
	else
	{
		// 실패 음성
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FailSound, CharcaterPos, 0.8f, 1.0f);
	}
}

void ASoundMommyQuizProp::StartSoundQuiz()
{
	Super::StartSoundQuiz();
	
	if (!PC) return;
	
	if (PC->IsLocalPlayerController())
	{
		// 시작하면 UI 띄우자
		if (SoundQuizUI)
		{
			SoundQuizUI->AddToViewport();
		}
		if (StartSoundUI)
		{
			StartSoundUI->RemoveFromParent();
		}
	}
}

void ASoundMommyQuizProp::StartRecord()
{
	Super::StartRecord();

	// 남은 시간 UI
	SoundQuizUI->WBP_TimeRemain->StartMissionTimer(3.0f);
	SoundQuizUI->WBP_TimeRemain->SetVisibility(ESlateVisibility::Visible);

	// 버튼 이름 바꾸고
	SoundQuizUI->Text_VoiceSend->SetText(FText::FromString(TEXT("대답 녹음중!")));
	// 버튼 비활성화
	SoundQuizUI->Btn_VoiceSend->SetIsEnabled(false);
}

void ASoundMommyQuizProp::StopRecord()
{
	Super::StopRecord();

	SoundQuizUI->WBP_TimeRemain->SetVisibility(ESlateVisibility::Hidden);
	SoundQuizUI->Text_VoiceSend->SetText(FText::FromString(TEXT("엄마 말 듣는중...")));
	// 버튼 다시 활성화
	SoundQuizUI->Btn_VoiceSend->SetIsEnabled(true);
}


void ASoundMommyQuizProp::RemoveSoundQuizUI()
{
	if (SoundQuizClear)
	{
		SoundQuizClear->RemoveFromParent();
	}

	if (SoundQuizFail)
	{
		SoundQuizFail->RemoveFromParent();
	}
}
