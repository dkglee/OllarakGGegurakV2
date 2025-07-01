// Fill out your copyright notice in the Description page of Project Settings.


#include "StageMapNodeComponent.h"

#include "AIController.h"
#include "StageNodeActor.h"
#include "StageOutSign.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "JumpGame/StageSystem/FieldTableRow.h"
#include "JumpGame/StageSystem/StageSystemSubsystem.h"
#include "JumpGame/UI/StageNode/NodeInfoUI.h"
#include "Kismet/GameplayStatics.h"


class AAIController;
// Sets default values for this component's properties
UStageMapNodeComponent::UStageMapNodeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStageMapNodeComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerChar = Cast<ACharacter>(GetOwner());

	AddAllNodesFromWorld();

	StageSystem = Cast<UStageSystemSubsystem>(GetWorld()->GetGameInstance()->GetSubsystem<UStageSystemSubsystem>());
	StageOutSign = Cast<AStageOutSign>(UGameplayStatics::GetActorOfClass(GetWorld(),AStageOutSign::StaticClass()));

	NodeInfoUI = CreateWidget<UNodeInfoUI>(GetWorld(), NodeInfoUIClass);
	if (NodeInfoUI)
	{
		NodeInfoUI->AddToViewport(50);
		NodeInfoUI->SetVisibility(ESlateVisibility::Hidden);
		NodeInfoUI->SetIsEnabled(false);
	}
}

// Called every frame
void UStageMapNodeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsMoving) return;
	if (!OwnerChar) return;
	
	FVector CharLoc = OwnerChar->GetActorLocation();
	FVector TargetLoc = NodeMap[DestinationNodeID].WorldPosition;
	float XYDist = FVector2D::Distance(FVector2D(CharLoc.X, CharLoc.Y), FVector2D(TargetLoc.X, TargetLoc.Y));
	
	if (XYDist < 55.f)
	{
		UpdateStageSign(CurrentNodeID, DestinationNodeID);
		
		CurrentNodeID = DestinationNodeID;
		DestinationNodeID = -1;
		bIsMoving = false;
		
		const FStageNodeInfo* NodeInfo = GetNode(CurrentNodeID);
		if (NodeInfo)
		{
			UE_LOG(LogTemp, Warning, TEXT("[NodeInfo 있음] FieldName = %s, bHasField = %s"),
				*NodeInfo->FieldName.ToString(),
				NodeInfo->bHasField ? TEXT("true") : TEXT("false"));
			bHasData = true;
		}

		if (NodeInfo && NodeInfo->bHasField)
		{
			NodeInfoUI->SetVisibility(ESlateVisibility::HitTestInvisible);
			NodeInfoUI->SetIsEnabled(true);
			if (const FFieldTableRow* FieldData = StageSystem->GetField(NodeInfo->FieldID))
			{
				UE_LOG(LogTemp, Warning, TEXT("[UI 설정] 별 개수 = %d, 클리어 시간 = %.2f"),
					FieldData->FieldStarCount,
					FieldData->FieldClearTime);

				NodeInfoUI->SelectFieldLevel = FieldData->FieldLevel;
				NodeInfoUI->SetFieldInfo(
					NodeInfo->FieldName,
					FieldData->FieldStarCount,
					FieldData->FieldClearTime
				);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[경고] FieldData 없음. FieldName = %s"), *NodeInfo->FieldName.ToString());

				NodeInfoUI->SelectFieldLevel = NAME_None;
				NodeInfoUI->SetFieldInfo(
					FText::GetEmpty(),
					0,
					0
				);
			}
		}
		else
		{
			bHasData = false;
			NodeInfoUI->SetVisibility(ESlateVisibility::Collapsed);
			UE_LOG(LogTemp, Warning, TEXT("[UI 숨김] NodeInfo가 없거나 필드가 없음"));
		}
	}
}

void UStageMapNodeComponent::AddNode(const FStageNodeInfo& NewNodeInfo)
{
	if (!NodeMap.Contains(NewNodeInfo.NodeID))
	{
		NodeMap.Add(NewNodeInfo.NodeID, NewNodeInfo);
	}
}

