// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpGameInstance.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "JumpGame/Core/GameState/LobbyGameState.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "HAL/PlatformProcess.h"
#include "Async/Async.h"
#include "Misc/Paths.h"
#include "HAL/FileManagerGeneric.h"

void UJumpGameInstance::Init()
{
	Super::Init();

	// 현재 사용하는 서브시스템을 가져오자
	IOnlineSubsystem* Subsys = IOnlineSubsystem::Get();
	if (Subsys)
	{
		SessionInterface = Subsys->GetSessionInterface();

		// 세션 생성 성공시 호출되는 함수 등록
		SessionInterface->OnCreateSessionCompleteDelegates.
		                  AddUObject(this, &UJumpGameInstance::OnCreateSessionComplete);
		// 세션 검색 성공시 호출되는 함수 등록
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnFindSessionComplete);
		// 세션 참여 성공시 호출되는 함수 등록
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnJoinSessionComplete);
		// 초대
		SessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UJumpGameInstance::OnSessionInviteAccepted));
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UJumpGameInstance::OnJoinSessionComplete));
	
		// 세션 파괴 성공시 호출되는 함수 등록
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnDestroySessionComplete);
		// 세션 비정상 종료 감지시 호출되는 함수 등록
		SessionInterface->OnSessionFailureDelegates.AddUObject(this, &UJumpGameInstance::OnFailureSessionDetected);
		// 세션 끝냈을 시 호출되는 함수 등록
		SessionInterface->OnEndSessionCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnEndSessionComplete);
	}

	if (bIsRunEyeScript)
	{
		RunEyeTrackingScript();
	}
}

void UJumpGameInstance::CreateMySession(FString DisplayName, int32 PlayerCount, const FString& Password)
{
	// 세션을 만들기 위한 옵션을 설정
	FOnlineSessionSettings SessionSettings;
	// Lan 사용 여부
	FName subsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("서브시스템 이름: %s"), *subsystemName.ToString())
	SessionSettings.bIsLANMatch = subsystemName.IsEqual(FName(TEXT("NULL")));

	// 로비 사용할지 여부
	SessionSettings.bUseLobbiesIfAvailable = true;
	// 친구 상태를 확인할 수 있는지 (게임중/로그아웃 등등 공개할건지) 여부
	SessionSettings.bUsesPresence = true;
	// 세션 실행중 들어오기 허용 여부
	SessionSettings.bAllowJoinInProgress = true;
	// 세션 검색을 허용할 지 여부
	SessionSettings.bShouldAdvertise = true;
	// 세션 최대 인원 설정
	SessionSettings.NumPublicConnections = PlayerCount;
	
	// 커스텀 정보 (세션이름)
	// base64로 인코딩
	DisplayName = StringBase64Encode(DisplayName);
	FName EncodedName = FName(DisplayName);
	SessionSettings.Set(FName(TEXT("DP_NAME")), DisplayName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	// 세션 이름 저장
	CurrentSessionName = EncodedName;
	// 잠금 여부
	bool bIsLocked = !Password.IsEmpty();
	SessionSettings.Set(FName(TEXT("IS_LOCKED")), bIsLocked ? 1 : 0, EOnlineDataAdvertisementType::ViaOnlineService);
	// 세션 파괴 여부 판단하기
	SessionSettings.Set(FName("bIsEnded"), false, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	// 세션 생성
	SessionInterface->CreateSession(0, FName(DisplayName), SessionSettings);
}

void UJumpGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 성공"), *SessionName.ToString());

		IOnlineSessionPtr Sessions = IOnlineSubsystem::Get()->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->StartSession(SessionName);   // 상태 → InProgress
		}
		// 대기방으로 이동
		GetWorld()->ServerTravel(TEXT("/Game/Maps/WaitRoomLevel?listen"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 실패"), *SessionName.ToString());
	}
}

