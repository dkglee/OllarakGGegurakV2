// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BottomNaviBarUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UBottomNaviBarUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Store;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_FrogPass;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Friend;

	UFUNCTION()
	void OnClickStore();
	UFUNCTION()
	void OnClickFrogPass();
	UFUNCTION()
	void OnClickFriend();

	UPROPERTY()
	bool bIsOnViewport = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UFriendsList> FriendsListClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFriendsList* FriendsList;
};