void UStageMapNodeComponent::AddAllNodesFromWorld()
{
	// 월드 내 모든 AStageNodeActor 찾아오기
	TArray<AActor*> FoundNodes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStageNodeActor::StaticClass(), FoundNodes);

	for (AActor* Actor : FoundNodes)
	{
		AStageNodeActor* NodeActor = Cast<AStageNodeActor>(Actor);
		if (!NodeActor) continue;

		// AStageNodeActor → FStageNodeInfo로 변환
		FStageNodeInfo NodeInfo = NodeActor->ToNodeInfo();

		AddNode(NodeInfo);
	}
	
	InitCurrentNode();
	UE_LOG(LogTemp, Warning, TEXT("총 %d개 노드 등록"), NodeMap.Num());
}

void UStageMapNodeComponent::InitCurrentNode()
{
	if (!OwnerChar) return;

	// 캐릭터가 어디 서있는지 판단
	FVector MyLoc = OwnerChar->GetActorLocation();
	float ClosestDist = TNumericLimits<float>::Max();
	int32 ClosestID = -1;

	for (auto& Pair : NodeMap)
	{
		float Dist = FVector::Dist(Pair.Value.WorldPosition, MyLoc);
		if (Dist < ClosestDist)
		{
			ClosestDist = Dist;
			ClosestID = Pair.Key;
		}
	}

	if (ClosestID != -1)
	{
		CurrentNodeID = ClosestID;
		// DestinationNodeID = ClosestID;
	}

	UE_LOG(LogTemp, Warning, TEXT("서있는 곳 노드 번호: %d"), CurrentNodeID);
}

void UStageMapNodeComponent::UpdateStageSign(int32 CurrentNode, int32 DestinationNode)
{
	// 집 → 바깥
	if (CurrentNodeID == 0 && DestinationNodeID != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("집에서 바깥으로! 간판 보이기"));
		StageOutSign->SpawnSign(false); // 간판 보이기
	}
	// 바깥 → 집
	else if (CurrentNodeID != 0 && DestinationNodeID == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("바깥에서 집으로! 간판 숨기기"));
		StageOutSign->SpawnSign(true); // 간판 숨기기
	}
	// 그 외 (밖 → 밖 or 집 → 집)
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("간판 변화 없음"));
	}
}

