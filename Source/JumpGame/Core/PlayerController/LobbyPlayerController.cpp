// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "OnlineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "JumpGame/UI/BottomNaviBarUI.h"
#include "JumpGame/UI/FriendsList.h"
#include "JumpGame/Utils/CursorManager.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

ALobbyPlayerController::ALobbyPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFinder(
		TEXT("/Game/Characters/Input/LobbyInput/IA_NextChat.IA_NextChat"));
	if (InputActionFinder.Succeeded())
	{
		NextChatAction = InputActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputContextFinder(
		TEXT("/Game/Characters/Input/LobbyInput/IMC_Lobby.IMC_Lobby"));
	if (InputContextFinder.Succeeded())
	{
		LobbyMappingContext = InputContextFinder.Object;
	}
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())})
	{
		Subsystem->AddMappingContext(LobbyMappingContext, 0);
	}

	if (IsLocalPlayerController())
	{
		BottomNaviBarUI = CreateWidget<UBottomNaviBarUI>(GetWorld(), BottomNaviBarUIClass);
		if (BottomNaviBarUI)
		{
			BottomNaviBarUI->AddToViewport(5);
		}
	
		UCursorManager::SetCursor(this, ECursorName::LeafCursor);
		
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);  // ★ 핵심! : 이렇게 해야 마우스가 창 밖으로 안나감
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent{Cast<UEnhancedInputComponent>(InputComponent)})
	{
		if (NextChatAction)
		{
			EnhancedInputComponent->BindAction(NextChatAction, ETriggerEvent::Started, this, &ALobbyPlayerController::NextChat);
		}
	}
}

void ALobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	Super::EndPlay(EndPlayReason);
}

void ALobbyPlayerController::NextChat()
{
	OnNextChatTriggered.Broadcast();
}

int32 ALobbyPlayerController::GetPitchYawRatio(float& OutPitch, float& OutYaw)
{
	// 현재 Viewtarget 중인 카메라를 가져온다.
	AActor* ViewTarget = GetViewTarget();

	// 카메라의 트랜스폼을 구한다.
	FTransform CameraTransform = ViewTarget->GetTransform();

	// Player의 좌표를 가져온다. (월드)
	ALobbyFrog* Frog = Cast<ALobbyFrog>(UGameplayStatics::GetActorOfClass(GetWorld(),ALobbyFrog::StaticClass()));
	FVector PlayerLocation = Frog->GetActorLocation() + FVector(0, 0, 25);

	// Player의 좌표를 카메라의 로컬 좌표계로 변환한다.
	FVector InLocallyPosition = CameraTransform.InverseTransformPosition(PlayerLocation);

	// 마우스의 좌표를 가져온다.
	// 이게 근데 UI 위에 있으면 무시되려나.. 근데 그때는 뭐 상관없긴 함
	FVector2D MouseScreenPosition;
	if (!GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y))
	{
		return 0;
	}

	// 캐릭터의 위치와 ViewPort의 사이즈를 기반으로 MinusMinMax와 PlusMinMax를 구한다.
	CalculateMinMax(InLocallyPosition);

	// 마우스의 월드 좌표를 가져온다.
	FVector MouseWorldPosition;
	FVector MouseWorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, MouseScreenPosition, MouseWorldPosition, MouseWorldDirection))
	{
		return 0;
	}

	// 마우스의 좌표를 카메라의 로컬 좌표계로 변환한다.
	FVector InLocallyMousePosition = CameraTransform.InverseTransformPosition(MouseWorldPosition);

	// 두 개의 좌표로 Pitch와 Yaw의 비율을 구한다.
	OutPitch = GetPitchRatio(InLocallyPosition, InLocallyMousePosition);
	OutYaw = GetYawRatio(InLocallyPosition, InLocallyMousePosition);

	return 1;
}

