// Fill out your copyright notice in the Description page of Project Settings.


#include "PondFly.h"

void UPondFly::InitializeStory()
{
	Super::InitializeStory();

	StoryArray.Add(FText::FromString(TEXT("나레이션: 연못가에서 깨구락이가 조약돌을 던지며 논다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 연못파리가 깨구락이 주변을 맴돈다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: 으, 왜 나만 따라오지?")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 연못파리는 네가 놀라는 걸 더 좋아해. 그냥 모른 척해봐.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 깨구락이는 연잎을 부채처럼 휘둘러보지만, 파리는 늘 먼저 도망간다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 친구들도 멀리서 웃으며 바라본다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: (투덜거리며) 연못파리만 아니면 더 재밌을 텐데…")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 깨구락이는 연잎에 꿀을 발라 파리를 유인해보지만, 소용없다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 어느 날, 연못파리가 연잎 위에 잠시 앉는다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: (작게) 생각보다 귀엽네…")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 연못파리도 연못의 일부란다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 이제 연못파리가 나타나도 깨구락이는 그저 연못의 한 풍경처럼 받아들인다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 가끔은 연못파리가 사라지면 조금 허전하기도 하다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 연못파리는 깨구락이에게 어린 시절의 한 장면으로 남았다.")));
}
