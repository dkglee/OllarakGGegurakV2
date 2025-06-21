// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogHill.h"

void UFrogHill::InitializeStory()
{
	Super::InitializeStory();

	StoryArray.Add(FText::FromString(TEXT("나레이션: 봄, 엄마가 깨구락이 손을 잡고 개구리동산으로 향한다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 오늘은 봄나물도 캐고, 개구리도 만나보자.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 동산 입구엔 민들레와 제비꽃이 가득 피었다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: 엄마, 저기 작은 개구리 봐!")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 동산엔 네 친구들이 많지.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 엄마는 돌콩잎과 봄나물을 바구니에 담는다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: 이건 먹어도 돼?")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 그건 그냥 풀! 향을 맡아봐.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 바위 아래엔 연한 봄나물도 숨어 있다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 이건 바위아래봄잎이야. 부드럽고 맛이 고와.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 엄마는 연잎에 봄나물을 싸서 건넨다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: (한 입 베어물며) 쌉쌀하지만 고소해.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 힘들 땐 이 꿀차를 마셔봐.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 엄마가 산샘물에 산꿀과 잎사귀를 풀어 잎사귀꿀차를 만들어 준다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: (차를 마시며) 엄마, 진짜 맛있어!")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 우리만의 소풍, 기억하자.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 해질 무렵, 엄마가 몰래 싸준 연잎밥이 깨구락이 손에 쥐어졌다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 개구리동산엔 자연의 향기와 엄마의 손길, 봄날의 추억이 남았다.")));
}
