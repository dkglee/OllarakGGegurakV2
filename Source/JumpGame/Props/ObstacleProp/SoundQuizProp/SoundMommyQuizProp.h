// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundQuizProp.h"
#include "SoundMommyQuizProp.generated.h"

UCLASS()
class JUMPGAME_API ASoundMommyQuizProp : public ASoundQuizProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundMommyQuizProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void ReceiveSoundQuizMessage() override;
	virtual void SendSoundQuizMessage() override;
	virtual void SendEndSoundQuizNotify() override;

	virtual void StartSoundQuiz() override;
	
	virtual void StartRecord() override;
	virtual void StopRecord() override;
	
public:
	UPROPERTY()
	FTimerHandle SpawnTimerHandle;
	
	// 퀴즈 시작 UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UStartSoundUI> StartSoundUIClass;
	UPROPERTY(EditAnywhere)
	UStartSoundUI* StartSoundUI;
	
	// 퀴즈 플레이 UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USoundQuizUI> SoundQuizUIClass;
	UPROPERTY(editAnywhere)
	USoundQuizUI* SoundQuizUI;

	// 퀴즈 성공 실패 UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USoundQuizClear> SoundQuizClearUIClass;
	UPROPERTY(EditAnywhere)
	USoundQuizClear* SoundQuizClear;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USoundQuizFail> SoundQuizFailUIClass;
	UPROPERTY(EditAnywhere)
	USoundQuizFail* SoundQuizFail;

	// 몇 초 후에는 지우자
	void RemoveSoundQuizUI();
	FTimerHandle UIRemoveTimerHandle;
	// UPROPERTY()
	// TMap<int32, FTimerHandle> PlayerUIRemoveTimers;

	// 사운드 및 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* ClearSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* FailSound;
	
	UPROPERTY(EditAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ClearEffect;
	UPROPERTY(EditAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* FailEffect;
	
};