void UJumpGameInstance::FindOtherSession()
{
	// sessionSearch 만들자
	SessionSearch = MakeShared<FOnlineSessionSearch>();

	// Lan 사용 여부
	FName subsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("서브시스템 이름: %s"), *subsystemName.ToString())
	SessionSearch->bIsLanQuery = subsystemName.IsEqual(FName(TEXT("NULL")));

	// 어떤 옵션을 기준으로 검색할건지
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	// 검색 갯수
	SessionSearch->MaxSearchResults = 100;

	// 위 설정들을 가지고 세션 검색해주세요
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UJumpGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	FFastLogger::LogFile(TEXT("FindSessionComplete.txt"), TEXT("Session Find Complete: %d"), (int32)bWasSuccessful);
	if (bWasSuccessful && SessionSearch && SessionSearch->SearchResults.Num() != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("세션 검색 성공!"));
		// 검색된 세션 결과들
		auto results = SessionSearch->SearchResults;
		for (int32 i = 0; i < results.Num(); i++)
		{
			FString TempName;
			results[i].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), TempName);
			FFastLogger::LogFile(TEXT("FindSessionComplete.txt"), TEXT("Session [%d] Name: %s"), i, TempName.IsEmpty() ? TEXT("Unknown") : *TempName);
			// 파괴된 세션 필터
			bool bIsEnded = true; // 기본 true로 두고, Get 실패해도 거르도록
			results[i].Session.SessionSettings.Get(FName("bIsEnded"), bIsEnded);
			if (bIsEnded)
			{
				UE_LOG(LogTemp, Warning, TEXT("세션 [%d]는 이미 종료된 세션입니다. 자동으로 제외됩니다."), i);
				FFastLogger::LogFile(TEXT("FindSessionComplete.txt"), TEXT("Session [%d] is Already Destroyed"), i);
				continue;
			}
			
			FRoomData Room;

			// 룸 인덱스
			Room.RoomID = i;
			// 룸 이름
			FString displayName;
			if (results[i].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), displayName))
			{
				Room.RoomName = StringBase64Decode(displayName);
			}
			// 인원수
			int32 MaxPlayers = results[i].Session.SessionSettings.NumPublicConnections;
			int32 OpenSlots = results[i].Session.NumOpenPublicConnections;

			int32 CurrentPlayers = MaxPlayers - OpenSlots;

			if (CurrentPlayers <= 0)
			{
				CurrentPlayers = 1;
			}

			Room.PlayerCount = CurrentPlayers;
			Room.MaxPlayer = MaxPlayers;
			
			UE_LOG(LogTemp, Warning, TEXT("Open: %d / Max: %d"), OpenSlots, MaxPlayers);
			
			// 맵 이름도 세션 설정에 있다면 가져오기
			FString MapName;
			if (results[i].Session.SessionSettings.Get(FName(TEXT("MAP_NAME")), MapName))
			{
				Room.MapName = MapName;
			}
			else
			{
				Room.MapName = TEXT("Unknown");
			}

			UE_LOG(LogTemp, Warning, TEXT("세션 - %d, 이름: %s (%d/%d)"), i, *Room.RoomName, CurrentPlayers, MaxPlayers);

			OnFindComplete.ExecuteIfBound(Room);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("세션 검색 실패"));
	}

	// 검색 끝났다는 알림
	FRoomData EndFlag;
	EndFlag.RoomID = -1;
	OnFindComplete.ExecuteIfBound(EndFlag);
}

void UJumpGameInstance::JoinOtherSession(int32 SessionIdx)
{
	// 검색된 세션 결과들
	auto results = SessionSearch->SearchResults;
	if (results.Num() == 0)
		return;

	// 세션 이름을 가져오자 (일단 0번째)
	FString displayName;
	// 5.5 이슈 해결 (이 값이 자동으로 false되니까 다시 변환해주기)
	results[SessionIdx].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	results[SessionIdx].Session.SessionSettings.bUsesPresence = true;

	// 수업시간엔 안나왔지만 추가로 설정
	results[SessionIdx].Session.SessionSettings.bAllowJoinInProgress = true;
	results[SessionIdx].Session.SessionSettings.bShouldAdvertise = true;

	results[SessionIdx].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), displayName);

	// 세션참여
	SessionInterface->JoinSession(0, FName(displayName), results[SessionIdx]);
}

void UJumpGameInstance::OnJoinSessionComplete(FName SessionName,
                                              EOnJoinSessionCompleteResult::Type Result)
{
	// 만약에 참여 성공했다면
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Join 성공! : %d"), (int32)Result);
		FString url;
		if (SessionInterface->GetResolvedConnectString(SessionName, url))
		{
			UE_LOG(LogTemp, Warning, TEXT("Resolved URL: %s"), *url);
			APlayerController* pc = GetWorld()->GetFirstPlayerController();
			pc->ClientTravel(url, TRAVEL_Absolute);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GetResolvedConnectString 실패!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Join 실패! 이유: %d"), (int32)Result);
	}
}

