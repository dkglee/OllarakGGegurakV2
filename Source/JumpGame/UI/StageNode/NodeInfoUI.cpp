// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeInfoUI.h"

#include "Components/Button.h"

void UNodeInfoUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_GameStart->OnClicked.AddDynamic(this, &UNodeInfoUI::OnClickGameStart);
}

void UNodeInfoUI::OnClickGameStart()
{
	// 필드 이름받아서 이동
}

void UNodeInfoUI::SetFieldInfo(FName CurrentFieldName, int32 StarCount, float ClearTime)
{
}
