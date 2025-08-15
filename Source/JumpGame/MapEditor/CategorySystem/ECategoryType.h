#pragma once

#include "ECategoryType.generated.h"

UENUM(BlueprintType)
enum class EMajorCategoryType : uint8
{
	None = 0		UMETA(DisplayName = "None"),
	Necessary = 1	UMETA(DisplayName = "Necessary"),	// 필수 카테고리
	Basic = 2		UMETA(DisplayName = "Basic"), // 기본 카테고리
	Decoration = 3	UMETA(DisplayName = "Decoration"), // 장식 카테고리
	Fake = 4		UMETA(DisplayName = "Fake"), // 가짜 카테고리
	Count			UMETA(DisplayName = "Count"),			// 카테고리 개수
};

UENUM()
enum class ESubCategoryType : uint8
{
	None = 0		UMETA(DisplayName = "None"),

	// Necessary
	Start			UMETA(DisplayName = "Start"), // 시작점
	End				UMETA(DisplayName = "End"), // 끝점
	Star			UMETA(DisplayName = "Star"), // 별

	// Basic
	Ground			UMETA(DisplayName = "Ground"), // 바닥
	NatureBasic		UMETA(DisplayName = "NatureBasic"), // 자연
	Building		UMETA(DisplayName = "Building"), // 건물
	Obstacle		UMETA(DisplayName = "Obstacle"), // 장애물

	// Decoration
	Item			UMETA(DisplayName = "Item"), // 아이템
	NatureDeco		UMETA(DisplayName = "NatureDeco"), // 자연 장식
	BuildingDeco	UMETA(DisplayName = "BuildingDeco"), // 건물 장식

	// Fake
	Chameleon		UMETA(DisplayName = "Chameleon"), // 카멜레온
	GroundFake		UMETA(DisplayName = "GroundFake"), // 바닥 가짜
	NatureFake		UMETA(DisplayName = "NatureFake"), // 자연 가짜
	BuildingFake	UMETA(DisplayName = "BuildingFake"), // 건물 가짜

	Count			UMETA(DisplayName = "Count"), // 서브 카테고리 개수
};