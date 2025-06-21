// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StoryUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UStoryUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	template <typename TStoryUI>
	static TStoryUI* CreateAndShowStory(APlayerController* OwningPlayer,
	                                    TSubclassOf<TStoryUI> WidgetClass)
	{
		if (OwningPlayer && WidgetClass)
		{
			TStoryUI* Instance{CreateWidget<TStoryUI>(OwningPlayer, WidgetClass)};
			if (Instance)
			{
				Instance->AddToViewport(100);

				return Instance;
			}
		}

		return nullptr;
	}

	UFUNCTION()
	void OnNextChatTriggered();
	// 스토리 초기화
	virtual void InitializeStory();
	// 이름 및 대화 내용 업데이트
	void UpdateSpeakerAndText(const FText& FullStoryLine);
	// 초기 대사 표시 함수
	void DisplayInitialStory();
	// 대사 애니메이션
	void ChatAppearAnimation();
	void CheckNextSpeaker(const FText& FullStoryLine);
	// 대사 등장
	void TextAppearAnimation();
	void StoryEnd();

	UPROPERTY(BlueprintReadWrite)
	class ALobbyPlayerController* LobbyController;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* MomImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BabyImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ChatBoxImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Talk;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Mom;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Baby;
	
	// 애니메이션
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ChatAppear;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* TextAppear;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* MomTalk;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* BabyTalk;

	// 스토리 담을 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FText> StoryArray;

	// 현재 표시 중인 스토리 인덱스
	int32 CurrentStoryIndex;

	FString SpeakerName{};
	bool bIsSpeakerChanged{false};
	bool bIsEnd{false};
	bool bIsTextAppearing{false};
	UPROPERTY()
	UUMGSequencePlayer* SequencePlayerTextAppearing;
	FString DialogText;

	bool bIsStoryStarted{false};
	bool bIsNextTriggered{false};
};