void UJumpGameInstance::LeaveSession(bool bDestroySession)
{
	// APlayerController* PC = GetWorld()->GetFirstPlayerController();
	// IOnlineSubsystem* Subsys = IOnlineSubsystem::Get();
	// if (Subsys)
	// {
	// 	SessionInterface = Subsys->GetSessionInterface();
	// 	if (SessionInterface.IsValid() && bDestroySession)
	// 	{
	// 		// 생성된 세션의 이름을 가져오자
	// 		FName SessionName = CurrentSessionName;
	//
	// 		FNamedOnlineSession* Session = SessionInterface->GetNamedSession(SessionName);
	// 		if (Session)
	// 		{
	// 			// bIsEnded = true로 설정
	// 			Session->SessionSettings.Set(FName("bIsEnded"), true, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//
	// 			// 업데이트 반영
	// 			SessionInterface->UpdateSession(SessionName, Session->SessionSettings, true);
	// 		}
	// 		
	// 		// 서버가 방을 떠나면, 세션을 삭제하자
	// 		if (PC->HasAuthority())
	// 		{
	// 			// 먼저 세션을 끝내고
	// 			SessionInterface->EndSession(SessionName);
	// 			// 그 다음 파괴
	// 			SessionInterface->DestroySession(SessionName);
	// 		}
	// 		else
	// 		{
	// 			// 클라이언트가 방을 떠나면, 그 클라이언트만 세션에서 나오자
	// 			SessionInterface->EndSession(SessionName);
	// 		}
	// 	}
	// }
	// FFastLogger::LogConsole(TEXT("UJumpGameInstance::LeaveSession, Server Leaving Session"));

	IOnlineSessionPtr Sessions = IOnlineSubsystem::Get()->GetSessionInterface();
	if (Sessions.IsValid())
	{
		Sessions->EndSession(CurrentSessionName);   // 비동기
	}
}

void UJumpGameInstance::OnDestroySessionComplete(FName Name, bool bArg)
{
	// FFastLogger::LogFile(TEXT("OnDestroySessionComplete"), TEXT("OnDestroySessionComplete Begin Function"));
	// // 만약에 파괴 성공했다면
	// if (bArg == true)
	// {
	// 	FFastLogger::LogScreen(FColor::Red, TEXT("UJumpGameInstance::OnDestroySessionComplete"));
	//
	// 	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	// 	if (PC)
	// 	{
	// 		// 서버라면, 로비로 이동하자
	// 		if (PC->HasAuthority())
	// 		{
	// 			FFastLogger::LogScreen(FColor::Red, TEXT("서버 이동@@@@@"));
	// 			GetWorld()->ServerTravel(TEXT("/Game/Maps/ClientRoomLevel?closed"));
	// 		}
	// 	}
	// }
	// FFastLogger::LogFile(TEXT("OnDestroySessionComplete"), TEXT("OnDestroySessionComplete End Function"));
	// UWorld* World = GetWorld();
	// if (World && World->GetNetDriver())
	// {
	// 	FFastLogger::LogFile(TEXT("DestroySession.txt"), TEXT("Clear Net Driver"));
	// 	World->ClearNetDriver(World->GetNetDriver());
	// }
	// else
	// {
	// 	FFastLogger::LogFile(TEXT("DestroySession.txt"), TEXT("Failed to Clear Net Driver"));
	// }
	
	FFastLogger::LogConsole(TEXT("DestroySession %s"), bArg ? TEXT("Success") : TEXT("Fail"));
	FFastLogger::LogScreen(FColor::Red, TEXT("DestroySession %s"), bArg ? TEXT("Success") : TEXT("Fail"));
	FFastLogger::LogFile(TEXT("DestroySession.txt"), TEXT("DestroySession %s"), bArg ? TEXT("Success") : TEXT("Fail"));
}

void UJumpGameInstance::OnFailureSessionDetected(const FUniqueNetId& UniqueNetId,
	ESessionFailure::Type Arg)
{
	FFastLogger::LogScreen(FColor::Orange, TEXT("OnFailure"));
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, FName(TEXT("/Game/Maps/ClientRoomLevel")));
	}
}

