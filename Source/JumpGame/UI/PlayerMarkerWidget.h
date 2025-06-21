// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMarkerWidget.generated.h"

/**
 * 동적으로 생성
 */
UCLASS()
class JUMPGAME_API UPlayerMarkerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_PlayerMarker;
	
	// 플레이어 ID 설정
	UFUNCTION(BlueprintCallable)
	void SetPlayerID(int32 ID);
	// 플레이어 ID
	UPROPERTY(editanywhere)
	int32 PlayerID = -1;
};
