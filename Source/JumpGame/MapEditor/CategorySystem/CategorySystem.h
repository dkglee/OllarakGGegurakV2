#pragma once

#include "CoreMinimal.h"
#include "ECategoryType.h"
#include "MajorTableInfo.h"
#include "PropStruct.h"
#include "Components/ActorComponent.h"
#include "CategorySystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPropAdded, const class UPropWrap*, Prop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPropRemoved, const class UPropWrap*, Prop);

USTRUCT()
struct FPropIndexList
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<class UPropWrap*> PropList;

	FPropIndexList(const TArray<class UPropWrap*> InPropList = {})
		: PropList(InPropList)
	{
	}
};

USTRUCT()
struct FSubCache
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<ESubCategoryType, FPropIndexList> SubCategoryMap;

	FSubCache(const TMap<ESubCategoryType, FPropIndexList> InSubCategoryMap = {})
		: SubCategoryMap(InSubCategoryMap)
	{
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UCategorySystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UCategorySystem();

	// CRUD 함수
	UFUNCTION(BlueprintCallable)
	bool AddProp(const FPropStruct& NewProp);
	UFUNCTION(BlueprintCallable)
	bool RemovePropByID(FName ID);
	
	const TArray<EMajorCategoryType>& GetMajorCategories();
	const TArray<ESubCategoryType>& GetSubCategoriesByMajor(EMajorCategoryType Major);
	const TArray<ESubCategoryType>& GetSubCategoriesByMajorWithoutHidden(EMajorCategoryType MajorCategory);

	// 조회 함수
	const TArray<class UPropWrap*>& GetPropsByMajor(EMajorCategoryType Major);
	const TArray<class UPropWrap*>& GetPropsBySub(EMajorCategoryType Major, ESubCategoryType Sub);
	const TArray<class UPropWrap*>& GetPropsBySubs(const TArray<ESubCategoryType>& Subs);
	const class UPropWrap* GetPropsByID(FName ID);
	const TArray<class UPropWrap*>& GetPropsByName(FName Name);

	UPROPERTY(BlueprintAssignable)
	FOnPropAdded OnPropAdded;
	UPROPERTY(BlueprintAssignable)
	FOnPropRemoved OnPropRemoved;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual ~UCategorySystem() override;
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	bool SubBelongsToMajorCategory(const EMajorCategoryType InMajorCategoryType, const ESubCategoryType InSubCategoryType);
	UFUNCTION()
	bool ValidateProp(const FPropStruct& InPropStruct);
	UFUNCTION()
	void ReIndex();
	UFUNCTION()
	void IndexProp(class UPropWrap* InPropWrap);
	
	const FMajorTableInfo* FindMajorTableInfoRow(const EMajorCategoryType InMajorCategoryType);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	class UDataTable* CategoryDataTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	class UDataTable* MajorTableInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	class UDataTable* CategoryInfoTables = nullptr;

	// 카테고리별로 Prop을 관리하기 위한 Map
	UPROPERTY()
	TArray<class UPropWrap*> PropList;
	UPROPERTY()
	TMap<EMajorCategoryType, FSubCache> Index;
	// TMap<EMajorCategoryType, TMap<ESubCategoryType, TArray<class UPropWrap*>>> Index;
};
