// Fill out your copyright notice in the Description page of Project Settings.


#include "StageNodeActor.h"

#include "Constraint.h"
#include "JumpGame/UI/StageNode/NodeResultUI.h"
#include "Components/WidgetComponent.h"
#include "DynamicMesh/MeshTransforms.h"


// Sets default values
AStageNodeActor::AStageNodeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fantastic_Village_Pack/Fantastic_Village_Pack/SM_MERGED_StaticMeshActor_272.SM_MERGED_StaticMeshActor_272"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
		StaticMesh->SetRelativeScale3D(FVector(0.45f));
		StaticMesh->SetupAttachment(RootComponent);
	}
	
	ConstructorHelpers::FClassFinder<UNodeResultUI> NodeResultUIWidget
		(TEXT("/Game/UI/StageNode/WBP_NodeClearResult.WBP_NodeClearResult_C"));
	if (NodeResultUIWidget.Succeeded())
	{
		NodeResultUIClass = NodeResultUIWidget.Class;
		NodeResultUIComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NodeResultUIComponent"));
		NodeResultUIComponent->SetupAttachment(GetRootComponent());
		NodeResultUIComponent->SetRelativeLocation(FVector(0, 90, 0));
		NodeResultUIComponent->SetRelativeRotation(FRotator(90, 90, 0));
		NodeResultUIComponent->SetRelativeScale3D(FVector(1.5,1.5,1.7));
		NodeResultUIComponent->SetWidgetClass(NodeResultUIClass);
	}
}

// Called when the game starts or when spawned
void AStageNodeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStageNodeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FStageNodeInfo AStageNodeActor::ToNodeInfo()
{
	FStageNodeInfo Info;
	Info.NodeID = NodeID;
	Info.WorldPosition = GetActorLocation();
	Info.bHasField = bHasField;
	Info.FieldID = FieldID;
	Info.FieldName = FieldName;

	/*for (AStageNodeActor* Connected : ConnectedActors)
	{
		if (Connected)
		{
			Info.ConnectedNodes.Add(Connected->NodeID);
		}
	}*/

	return Info;
}

void AStageNodeActor::UpdateInfo(int32 Star)
{
	if (!NodeResultUIComponent) return;

	// UWidgetComponent에서 실제 위젯 인스턴스 가져오기
	UNodeResultUI* UI = Cast<UNodeResultUI>(NodeResultUIComponent->GetUserWidgetObject());
	if (!UI) return;

	UI->UpdateNodeStarImages(Star);
}

