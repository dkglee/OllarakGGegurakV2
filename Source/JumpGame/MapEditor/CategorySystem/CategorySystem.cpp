#include "CategorySystem.h"

#include "CategoryName.h"
#include "MajorTableInfo.h"
#include "PropWrap.h"
#include "JumpGame/Utils/CommonUtils.h"

struct FCategoryName;

UCategorySystem::UCategorySystem()
{
	PrimaryComponentTick.bCanEverTick = false;

	// static ConstructorHelpers::FObjectFinder<UDataTable> DT_PROPDATA
	// (TEXT("/Game/MapEditor/DataTable/DT_PropData.DT_PropData"));
	// CategoryDataTable = DT_PROPDATA.Object;
	//
	// static ConstructorHelpers::FObjectFinder<UDataTable> DT_MAJORINFO
	// (TEXT("/Game/MapEditor/DataTable/DT_MajorTable.DT_MajorTable"));
	// MajorTableInfoTable = DT_MAJORINFO.Object;
}

void UCategorySystem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// 카테고리 시스템을 만들어보자!!
void UCategorySystem::BeginPlay()
{
	Super::BeginPlay();

	// CategoryDataTable을 순회하고, 아이템들을 인덱싱해준다.
	if (!CategoryDataTable)
	{
		return;
	}
	
	TArray<FPropStruct*> Rows;
	CategoryDataTable->GetAllRows<FPropStruct>(TEXT(""), Rows);
	for (auto& Row : Rows)
	{
		if (Row)
		{
			AddProp(*Row);
		}
	}
}

bool UCategorySystem::SubBelongsToMajorCategory(const EMajorCategoryType InMajorCategoryType,
	const ESubCategoryType InSubCategoryType)
{
	if (!MajorTableInfoTable)
	{
		return false;
	}

	// 카테고리의 관계를 가진 테이블을 가져옴
	const FMajorTableInfo* Found = FindMajorTableInfoRow(InMajorCategoryType);

	// 찾아진게 있고 SubCategory에 대한 정보가 있는지를 반환
	return Found && Found->SubCategoryTypes.Contains(InSubCategoryType);
}

bool UCategorySystem::ValidateProp(const FPropStruct& InPropStruct)
{
	for (const auto& SubCategory : InPropStruct.PropSubCategories)
	{
		if (!SubBelongsToMajorCategory(InPropStruct.PropCategory, SubCategory))
		{
			FFastLogger::LogConsole(TEXT("PropID : %s, SubCategory : %s is not belong to MajorCategory : %s"), *InPropStruct.PropID.ToString(), *FCommonUtil::GetEnumDisplayName(SubCategory).ToString(), *FCommonUtil::GetEnumDisplayName(InPropStruct.PropCategory).ToString());
			
			return false;
		}
	}
	return true;
}

const FMajorTableInfo* UCategorySystem::FindMajorTableInfoRow(const EMajorCategoryType InMajorCategoryType)
{
	if (!MajorTableInfoTable)
	{
		return nullptr;
	}

	FName RowName = FName(*FCommonUtil::GetEnumDisplayName(InMajorCategoryType).ToString());
	const FMajorTableInfo* MajorTableInfo = MajorTableInfoTable->FindRow<FMajorTableInfo>(RowName, TEXT(""));
	if (!MajorTableInfo)
	{
		return nullptr;
	}
	
	return MajorTableInfo;
}

bool UCategorySystem::AddProp(const FPropStruct& NewProp)
{
	if (!ValidateProp(NewProp))
	{
		FFastLogger::LogConsole(TEXT("PropID : %s is not valid"), *NewProp.PropID.ToString());
	}

	UPropWrap* NewPropWrap = NewObject<UPropWrap>();
	NewPropWrap->Data = NewProp;
	PropList.Add(NewPropWrap);
	// 단일 인덱싱
	IndexProp(PropList.Last());
	OnPropAdded.Broadcast(PropList.Last());
	return true;
}

bool UCategorySystem::RemovePropByID(FName ID)
{
	int32 Removed = PropList.RemoveAll([ID](const UPropWrap* It)
	{
		return It && It->Data.PropID == ID;
	});
	if (Removed)
	{
		ReIndex();
		// 필요시 제거된 PROP 전달
		OnPropRemoved.Broadcast(nullptr);
		return true;
	}
	return false;
}

const TArray<UPropWrap*>& UCategorySystem::GetPropsByMajor(EMajorCategoryType Major)
{
	static const TArray<UPropWrap*> EmptyArray;
	FSubCache* Found = Index.Find(Major);
	if (!Found)
	{
		return EmptyArray;
	}

	static TArray<UPropWrap*> Buffer;
	Buffer.Reset();

	TSet<UPropWrap*> Seen;
	Seen.Reserve(128);

	for (auto& KV : Found->SubCategoryMap)
	{
		for (UPropWrap* Prop : KV.Value.PropList)
		{
			if (!Seen.Contains(Prop))
			{
				Seen.Add(Prop);
				Buffer.Add(Prop);
			}
		}
	}
	return Buffer;
}

