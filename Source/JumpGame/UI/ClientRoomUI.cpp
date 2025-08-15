// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientRoomUI.h"

#include "CreditUI.h"
#include "CustomGameUI.h"
#include "GameQuitUI.h"
#include "GameSettingUI.h"
#include "ScoreCollectUI.h"
#include "SessionListItemWidget.h"
#include "SessionListItemDouble.h"
#include "Components/AudioComponent.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "JumpGame/Maps/Node/NodeTutorial.h"
#include "JumpGame/Maps/Node/StageMapNodeComponent.h"
#include "JumpGame/StageSystem/StageSystemSubsystem.h"
#include "Sound/SoundCue.h"
#include "StageNode/NodeInfoUI.h"
#include "UICam/LobbyCameraComp.h"


UClientRoomUI::UClientRoomUI(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<USoundCue> SoundObject
	(TEXT("/Game/Sounds/Ques/Lobby_Cue.Lobby_Cue"));
	if (SoundObject.Succeeded())
	{
		LobbySoundCue = SoundObject.Object;
	}
}

void UClientRoomUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// 마우스 활성화
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	Frog = Cast<ALobbyFrog>(UGameplayStatics::GetActorOfClass(GetWorld(),ALobbyFrog::StaticClass()));
	CameraComp = Cast<ULobbyCameraComp>(Frog->CameraComp);

	// WidgetSwitcher (0)
	// 메인 화면 버튼 클릭
	Btn_GoFind->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoStartStageGame);
	Btn_GoCreateMap->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoCreateMap);
	Btn_GoStoryMenu->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoStoryMenu);
	Btn_GoSettings->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoSettings);
	Btn_GoCredit->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoCredit);
	Btn_GoGameEnd->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoGameEnd);

	// WidgetSwitcher (1)
	// 방 만들기 화면 버튼 클릭
	Btn_GoCreate->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoCreateRoom);
	Btn_Create->OnClicked.AddDynamic(this, &UClientRoomUI::CreateSession);
	Btn_BackFromCreate->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickBack);
	// 슬라이더 값 변경될때 호출되는 함수
	Slider_PlayerCnt->OnValueChanged.AddDynamic(this, &UClientRoomUI::OnValueChanged);
	// 비밀번호 설정 함수
	CheckBox_Lock->OnCheckStateChanged.AddDynamic(this, &UClientRoomUI::OnClickCheckBox);
	
	// session 검색 관련 ui 버튼 클릭
	Btn_Find->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickFind);
	Btn_BackFromFind->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickBackFromFind);
	GI->OnFindComplete.BindUObject(this, &UClientRoomUI::OnFindComplete);

	// WidgetSwitcher (2)
	Btn_GoLobby->OnClicked.AddDynamic(this, &UClientRoomUI::UClientRoomUI::OnClickGoLobby);

	// 초기화
	InitRoomListPool();
	InitTutorialNode();

	// UI 만들기
	/*StoryMenuUI = CreateWidget<UStoryMenuUI>(GetWorld(), StoryMenuUIClass);
	if (StoryMenuUI)
	{
		StoryMenuUI->OnClickBackToLobby.AddDynamic(this, &UClientRoomUI::SetVisibleMain);
	}*/
	CustomGameUI = CreateWidget<UCustomGameUI>(GetWorld(), CustomGameUIClass);
	if (CustomGameUI)
	{
		CustomGameUI->OnClickBackToLobby.AddDynamic(this, &UClientRoomUI::SetVisibleMain);
	}

	GameSettingUI = CreateWidget<UGameSettingUI>(GetWorld(), GameSettingUIClass);
	if (GameSettingUI)
	{
		GameSettingUI->AddToViewport(10);
		GameSettingUI->SetVisibility(ESlateVisibility::Collapsed);
	}

	CreditUI = CreateWidget<UCreditUI>(GetWorld(), CreditUIClass);
	if (CreditUI)
	{
		CreditUI->OnClickBackToLobbyFromCredit.AddDynamic(this, &UClientRoomUI::SetVisibleMain);
	}
	
	LobbyAudio = UGameplayStatics::CreateSound2D(GetWorld(), LobbySoundCue);
}

void UClientRoomUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (LobbyAudio)
	{
		LobbyAudio->Play();
	}
	else
	{
		LobbyAudio = UGameplayStatics::CreateSound2D(GetWorld(), LobbySoundCue);
		LobbyAudio->Play();
	}
}

void UClientRoomUI::SetVisibleMain()
{
	bIsVisible = CanvasMain->GetVisibility();
	if (bIsVisible == ESlateVisibility::Hidden)
	{
		CanvasMain->SetVisibility(ESlateVisibility::Visible);
	}
}