float ALobbyPlayerController::GetPitchRatio(FVector InLocallyPosition, FVector InLocallyMousePosition)
{
	// InLocallyMousePosition - InLocallyPosition : 비율을 계산하기 위한 벡터
	FVector RatioVector = InLocallyMousePosition - InLocallyPosition;

	float PitchRatioFromVector = RatioVector.Z;
	float Sign = FMath::Sign(PitchRatioFromVector);

	float AbsRatio = 0;
	if (Sign > 0)
	{
		AbsRatio = FMath::Abs(PitchRatioFromVector / PlusPitchMinMax.Y);
	}
	else
	{
		AbsRatio = FMath::Abs(PitchRatioFromVector / MinusPitchMinMax.Y);
	}
	return Sign * AbsRatio;
}

float ALobbyPlayerController::GetYawRatio(FVector InLocallyPosition, FVector InLocallyMousePosition)
{
	// InLocallyMousePosition - InLocallyPosition : 비율을 계산하기 위한 벡터
	FVector RatioVector = InLocallyMousePosition - InLocallyPosition;

	float YawRatioFromVector = RatioVector.Y;
	float Sign = FMath::Sign(YawRatioFromVector);

	float AbsRatio = 0;
	if (Sign > 0)
	{
		AbsRatio = FMath::Abs(YawRatioFromVector / PlusYawMinMax.Y);
	}
	else
	{
		AbsRatio = FMath::Abs(YawRatioFromVector / MinusYawMinMax.Y);
	}
	return Sign * AbsRatio;
}

void ALobbyPlayerController::CalculateMinMax(const FVector& InLocallyPosition)
{
	// ViewPort의 사이즈를 구한다.
	FIntVector2 ViewportSize;
	GetViewportSize(ViewportSize.X, ViewportSize.Y);

	// 좌상단 좌표와 우하단 좌표를 구한다.
	FVector2D TopLeft = FVector2D(0.f, 0.f);
	FVector2D BottomRight = FVector2D(ViewportSize.X, ViewportSize.Y);

	// 위의 좌표를 기준으로 월드 좌표를 구한다.
	FVector TopLeftWorld;
	FVector TopLeftWorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, TopLeft, TopLeftWorld, TopLeftWorldDirection))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Deproject Top Left World ScreenToWorld failed"));
		return;
	}

	FVector BottomRightWorld;
	FVector BottomRightWorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, BottomRight, BottomRightWorld, BottomRightWorldDirection))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Deproject Bottom Right ScreenToWorld failed"));
		return;
	}

	// 월드 좌표를 카메라의 로컬 좌표계로 변환한다.
	FTransform CameraTransform = GetViewTarget()->GetTransform();
	FVector InLocallyTopLeft = CameraTransform.InverseTransformPosition(TopLeftWorld);
	FVector InLocallyBottomRight = CameraTransform.InverseTransformPosition(BottomRightWorld);

	// 위의 좌표로 Max와 Min을 각각 구한다.
	FVector InMinusMinMax = InLocallyTopLeft - InLocallyPosition;
	FVector InPlusMinMax = InLocallyBottomRight - InLocallyPosition;

	// InMinusMinMax로 Minus Pitch 과 Yaw을 구한다.
	// X : NOT USE | Y : Yaw | Z : Pitch
	MinusPitchMinMax.Y = InMinusMinMax.Z;
	MinusYawMinMax.Y = InMinusMinMax.Y;

	// InPlusMinMax로 Plus Pitch 과 Yaw을 구한다.
	// X : NOT USE | Y : Yaw | Z : Pitch
	PlusPitchMinMax.Y = InPlusMinMax.Z;
	PlusYawMinMax.Y = InPlusMinMax.Y;
}

void ALobbyPlayerController::RequestFriendList()
{
	/* 로컬 클라이언트에서 호출됐으면 바로 실행,
	   서버에서 강제로 부른 경우엔 오너 클라에 시키도록 분기 */
	if (IsLocalController())
	{
		FetchAndSendFriendList();  // 로컬일 때 바로 실행
	}
	else
	{
		ClientRPC_RequestFriendList();   // 서버라면 오너 클라에 시킴
	}
}