void UStageMapNodeComponent::HandleKeyBoardInput(int32 Direction)
{
	int32 NextID = CurrentNodeID + Direction;

	// 현재 위치가 이동 중일 때는 위치 기준을 DestinationNodeID로 판단
	if (bIsMoving)
	{
		NextID = DestinationNodeID + Direction;
	}

	if (NodeMap.Contains(NextID))
	{
		RequestMoveTo(NextID); // 항상 호출 가능

		// 이동 시작 시 UI는 반드시 꺼야 함 (마우스 클릭 UI 중이었어도!)
		NodeInfoUI->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UStageMapNodeComponent::HandleMouseInput(FVector2D ScreenPos)
{
	int32 ClickedID = GetNodeByPosition(ScreenPos);
	if (ClickedID != -1)
	{
		RequestMoveTo(ClickedID);
	}

	// 마우스는 즉시 UI 갱신
	if (const FStageNodeInfo* NodeInfo = GetNode(ClickedID))
	{
		if (NodeInfo->bHasField)
		{
			NodeInfoUI->SetVisibility(ESlateVisibility::HitTestInvisible);
			NodeInfoUI->SetIsEnabled(true);
			
			if (const FFieldTableRow* FieldData = StageSystem->GetField(NodeInfo->FieldID))
			{
				NodeInfoUI->SelectFieldLevel = FieldData->FieldLevel;
				NodeInfoUI->SetFieldInfo(NodeInfo->FieldName, FieldData->FieldStarCount, FieldData->FieldClearTime);
			}
		}
		else
		{
			NodeInfoUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

int32 UStageMapNodeComponent::GetNodeByPosition(FVector2D ScreenPos)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	float MinDist = 100.f; // 허용 거리 (클릭 허용 범위)
	int32 ClosestID = -1;

	// 모든 노드의 월드 위치를 화면 좌표로 변환하여 비교하자
	for (auto& Pair : NodeMap)
	{
		FVector2D Projected;
		PC->ProjectWorldLocationToScreen(Pair.Value.WorldPosition, Projected);

		float Dist = FVector2D::Distance(ScreenPos, Projected);
		if (Dist < MinDist)
		{
			MinDist = Dist;
			ClosestID = Pair.Key;
		}
	}
	return ClosestID;
}

void UStageMapNodeComponent::RequestMoveTo(int32 TargetNodeID)
{
	if (TargetNodeID == CurrentNodeID) return; // 현재 위치와 동일하면 무시

	const FStageNodeInfo* TargetNode = NodeMap.Find(TargetNodeID);
	if (!TargetNode) return;
	
	if (!OwnerChar) return;

	AAIController* AICon = Cast<AAIController>(OwnerChar->GetController());
	if (!AICon) return;

	// 캡슐 너비만큼 보정
	FVector DirectionToTarget = (TargetNode->WorldPosition - OwnerChar->GetActorLocation()).GetSafeNormal(); // 방향 벡터
	float CapsuleRadius = AICon->GetPawn()->FindComponentByClass<UCapsuleComponent>()->GetScaledCapsuleRadius();
	FVector AdjustedTarget = TargetNode->WorldPosition + DirectionToTarget * CapsuleRadius;
	
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(AICon, AdjustedTarget); 
	bIsMoving = true;
	DestinationNodeID = TargetNodeID; // 도착 후 갱신
}

/*bool UStageMapNodeComponent::IsValidNodeID(int32 FromNodeID, int32 ToNodeID)
{
	const FStageNodeInfo* Node = GetNode(FromNodeID);
	return Node && Node->ConnectedNodes.Contains(ToNodeID);
}

void UStageMapNodeComponent::HandleKeyBoardInput(FVector InputDir)
{
	int32 NextID = GetNextNodeByDirection(InputDir);
	if (NextID != -1)
	{
		RequestMoveTo(NextID);
	}
}

void UStageMapNodeComponent::HandleMouseInput(FVector2D ScreenPos)
{
	int32 ClickedID = GetNodeByPosition(ScreenPos);
	if (ClickedID != -1)
	{
		RequestMoveTo(ClickedID);
	}
}


int32 UStageMapNodeComponent::GetNextNodeByDirection(FVector InputDir)
{
	const FStageNodeInfo* CurrentNode = NodeMap.Find(CurrentNodeID);
	if (!CurrentNode) return -1;
	
	float BestDot = -1.f; // 가장 높은 내적값 (가장 유사한 방향)
	int32 BestID = -1; // 선택된 노드 ID

	// 인접 노드들 중 방향이 가장 유사한 노드를 선택하자
	for (int32 NeighborID : CurrentNode->ConnectedNodes)
	{
		const FStageNodeInfo* Neighbor = NodeMap.Find(NeighborID);
		if (!Neighbor) continue;

		FVector Dir = (Neighbor->WorldPosition - CurrentNode->WorldPosition).GetSafeNormal();
		float Dot = FVector::DotProduct(InputDir, Dir);

		if (Dot > BestDot)
		{
			BestDot = Dot;
			BestID = NeighborID;
		}
	}
	return BestID;
}

TArray<int32> UStageMapNodeComponent::FindPathAStar(int32 StartNodeID, int32 EndNodeID)
{
	// StartID에서 GoalID까지의 최단 경로를 계산, NodeID 배열로 반환하자
	TMap<int32, int32> CameFrom; // 역추적용 노드 맵
	TSet<int32> Closed; // 방문한 노드 집합
	TQueue<int32> Open; // 탐색할 노드 큐

	Open.Enqueue(StartNodeID);

	while (!Open.IsEmpty())
	{
		int32 Current;
		Open.Dequeue(Current);

		// 도착 시 경로를 역추적하여 반환한다
		if (Current == EndNodeID)
		{
			TArray<int32> Path;
			int32 Step = EndNodeID;

			while (Step != StartNodeID)
			{
				Path.Insert(Step, 0);
				Step = CameFrom[Step];
			}
			Path.Insert(StartNodeID, 0);
			return Path;
		}

		Closed.Add(Current);
		const FStageNodeInfo* Node = NodeMap.Find(Current);
		if (!Node) continue;

		// 인접 노드 순회하기
		for (int32 Neighbor : Node->ConnectedNodes)
		{
			if (Closed.Contains(Neighbor)) continue;
			if (!CameFrom.Contains(Neighbor))
			{
				CameFrom.Add(Neighbor, Current);
				Open.Enqueue(Neighbor);
			}
		}
	}
	return {}; // 경로 없음
}
*/