void UClientRoomUI::OnClickGoLobby()
{
	// 스테이지 -> 로비로 돌아가자
	if (Frog)
	{
		// 움직이는 중이면 반응하지말자
		if (Frog->StageMapNodeComponent->bIsMoving == true) return;
	}
	
	WidgetSwitcher->SetActiveWidgetIndex(0);
	CameraComp->SetViewTarget(ECameraState::Main);
	CanvasMain->SetVisibility(ESlateVisibility::Visible);
	SetTutorialNode(false);

	// 로비 애니메이션 전환
	if (Frog)
	{
		Frog->UpdateAnimation(ELobbyCharacterState::InLobby);
		Frog->StageMapNodeComponent->NodeInfoUI->SetVisibility(ESlateVisibility::Collapsed);
		Frog->SetActorRotation(FRotator(0, -10, 0));
	}
}

void UClientRoomUI::InitTutorialNode()
{
	AllTutorialActors.Empty();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANodeTutorial::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ANodeTutorial* Node = Cast<ANodeTutorial>(Actor))
		{
			AllTutorialActors.Add(Node);
		}
	}
}

void UClientRoomUI::SetTutorialNode(bool IsVisible)
{
	for (AActor* NodeActor : AllTutorialActors)
	{
		if (!NodeActor) continue;
		NodeActor->SetActorHiddenInGame(!IsVisible);
	}
}

void UClientRoomUI::OnClickGoFindRoom()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
	//CameraComp->SetViewTarget();
	CanvasMain->SetVisibility(ESlateVisibility::Hidden);
	
	// 세션 검색 화면 넘어갈때 자동으로 한번은 세션을 검색해주자
	OnClickFind();
}

void UClientRoomUI::OnClickGoStartStageGame()
{
	// 스테이지로 이동하자
	WidgetSwitcher->SetActiveWidgetIndex(2);
	CameraComp->SetViewTarget(ECameraState::Stage);
	CanvasMain->SetVisibility(ESlateVisibility::Hidden);
	SetTutorialNode(true);

	// 스테이지 애니메이션 전환
	if (Frog)
	{
		Frog->UpdateAnimation(ELobbyCharacterState::OnStageMap);
		if (Frog->StageMapNodeComponent->NodeInfoUI->GetVisibility() == ESlateVisibility::Collapsed && Frog->StageMapNodeComponent->bHasData == true)
		{
			Frog->StageMapNodeComponent->NodeInfoUI->SetVisibility(ESlateVisibility::Visible);
		}
	}

	UJumpGameInstance* JumpGI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	if (!JumpGI) return;
	UStageSystemSubsystem* StageSystem = JumpGI->GetSubsystem<UStageSystemSubsystem>();
	if (!StageSystem) return;
	
	GameScoreUI->UpdateCurrentScore(StageSystem->GetTotalStarCountByStageID(StageSystem->GetChosenStage()));
	GameScoreUI->UpdateTotalScore(StageSystem->GetTotalFieldCountByStageID(StageSystem->GetChosenStage()) * 3);
}

void UClientRoomUI::OnClickGoCreateMap()
{
	//UGameplayStatics::OpenLevel(GetWorld(),TEXT("MapEditorLevel"));
}

void UClientRoomUI::OnClickGoStoryMenu()
{
	// 스토리 UI를 커스텀 게임 UI로 변경함
	CameraComp->SetViewTarget(ECameraState::Sub);
	CanvasMain->SetVisibility(ESlateVisibility::Hidden);
	if (CustomGameUI)
	{
		CustomGameUI->AddToViewport();
	}
	SetTutorialNode(false);
	
	/*CameraComp->SetViewTarget(ECameraState::Sub);
	CanvasMain->SetVisibility(ESlateVisibility::Hidden);
	if (StoryMenuUI)
	{
		StoryMenuUI->AddToViewport();
	}*/
}

void UClientRoomUI::OnClickGoSettings()
{
	GameSettingUI->PlaySettingAnim(true);
}

void UClientRoomUI::OnClickGoCredit()
{
	CameraComp->SetViewTarget(ECameraState::Sub);
	CanvasMain->SetVisibility(ESlateVisibility::Hidden);
	SetTutorialNode(false);
	
	if (CreditUI)
	{
		CreditUI->AddToViewport();
		CreditUI->CreditCanvas->SetRenderOpacity(1);
	}
}

void UClientRoomUI::OnClickGoGameEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("OnClickGoGameEnd"));
	if (GameSettingUI->GameQuitUI)
	{
		GameSettingUI->GameQuitUI->PlayQuitAnim(true);
		UE_LOG(LogTemp, Warning, TEXT("PlayQuitAnim"));
	}
}

void UClientRoomUI::OnClickGoCreateRoom()
{
	// UI를 팝업으로 띄우자
	CanvasCreateRoom->SetVisibility(ESlateVisibility::Visible);
}

