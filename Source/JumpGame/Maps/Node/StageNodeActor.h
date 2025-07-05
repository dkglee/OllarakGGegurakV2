// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StageNodeInfo.h"
#include "GameFramework/Actor.h"
#include "StageNodeActor.generated.h"

UENUM(BlueprintType)
enum class EFieldProgressState : uint8
{
	None        UMETA(DisplayName="None"),        // 도전 기록 없음
	InProgress  UMETA(DisplayName="InProgress"),  // 도전 진행 중
	Cleared     UMETA(DisplayName="Cleared")      // 클리어 기록!
};

UCLASS()
class JUMPGAME_API AStageNodeActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStageNodeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NodeID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasField = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FieldID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText FieldName;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TArray<AStageNodeActor*> ConnectedActors; // Actor 참조

	// 런타임 등록용
	FStageNodeInfo ToNodeInfo();

public:
	// 외관
	UPROPERTY()
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UNodeResultUI> NodeResultUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* NodeResultUIComponent;

	UFUNCTION()
	void UpdateInfo(int32 Star);

	// 클리어에 따른 색깔 변동
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UMaterialInstance*> ColorTextures;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ColorIndex{};

	UFUNCTION()
	void UpdateColor(int32 NewIndex);
	UFUNCTION()
	void InitColorMat();
};