void UJumpGameInstance::OnEndSessionComplete(FName Name, bool bArg)
{
	// FFastLogger::LogScreen(FColor::Red, TEXT("OnEndSessionComplete@@@@@@"));
	// UWorld* World = GetWorld();
	// if (World)
	// {
	// 	// 서버 클라 구분없이 호출한 측에서 독립적으로 레벨 변경
	// 	// UGameplayStatics::OpenLevel(World, FName(TEXT("/Game/Maps/ClientRoomLevel")));
	// }

	// 성공 여부와 무관하게 클라·서버 모두 로비로 이동
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/ClientRoomLevel"));

	// 세션 파괴는 백그라운드로
	IOnlineSessionPtr Sessions = IOnlineSubsystem::Get()->GetSessionInterface();
	if (Sessions.IsValid())
	{
		Sessions->DestroySession(CurrentSessionName);
	}
	FFastLogger::LogFile(TEXT("EndSession.txt"), TEXT("EndSession %s"), bArg ? TEXT("Success") : TEXT("Fail"));
}

FString UJumpGameInstance::StringBase64Encode(FString Str)
{
	// Set 할 때 : FString -> UTF8 -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*Str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)utf8String.c_str(), utf8String.length());

	return FBase64::Encode(arrayData);
}

FString UJumpGameInstance::StringBase64Decode(FString Str)
{
	// Get 할 때 : base64 를 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(Str, arrayData);
	std::string utf8String((char*)arrayData.GetData(), arrayData.Num());

	return UTF8_TO_TCHAR(utf8String.c_str());
}

void UJumpGameInstance::GetSteamFriends()
{
	IOnlineSubsystem* Subsys = IOnlineSubsystem::Get();
	if (!Subsys)
	{
		UE_LOG(LogTemp, Error, TEXT("OnlineSubsystem is NULL"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem Name: %s"), *Subsys->GetSubsystemName().ToString());

	IOnlineFriendsPtr FriendsInterface = Subsys->GetFriendsInterface();
	if (!FriendsInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("FriendsInterface invalid"));
		return;
	}

	IOnlineIdentityPtr Identity = Subsys->GetIdentityInterface();
	if (!Identity.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("IdentityInterface invalid"));
		return;
	}

	LocalUserNumber = 0;

	if (Identity->GetLoginStatus(LocalUserNumber) != ELoginStatus::LoggedIn)
	{
		UE_LOG(LogTemp, Error, TEXT("Steam에 로그인되어 있지 않음"));
		return;
	}

	TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalUserNumber);
	if (!UserId.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("유저 ID를 가져올 수 없음"));
		return;
	}

	FriendsInterface->ReadFriendsList(
		LocalUserNumber,
		EFriendsLists::ToString(EFriendsLists::Default),
		FOnReadFriendsListComplete::CreateUObject(this, &UJumpGameInstance::OnReadFriendsComplete));
}

void UJumpGameInstance::OnReadFriendsComplete(int32 LocalPlayer, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	IOnlineSubsystem* Subsys = IOnlineSubsystem::Get();
	if (!Subsys) return;

	IOnlineFriendsPtr FriendsInterface = Subsys->GetFriendsInterface();
	if (!FriendsInterface.IsValid()) return;

	TArray<TSharedRef<FOnlineFriend>> FriendList;
	if (!FriendsInterface->GetFriendsList(LocalPlayer, ListName, FriendList)) return;

	TArray<FSteamFriendData> OnlineFriends;
	TArray<FSteamFriendData> OfflineFriends;

	// 테스트 중이므로 Spacewar AppID 사용
	const int32 MyAppId = 480; 

	for (const TSharedRef<FOnlineFriend>& Friend : FriendList)
    {
    	const FOnlineUserPresence& Presence = Friend->GetPresence();

    	FSteamFriendData Data;
    	Data.DisplayName = Friend->GetDisplayName();
    	Data.SteamId = Friend->GetUserId()->ToString();
    	Data.bIsOnline = Presence.bIsOnline;

		UE_LOG(LogTemp, Log, TEXT("친구: %s | 상태: %s"), 
		*Data.DisplayName,
		Data.bIsOnline ? TEXT("온라인") : TEXT("오프라인"));

    	// 결과 출력
    	FString Name = Friend->GetDisplayName();
    	FString IdStr = Friend->GetUserId()->ToString();

    	if (Presence.bIsOnline)
    	{
    		OnlineFriends.Add(Data);
    	}
    	else
    	{
    		OfflineFriends.Add(Data);
    	}
    }
	
    TArray<FSteamFriendData> MergedList = OnlineFriends;
    MergedList.Append(OfflineFriends);

    SetFilteredFriendList(MergedList);
}

