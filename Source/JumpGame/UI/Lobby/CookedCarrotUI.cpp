// Fill out your copyright notice in the Description page of Project Settings.


#include "CookedCarrotUI.h"

void UCookedCarrotUI::InitializeStory()
{
	Super::InitializeStory();

	StoryArray.Add(FText::FromString(TEXT("나레이션: 봄날, 엄마가 싱싱한 당근을 한 아름 사온다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 오늘은 삶은 당근으로 밥상을 차려볼까?")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 주방엔 달큰한 당근 향이 가득 퍼진다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: (숟가락으로 당근을 밀며) 엄마, 난 당근이 별로야…")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 네가 아팠을 때 이 당근죽 먹고 금방 나았잖아.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 깨구락이는 그때의 따뜻한 죽을 떠올린다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 한 번만 먹어봐. 봄 당근은 달고 부드러워.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 깨구락이는 조심스럽게 한 조각을 입에 넣는다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: …음, 예전보다 괜찮은 것 같아.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 그래, 조금씩 익숙해지는 거야.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 엄마는 당근을 작게 썰어 도토리묵 무침에도 곁들여준다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: (도토리묵과 함께 먹으며) 묘하게 잘 어울리네.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 연잎에 싸서 먹으면 더 향긋하단다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 어느 날, 엄마는 삶은 당근을 연잎에 싸서 주먹밥을 만들어준다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: (연잎밥을 먹으며) 이건 향긋해서 좋아.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 네가 자라는 만큼, 입맛도 자라는 거란다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 깨구락이는 식탁에 당근이 올라와도 예전처럼 미루지 않는다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 아직도 썩 좋아하진 않지만, 엄마의 마음을 생각하며 한 조각씩 꼭꼭 씹어 먹는다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 삶은 당근은 깨구락이에게 봄의 온기와 엄마의 마음이 담긴 음식으로 남았다.")));
}
