// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"

#include "RollingBallProp.h"
#include "RollingCannonProp.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UObjectPoolComponent::DestoryObjectPool()
{
	for (ARollingBallProp* Ball : Pool)
	{
		if (Ball)
		{
			Ball->Destroy();
		}
	}

	for (ARollingBallProp* Ball : InactivePool)
	{
		if (Ball)
		{
			Ball->Destroy();
		}
	}
}

// Called when the game starts
void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Initialize();
}

void UObjectPoolComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ObPoolTimerHandle);
	}
	
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UObjectPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

class ARollingBallProp* UObjectPoolComponent::GetRollingBallProp()
{
	// 0개면 추가하자
	if (Pool.Num() == 0)
	{
		FFastLogger::LogConsole(TEXT("풀 비었음"));
		// 생성하지 말자
		return nullptr;
	}
	// 마지막 오브젝트 반환
	ARollingBallProp* BallProp = Pool.Pop();
	InactivePool.Push(BallProp);
	return BallProp;
}

void UObjectPoolComponent::Expand()
{
	// 풀에 오브젝트 부족하면 추가생성
	for (int32 i = 0; i < ExpandSize; i++)
	{
		ARollingCannonProp* Cannon = Cast<ARollingCannonProp>(UGameplayStatics::GetActorOfClass(GetWorld(), ARollingCannonProp::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Forward: %s"), *Cannon->GetActorRightVector().ToString());

		ARollingBallProp* BallProp = GetWorld()->SpawnActor<ARollingBallProp>(PooledObjectClass,
			FVector::ZeroVector, Cannon->GetActorRotation());
		UE_LOG(LogTemp, Log, TEXT("Forward: %s"), *BallProp->GetActorRightVector().ToString());
		
		// 소속풀 지정
		// Queue 로 연습하기
		BallProp->SetObjectPool(this);
		BallProp->SetActive(false);
		Pool.Push(BallProp);
	}
	PoolSize += ExpandSize;
}

void UObjectPoolComponent::ReturnObject(class ARollingBallProp* ReturnObject, float RemainTime)
{
	// 사용하고 나서 다시 풀에 넣자 
	InactivePool.Remove(ReturnObject);
	Pool.Push(ReturnObject);
	
	if (GetOwner()->HasAuthority())
	{
		// 알림을 보낸다: 다시 왔음!!
		if (!FMath::IsNearlyZero(RemainTime))
		{
			UE_LOG(LogTemp, Log, TEXT("RemainTime: %f"),RemainTime);
			TWeakObjectPtr<UObjectPoolComponent> WeakThis = this;
			GetWorld()->GetTimerManager().SetTimer(ObPoolTimerHandle, FTimerDelegate::CreateLambda([WeakThis]()
			{
				if (!WeakThis.IsValid())
				{
					return;
				}
				UObjectPoolComponent* StrongThis = Cast<UObjectPoolComponent>(WeakThis.Get());
				StrongThis->OnObjectReturn.Broadcast();
			}), RemainTime, false);
		}
		else
		{
			OnObjectReturn.Broadcast();
		}
	}
}

void UObjectPoolComponent::Initialize()
{
	// 시작할 때, 단 한번만 생성
	ExpandSize = 1;
	Expand();
}
