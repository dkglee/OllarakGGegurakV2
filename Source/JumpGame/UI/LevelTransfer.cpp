// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransfer.h"

#include "Components/Image.h"

void ULevelTransfer::FillViewport()
{
	Image_Circle->GetDynamicMaterial()->SetScalarParameterValue("Radius", 3.f);
}
