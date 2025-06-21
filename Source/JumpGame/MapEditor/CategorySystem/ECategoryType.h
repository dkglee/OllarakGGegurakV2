#pragma once

#include "ECategoryType.generated.h"

UENUM(BlueprintType)
enum class EMajorCategoryType : uint8
{
	None = 0		UMETA(DisplayName = "None"),
	Basic = 1		UMETA(DisplayName = "Basic"),
	Obstacle = 2	UMETA(DisplayName = "Obstacle"),
	Platform = 3	UMETA(DisplayName = "Platform"),
	Decoration = 4	UMETA(DisplayName = "Decoration"),
	Count			UMETA(DisplayName = "Count"),			// 카테고리 개수
};

UENUM()
enum class ESubCategoryType : uint8
{
	None = 0		UMETA(DisplayName = "None"),

#pragma region BaseSubTags
	// 자연 재질
	Wood			UMETA(DisplayName = "Wood"),			// 목재, 대나무 포함
	Stone			UMETA(DisplayName = "Stone"),			// 암석 계열
	Soil			UMETA(DisplayName = "Soil"),			// 흙, 모래, 자갈 포함
	Plant			UMETA(DisplayName = "Plant"),			// 수풀
	Water			UMETA(DisplayName = "Water"),			// 수풀(물), 수련잎 포함
	Ice				UMETA(DisplayName = "Ice"),				// 얼음, 눈 포함

	// 구조 및 인공물
	Build			UMETA(DisplayName = "Build"),			// 인공물
	BaseEtc			UMETA(DisplayName = "BaseEtc"),			// 기본/기타
	
#pragma endregion
	
#pragma region ObstacleSubTags
	Bouncing			UMETA(DisplayName = "Bouncing"),	// 바운스
	Rotating			UMETA(DisplayName = "Rotating"),	// 회전
	Pad					UMETA(DisplayName = "Pad"),			// 패드
	Firing				UMETA(DisplayName = "Firing"),		// 발사
	AI					UMETA(DisplayName = "AI"),			// AI
	ObstacleEtc			UMETA(DisplayName = "ObstacleEtc"),	// 장애물/기타
	
#pragma endregion

#pragma region PlatformSubTags
	// 플랫폼
	Moving			UMETA(DisplayName = "Moving"),			// 이동 플랫폼
	Starting		UMETA(DisplayName = "Starting"),		// 시작
	Ending			UMETA(DisplayName = "Ending"),				// 끝
	PlatformEtc		UMETA(DisplayName = "PlatformEtc"),		// 플랫폼/기타

#pragma endregion

#pragma region DecorationSubTags
	// 동적/특수 효과
	Fire			UMETA(DisplayName = "Fire"),			// 불
	Flower			UMETA(DisplayName = "Flower"),			// 꽃, 잔디, 꽃, 덩쿨 등
	Tree			UMETA(DisplayName = "Tree"),			// 나무
	DecorationEtc	UMETA(DisplayName = "DecorationEtc"),				// 기타
	
#pragma endregion

	Count			UMETA(DisplayName = "Count"),			// 카테고리 개수

#pragma region Seasons
	Spring	= 101		UMETA(DisplayName = "Spring"),			// 봄
	Summer	= 102		UMETA(DisplayName = "Summer"),			// 여름
	Fall	= 103		UMETA(DisplayName = "Fall"),				// 가을
	Winter	= 104		UMETA(DisplayName = "Winter"),			// 겨울
#pragma endregion
};