void UJumpGameInstance::InviteFriendToSession(const FString& FriendIdStr)
{
	IOnlineSubsystem* Subsys = IOnlineSubsystem::Get();
	if (!Subsys) return;

	SessionInterface = Subsys->GetSessionInterface();
	if (!SessionInterface.IsValid()) return;

	IOnlineIdentityPtr IdentityInterface = Subsys->GetIdentityInterface();
	if (!IdentityInterface.IsValid()) return;

	TSharedPtr<const FUniqueNetId> LocalUserId = IdentityInterface->GetUniquePlayerId(0);
	TSharedPtr<const FUniqueNetId> FriendId = IdentityInterface->CreateUniquePlayerId(FriendIdStr);

	if (!LocalUserId.IsValid() || !FriendId.IsValid()) return;

	const FName SessionName = CurrentSessionName;
	bool bSuccess = SessionInterface->SendSessionInviteToFriend(*LocalUserId, SessionName, *FriendId);
	UE_LOG(LogTemp, Log, TEXT("친구 초대: %s → 세션 이름: %s"), *FriendIdStr, *SessionName.ToString());
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("SendSessionInviteToFriend 실패"));
	}
}

void UJumpGameInstance::OnSessionInviteAccepted(bool bWasSuccessful, int32 LocalUserNum,
	TSharedPtr<const FUniqueNetId> InvitingPlayerId,
	const FOnlineSessionSearchResult& SessionToJoin)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("초대 수락 실패 또는 거절됨"));
		// 여기서 UI 띄우거나, 로그 남기기
		return;
	}

	if (!SessionToJoin.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("초대 세션 정보가 유효하지 않음"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("초대 수락됨 - 세션 참가 시도"));
	SessionInterface->JoinSession(LocalUserNum, CurrentSessionName, SessionToJoin);
}

void UJumpGameInstance::SetFilteredFriendList(const TArray<FSteamFriendData>& FriendList)
{
	FilteredFriendList = FriendList;
	
	// 한 명씩 델리게이트 호출
	for (int32 i = 0; i < FilteredFriendList.Num(); ++i)
	{
		FSteamFriendData Copy = FilteredFriendList[i];
		Copy.FriendIdx = i; // 인덱스 부여
		OnFriendListUpdated.ExecuteIfBound(Copy);
	}

	// 종료 신호
	FSteamFriendData EndFlag;
	EndFlag.FriendIdx = -1;
	OnFriendListUpdated.ExecuteIfBound(EndFlag);
}

void UJumpGameInstance::RunEyeTrackingScript()
{
	FLog::Log("RunEyeTrackingScript");
	
	FString PythonPath = TEXT("C:\\Users\\user\\miniconda3\\envs\\myenv_311\\python.exe");
	FString ScriptPath = FPaths::ProjectDir() + TEXT("AppData/PythonScripts/main.py");
	FString AbsoluteScriptPath = FPaths::ConvertRelativePathToFull(ScriptPath);
	
	//FString ScriptPath = TEXT("C:\\FinalProject\\Game\\AI_Service\\eye_tracking\\infinite_counter.py");
	FString WorkingDirectory = FPaths::GetPath(AbsoluteScriptPath);

	FFastLogger::LogScreen(FColor::Red, TEXT("Python Path: %s"), *AbsoluteScriptPath);

	// Python 실행 명령 구성
	FString Command = FString::Printf(TEXT("\"%s\" \"%s\""), *PythonPath, *AbsoluteScriptPath);

	// 비동기 작업 실행
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [Command, WorkingDirectory, PythonPath, AbsoluteScriptPath]() {
		FProcHandle ProcHandle = FPlatformProcess::CreateProc(
		   *PythonPath,        // 실행 파일 경로
		   *FString::Printf(TEXT("\"%s\""), *AbsoluteScriptPath), // 인수 (스크립트 경로)
		   false,              // bLaunchHidden
		   false,              // bLaunchReallyHidden
		   false,              // bLaunchInBackground (AsyncTask가 이미 백그라운드에서 실행)
		   nullptr,            // OutProcessID
		   0,                  // PriorityModifier
		   *WorkingDirectory,      // OptionalWorkingDirectory (FString 내용을 const TCHAR*로 전달)
		   nullptr,            // PipeWriteChild
		   nullptr             // PipeReadChild
		);
	
		if (!ProcHandle.IsValid())
		{
		   UE_LOG(LogTemp, Error, TEXT("비동기 Python 프로세스 생성 실패"));
		}
		else
		{
		   UE_LOG(LogTemp, Log, TEXT("비동기 Python 프로세스 시작"));
		   // FPlatformProcess::WaitForProc(ProcHandle); // 제거: 비동기 유지를 위해 대기하지 않음
		   FPlatformProcess::CloseProc(ProcHandle);
		}
	});
}