void ALobbyPlayerController::FetchAndSendFriendList()
{
	IOnlineFriendsPtr Friends = IOnlineSubsystem::Get()->GetFriendsInterface();
	IOnlineIdentityPtr Identity = IOnlineSubsystem::Get()->GetIdentityInterface();
	if (!Friends.IsValid() || !Identity.IsValid()) return;

	const int32 LocalUserNum = 0;              // 로컬 PC에 로그인된 첫 번째 계정
	const FString ListName = EFriendsLists::ToString(EFriendsLists::Default);

	Friends->ReadFriendsList(LocalUserNum, ListName,
		FOnReadFriendsListComplete::CreateLambda(
		[WeakThis = TWeakObjectPtr<ALobbyPlayerController>(this),
		 LocalUserNum, ListName]
		(int32 /*UserNum*/, bool bSuccess, const FString& /*List*/, const FString& /*Err*/)
	{
		if (!WeakThis.IsValid()) return;

		TArray<FSteamFriendData> Tmp;
		TArray<TSharedRef<FOnlineFriend>> Raw;

		if (bSuccess &&
			IOnlineSubsystem::Get()->GetFriendsInterface()->GetFriendsList(
				LocalUserNum, ListName, Raw))
		{
			for (int32 Index = 0; Index < Raw.Num(); ++Index)
			{
				const auto& F = Raw[Index];
				FSteamFriendData D;
				D.DisplayName = F->GetDisplayName();
				D.SteamId     = F->GetUserId()->ToString();
				D.bIsOnline   = F->GetPresence().bIsOnline;
				D.FriendIdx   = Index;
				Tmp.Add(D);
			}
		}

		/* ─── 게임 스레드로 이동 후 RPC 호출 ─── */
		AsyncTask(ENamedThreads::GameThread,
			[WeakThis, List = MoveTemp(Tmp)]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->ServerRPC_SendFriendList(List);
			}
		});
	}));
}

void ALobbyPlayerController::ClientRPC_RequestFriendList_Implementation()
{
	FetchAndSendFriendList();
}

/** 클라 → 서버 */
void ALobbyPlayerController::ServerRPC_SendFriendList_Implementation(
		const TArray<FSteamFriendData>& FriendList)
{
	MyFriendList = FriendList;

	/* 보낸 사람 ID 한 번만 확보 */
	const FUniqueNetIdRepl SenderId =
		PlayerState ? PlayerState->GetUniqueId() : FUniqueNetIdRepl();

	/* 보낸 사람(=this)에게만 목록 회신 */
	ClientRPC_ReceiveFriendList(PlayerState->GetUniqueId(), FriendList);
}

/** 서버 → 클라 */
void ALobbyPlayerController::ClientRPC_ReceiveFriendList_Implementation(
		const FUniqueNetIdRepl& FromPlayer,
		const TArray<FSteamFriendData>& FriendList)
{
	/* 내 UI는 내 것만 보고 싶다 */
	if (FromPlayer != PlayerState->GetUniqueId())
	{
		return;                     // 남의 리스트면 무시
	}
	
	// GameInstance에 친구 목록 전달
	if (UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance()))
	{
		GI->SetFilteredFriendList(FriendList);
	}

	OnFriendListUpdated.Broadcast(FriendList);
	
	// Recursive_ReceiveFriendList(FromPlayer, FriendList);
}


void ALobbyPlayerController::Recursive_ReceiveFriendList(const FUniqueNetIdRepl& FromPlayer,
	const TArray<FSteamFriendData>& FriendList)
{
	if (!PlayerState)
	{
		TWeakObjectPtr<ALobbyPlayerController> WeakThis{this};
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakThis, FromPlayer, FriendList]()
		{
			if (!WeakThis.IsValid())
			{
				return;
			}
			ALobbyPlayerController* StrongThis{WeakThis.Get()};
			
			StrongThis->Recursive_ReceiveFriendList(FromPlayer, FriendList);
		}, 0.2f, false); // 0.1초 후에 다시 시도
		return ;
	}
	
}