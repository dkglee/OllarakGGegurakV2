// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpGame/AIServices/Shared/Message.h"
#include "JumpGame/Props/ObstacleProp/ObstacleProp.h"
#include "EyeTrackingProp.generated.h"

UCLASS(Blueprintable)
class JUMPGAME_API AEyeTrackingProp : public AObstacleProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEyeTrackingProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp,
	                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex) override;
	
	virtual void SetCollision(bool bEnable) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void RecvReadyEyeTracking();
	void SendEyeTrackingStart();
	void SendEyeTrackingEnd();
	void SendEyeTrackingSettingStart();

	virtual void RecvEyeTrackingInfo();

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USettingEyeTrackingUI> SettingEyeTrackingUIClass;
	UPROPERTY(EditAnywhere)
	USettingEyeTrackingUI* SettingEyeTrackingUI;

public:
	FMessageHeader Header;
	uint8 QuizID;
	// 퀴즈 시작 및 종료
	uint8 Start;
	uint8 End;
	// 서버 해상도
	float Width;
	float Height;
	// 위치 좌표
	float X;
	float Y;
	// 눈 감았는지
	uint8 bBlink;
	// 100 : 정상성, 200 : 클라 에러, 300 : 서버 에러
	uint8 State;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStartHunt{false};
	FVector2D SettingLocation[4]{};
	int32 SettingCount{};
	// bool bIsSettingFinish{false};	// Todo : 추후 수정 예정
	bool bIsSettingFinish{true};
};
