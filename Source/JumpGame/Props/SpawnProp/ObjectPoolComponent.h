// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"

// 풀로 오브젝트가 돌아오면 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectRetrurn);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UObjectPoolComponent();
	void DestoryObjectPool();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	class ARollingBallProp* GetRollingBallProp();
	void Expand();
	void ReturnObject(class ARollingBallProp* ReturnObject, float RemainTime);
	void Initialize();

	// 델리게이트 함수
	UPROPERTY(BlueprintAssignable)
	FOnObjectRetrurn OnObjectReturn;

public:
	// 생성할 액터 타입 설정
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARollingBallProp> PooledObjectClass;
	// 현재 풀의 크기
	int32 PoolSize = 0;
	// 부족할 때 늘릴 개수, 대포는 월드에 한개만
	int32 ExpandSize = 1;

private:
	// 오브젝트 들어있는 배열
	UPROPERTY()
	TArray<class ARollingBallProp*> Pool;
	UPROPERTY()
	TArray<class ARollingBallProp*> InactivePool;
	UPROPERTY()
	FTimerHandle ObPoolTimerHandle;
};