void UClientRoomUI::CreateSession()
{
	// 방 제목 가져오기
	FString RoomName = Edit_RoomName->GetText().ToString();
	// 만약 방 제목 없으면 나가자
	if (RoomName.IsEmpty()) return;
	
	// 인원 수
	int32 PlayerCnt = Slider_PlayerCnt->GetValue();
	FString Password = Edit_Password->GetText().ToString();
	
	// 생성
	GI->CreateMySession(RoomName, PlayerCnt, Password);
}

void UClientRoomUI::OnClickCheckBox(bool bIsChecked)
{
	Edit_Password->SetIsEnabled(bIsChecked);
}

void UClientRoomUI::OnValueChanged(float Value)
{
	// value값을 text값으로 변경후 세팅
	Text_PlayerCnt->SetText(FText::AsNumber(Value));
}

void UClientRoomUI::OnClickFind()
{
	// 스크롤 룸리스트의 자식들 다 지우자 (초기화, 중복 방지)
	AllFoundRooms.Empty();
	UpdateRoomList(AllFoundRooms);
	
	// 그 다음에 검색을 시작
	GI->FindOtherSession();

	// 찾는 도중엔 버튼을 비활성화
	// 검색 버튼 문구 바꿔주고
	Text_BtnFind->SetText(FText::FromString(TEXT("검색 중...")));
	// 검색 버튼을 비활성화
	Btn_Find->SetIsEnabled(false);
}

void UClientRoomUI::OnClickBack()
{
	// 팝업UI 없애기
	CanvasCreateRoom->SetVisibility(ESlateVisibility::Hidden);
}

void UClientRoomUI::OnFindComplete(const FRoomData& Data)
{
	// 만약에 idx가 -1이면
	if (Data.RoomID == -1)
	{
		// 검색 버튼 내용 다시 find로
		Text_BtnFind->SetText(FText::FromString(TEXT("새로고침")));
		// 검색 버튼을 활성화
		Btn_Find->SetIsEnabled(true);

		UpdateRoomList(AllFoundRooms);
	}
	// 그렇지 않으면
	else
	{
		AllFoundRooms.Add(Data);
	}
}

void UClientRoomUI::OnClickBackFromFind()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
	CameraComp->SetViewTarget(ECameraState::Main);
	CanvasMain->SetVisibility(ESlateVisibility::Visible);
}

void UClientRoomUI::InitRoomListPool()
{
	Scroll_RoomList->ClearChildren();
	RoomListPool.Empty();
	RoomDoubleWidgets.Empty();

	for (int32 i = 0; i < MaxRoomCount; i += 2)
	{
		// RoomDoubleWidget을 생성하자
		USessionListItemDouble* DoubleWidget = CreateWidget<USessionListItemDouble>(GetWorld(), SessionItemDoubleFactory);

		USessionListItemWidget* Left = CreateWidget<USessionListItemWidget>(GetWorld(), SessionItemFactory);
		USessionListItemWidget* Right = CreateWidget<USessionListItemWidget>(GetWorld(), SessionItemFactory);

		DoubleWidget->Init(Left, Right);

		// 배열에 저장
		RoomListPool.Add(Left);
		RoomListPool.Add(Right);
		RoomDoubleWidgets.Add(DoubleWidget);

		// scroll box에 추가
		Scroll_RoomList->AddChild(DoubleWidget);
	}
}

void UClientRoomUI::UpdateRoomList(const TArray<FRoomData>& RoomDataArray)
{
	const int32 NumRooms = RoomDataArray.Num();
	const int32 NumRowsToShow = FMath::DivideAndRoundUp(NumRooms, 2);

	// 전체 줄을 순회하면서 Visible/Collapsed 처리
	for (int32 Row = 0; Row < RoomDoubleWidgets.Num(); ++Row)
	{
		USessionListItemDouble* RowWidget = RoomDoubleWidgets[Row];
		const int32 LeftIdx = Row * 2;
		const int32 RightIdx = Row * 2 + 1;

		if (Row < NumRowsToShow)
		{
			// 왼쪽 방
			if (RoomListPool.IsValidIndex(LeftIdx) && RoomDataArray.IsValidIndex(LeftIdx))
			{
				RoomListPool[LeftIdx]->SetRoomInfo(RoomDataArray[LeftIdx]);
				RoomListPool[LeftIdx]->SetVisibility(ESlateVisibility::Visible);
			}

			// 오른쪽 방
			if (RoomListPool.IsValidIndex(RightIdx) && RoomDataArray.IsValidIndex(RightIdx))
			{
				RoomListPool[RightIdx]->SetRoomInfo(RoomDataArray[RightIdx]);
				RoomListPool[RightIdx]->SetVisibility(ESlateVisibility::Visible);
			}
			else if (RoomListPool.IsValidIndex(RightIdx))
			{
				RoomListPool[RightIdx]->SetVisibility(ESlateVisibility::Collapsed);
			}

			RowWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			RowWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
