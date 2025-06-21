// Fill out your copyright notice in the Description page of Project Settings.


#include "BigBigBee.h"

void UBigBigBee::InitializeStory()
{
	Super::InitializeStory();
	
	StoryArray.Add(FText::FromString(TEXT("나레이션: 여름, 깨구락이가 연못가 풀숲에서 혼자 논다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 낮고 묵직한 윙윙거림이 들린다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: (두리번거리며) 뭐지, 저 소리…?")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 커다란 왕왕벌이 다가온다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 깨구락아, 가만히 있으면 벌은 그냥 지나가.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 왕왕벌이 가까이 와 바람을 일으킨다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락: (놀라며) 으악!")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 깨구락이가 물에 빠질 뻔한 순간, 엄마가 달려와 깨구락이를 안아준다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 괜찮아, 겁이 나도 침착하게 기다리면 돼.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 그날 이후, 깨구락이는 연못가를 지날 때 연잎 부채를 꼭 쥔다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 다시 왕왕벌을 만났을 때, 깨구락이는 연잎 뒤에 조용히 몸을 숨긴다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 왕왕벌은 잠시 깨구락이를 바라보다 천천히 날아간다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마: 잘했어, 점점 더 용감해지고 있구나.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 왕왕벌은 깨구락이에게 두려움을 이겨낸 용기의 상징이 되었다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션: 연못가에 울려 퍼지는 왕왕벌의 낮은 윙윙거림은 깨구락이에게 여름의 소리, 성장의 기억으로 남았다.")));
}
