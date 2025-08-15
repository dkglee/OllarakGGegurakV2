// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StageNodeInfo.h"
#include "Components/ActorComponent.h"
#include "StageMapNodeComponent.generated.h"

// 런타임에서 노드 데이터를 관리, 이동/경로 관련 계산 처리

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UStageMapNodeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStageMapNodeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 모든 노드의 정보 저장 (Key: NodeID)
	UPROPERTY()
	TMap<int32, FStageNodeInfo> NodeMap;
	UPROPERTY()
	int32 CurrentNodeID = -1;
	UPROPERTY()
	int32 DestinationNodeID = -1;

	// 필드 -> 노드 접근용
	UPROPERTY()
	TMap<FName, class AStageNodeActor*> FieldIDToNodeActorMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMoving = false;
	UPROPERTY()
	bool bHasData = false;

public:
	// 시작할 때 플레이어의 위치
	UPROPERTY()
	ACharacter* OwnerChar = nullptr;
	UFUNCTION()
	void InitCurrentNode();
	UFUNCTION()
	void InitAllFieldStars(); // 전체 노드 갱신
	
	// 노드 추가
	UFUNCTION()
	void AddNode(const FStageNodeInfo& NewNodeInfo);
	UFUNCTION()
	void AddAllNodesFromWorld();

	// 특정 노드의 정보 가져오자
	FStageNodeInfo* GetNode(int32 NodeID) { return NodeMap.Find(NodeID); }

	// 다음 노드 선택 | 키보드, 마우스
	UFUNCTION()
	void HandleKeyBoardInput(int32 Direction);
	UFUNCTION()
	void HandleMouseInput(FVector2D ScreenPos);
	UFUNCTION()
	int32 GetNodeByPosition(FVector2D ScreenPos);
	// int32 GetNextNodeByDirection(FVector InputDir);

	// 이동 요청
	UFUNCTION()
	void RequestMoveTo(int32 TargetNodeID);

	// 반환
	static UStageMapNodeComponent* Get(UWorld* World);

	// 앞을 보자
	UPROPERTY()
	FRotator TargetRotation = FRotator(0.f, -10.f, 0.f);
	
	UPROPERTY()
	class AStageOutSign* StageOutSign = nullptr; // 외출중 표지판
	UFUNCTION()
	void UpdateStageSign(int32 CurrentNode, int32 DestinationNode);

	UFUNCTION()
	int32 GetNodeIDByFieldID(const FName& InFieldID) const;
	
	// 이동 가능한 노드인가?
	// bool IsValidNodeID(int32 FromNodeID, int32 ToNodeID);
	// 경로탐색
	// TArray<int32> FindPathAStar(int32 StartNodeID, int32 EndNodeID);

public:
	UPROPERTY()
	class UStageSystemSubsystem* StageSystem;
	
	// 필드 UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UNodeInfoUI> NodeInfoUIClass;
	UPROPERTY(EditAnywhere)
	class UNodeInfoUI* NodeInfoUI;
};