const TArray<UPropWrap*>& UCategorySystem::GetPropsBySub(EMajorCategoryType Major, ESubCategoryType Sub)
{
	static const TArray<UPropWrap*> EmptyArray;
	FSubCache* Found = Index.Find(Major);
	if (!Found)
	{
		return EmptyArray;
	}

	FPropIndexList* FoundSub = Found->SubCategoryMap.Find(Sub);
	return FoundSub ? FoundSub->PropList : EmptyArray;
}

const TArray<class UPropWrap*>& UCategorySystem::GetPropsBySubs(const TArray<ESubCategoryType>& Subs)
{
	static const TArray<UPropWrap*> EmptyArray;
	TSet<UPropWrap*> Seen;
	Seen.Reserve(128);

	static TArray<class UPropWrap*> FoundLists;
	FoundLists.Reset();
	FoundLists.Reserve(128);

	// 대 분류를 먼저 돌면서 Sub에 대한 정보를 가져옴 그리고 FoundLists에 넣어줌
	for (auto& Major : Index)
	{
		for (auto& Sub : Subs)
		{
			FPropIndexList* Found = Major.Value.SubCategoryMap.Find(Sub);
			if (Found)
			{
				FoundLists.Append(Found->PropList);
			}
		}
	}
	// 중복된 PropWrap을 제거
	for (auto& Prop : FoundLists)
	{
		if (!Seen.Contains(Prop))
		{
			Seen.Add(Prop);
		}
	}
	FoundLists.Reset();
	FoundLists.Append(Seen.Array());
	return FoundLists;
}

const UPropWrap* UCategorySystem::GetPropsByID(FName ID)
{
	UPropWrap** Found = PropList.FindByPredicate([ID](const UPropWrap* It)
	{
		return It && It->Data.PropID == ID;
	});
	return Found && *Found ? *Found : nullptr;
}

const TArray<class UPropWrap*>& UCategorySystem::GetPropsByName(FName Name)
{
	static TArray<class UPropWrap*> FoundLists;
	FoundLists.Reset();
	FoundLists.Reserve(128);
	
	for (auto& Prop : PropList)
	{
		if (Prop && Prop->Data.PropName.ToString().Contains(Name.ToString(), ESearchCase::IgnoreCase))
		{
			FoundLists.Add(Prop);
		}
	}
	
	return FoundLists;
}

const TArray<EMajorCategoryType>& UCategorySystem::GetMajorCategories()
{
	static TArray<EMajorCategoryType> MajorCategories;
	MajorCategories.Reset();

	TArray<FMajorTableInfo*> MajorTableInfoRows;
	MajorTableInfoTable->GetAllRows<FMajorTableInfo>(TEXT(""), MajorTableInfoRows);
	for (auto& MajorTableInfo : MajorTableInfoRows)
	{
		if (MajorTableInfo)
		{
			MajorCategories.Add(MajorTableInfo->MajorCategoryType);
		}
	}
	
	return MajorCategories;
}

const TArray<ESubCategoryType>& UCategorySystem::GetSubCategoriesByMajor(EMajorCategoryType Major)
{
	static const TArray<ESubCategoryType> EmptyArray;
	
	static TArray<ESubCategoryType> SubCategories;
	SubCategories.Reset();
	
	const FMajorTableInfo* MajorTableInfo = FindMajorTableInfoRow(Major);
	if (!MajorTableInfo)
	{
		return EmptyArray;
	}

	for (auto& SubCategory : MajorTableInfo->SubCategoryTypes)
	{
		SubCategories.Add(SubCategory);
	}
	
	return SubCategories;
}

const TArray<ESubCategoryType>& UCategorySystem::GetSubCategoriesByMajorWithoutHidden(EMajorCategoryType MajorCategory)
{
	static const TArray<ESubCategoryType> EmptyArray;
	
	static TArray<ESubCategoryType> SubCategories;
	SubCategories.Reset();
	
	const FMajorTableInfo* MajorTableInfo = FindMajorTableInfoRow(MajorCategory);
	if (!MajorTableInfo)
	{
		return EmptyArray;
	}

	for (auto& SubCategory : MajorTableInfo->SubCategoryTypes)
	{
		if (CategoryInfoTables->FindRow<FCategoryName>(FName(*FCommonUtil::GetEnumDisplayName(SubCategory).ToString()), TEXT(""), true)->bHidden)
		{
			continue ;
		}
		SubCategories.Add(SubCategory);
	}
	
	return SubCategories;
}

void UCategorySystem::ReIndex()
{
	Index.Empty();
	for (auto& Prop : PropList)
	{
		IndexProp(Prop);
	}
}

void UCategorySystem::IndexProp(UPropWrap* InPropWrap)
{
	if (!InPropWrap) return;

	const EMajorCategoryType Major = InPropWrap->Data.PropCategory;
	const TArray<ESubCategoryType>& SubCategories = InPropWrap->Data.PropSubCategories;

	if (SubCategories.Num() == 0)
	{
		// SubCategory가 없는 경우, None으로 분류
		Index.FindOrAdd(Major)
			.SubCategoryMap.FindOrAdd(ESubCategoryType::None)
			.PropList.Add(InPropWrap);
	}
	else
	{
		for (const ESubCategoryType& Sub : SubCategories)
		{
			Index.FindOrAdd(Major)
				.SubCategoryMap.FindOrAdd(Sub)
				.PropList.Add(InPropWrap);
		}
	}
}

UCategorySystem::~UCategorySystem()
{
}