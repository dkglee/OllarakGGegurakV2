// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

void UFrogAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Frog = Cast<AFrog>(TryGetPawnOwner());
}

void UFrogAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Frog)
	{
		return;
	}
	
	Speed = UKismetMathLibrary::VSize(Frog->GetCharacterMovement()->Velocity);
	bIsFalling = Frog->GetCharacterMovement()->IsFalling();
	bIsCrouching = Frog->bIsCrouching;
	bIsSwimming = Frog->bIsSwimming;
	
	// Pitch = Frog->Pitch;
	// Yaw = Frog->Yaw;
	// 부드럽게 보간
	Pitch = FMath::FInterpTo(Pitch, Frog->Pitch, DeltaSeconds, 10.f);
	Yaw = FMath::FInterpTo(Yaw, Frog->Yaw, DeltaSeconds, 10.f);
}