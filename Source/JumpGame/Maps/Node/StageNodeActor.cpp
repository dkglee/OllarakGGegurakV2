// Fill out your copyright notice in the Description page of Project Settings.


#include "StageNodeActor.h"

#include "Constraint.h"
#include "JumpGame/UI/StageNode/NodeResultUI.h"
#include "Components/WidgetComponent.h"
#include "DynamicMesh/MeshTransforms.h"
#include "JumpGame/StageSystem/StageSystemSubsystem.h"


class UStageSystemSubsystem;
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

	InitColorMat();
	if (ColorTextures.IsValidIndex(0))
	{
		StaticMesh->SetMaterial(1, ColorTextures[0]);   // 기본값 적용
	}
}

// Called when the game starts or when spawned
void AStageNodeActor::BeginPlay()
{
	Super::BeginPlay();
	
	const auto* SS = GetWorld()->GetGameInstance()->GetSubsystem<UStageSystemSubsystem>();
	EFieldProgressState St = SS ? SS->GetFieldState(FieldID)
								: EFieldProgressState::None;

	int32 ColorIdx = 0; // black
	if      (St == EFieldProgressState::InProgress) ColorIdx = 1;   // blue
	else if (St == EFieldProgressState::Cleared)    ColorIdx = 2;   // green

	UpdateColor(ColorIdx);
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

void AStageNodeActor::UpdateColor(int32 NewIndex)
{
	if (!ColorTextures.IsValidIndex(NewIndex)) return;

	ColorIndex = NewIndex;
	StaticMesh->SetMaterial(1, ColorTextures[ColorIndex]);
}

void AStageNodeActor::InitColorMat()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M0(
		TEXT("/Game/Fantastic_Village_Pack/Fantastic_Village_Pack/Materials/MI_CLR_black.MI_CLR_black"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M1(
		TEXT("/Game/Fantastic_Village_Pack/Fantastic_Village_Pack/Materials/MI_CLR_Blueg.MI_CLR_Blueg"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M2(
		TEXT("/Game/Fantastic_Village_Pack/Fantastic_Village_Pack/Materials/MI_CLR_Green.MI_CLR_Green"));

	if (M0.Succeeded()) ColorTextures.Add(M0.Object);
	if (M1.Succeeded()) ColorTextures.Add(M1.Object);
	if (M2.Succeeded()) ColorTextures.Add(M2.Object);
}

