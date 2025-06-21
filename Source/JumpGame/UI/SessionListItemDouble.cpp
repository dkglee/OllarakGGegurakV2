// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionListItemDouble.h"

#include "SessionListItemWidget.h"
#include "Components/HorizontalBox.h"

void USessionListItemDouble::Init(class USessionListItemWidget* ListLeft,
                                  class USessionListItemWidget* ListRight)
{
	SessionListLeft = ListLeft;
	SessionListRight = ListRight;

	if (RoomContainer)
	{
		RoomContainer->ClearChildren();

		RoomContainer->AddChildToHorizontalBox(SessionListLeft);
		RoomContainer->AddChildToHorizontalBox(SessionListRight);
	}
}
