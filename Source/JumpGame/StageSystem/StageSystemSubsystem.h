#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StageTableRow.h"
#include "FieldTableRow.h"
#include "StageSystemSubsystem.generated.h"

#define DEFAULT_STAGE_PREFIX TEXT("Maps/Stages/") 


UCLASS()
class JUMPGAME_API UStageSystemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	// 조회용 API
	const FStageTableRow* GetStage(const FName& StageID) const;
	const FFieldTableRow* GetField(const FName& FieldID) const;
	const TArray<FName>& GetFieldsInStage(const FName& StageID) const;

	FString ConvertFiledIDToPath(const FName& FieldID) const;

	// 게임 저장
	void SaveFieldResult(FName FieldID, int32 StarCount, float ClearTime);

	// Save/Load 함수
	void SaveProgressToDisk();
	void LoadProgressFromDisk();

#pragma region GETTERSETTER
	void SetChosenStage(const FName& StageID) { ChosenStageID = StageID; }
	FName GetChosenStage() const { return ChosenStageID; }
	void SetChosenField(const FName& FieldID) { ChosenFieldID = FieldID; }
	FName GetChosenField() const { return ChosenFieldID; }
#pragma endregion

private:
	// 스테이지 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage System", meta = (AllowPrivateAccess = "true"))
	class UDataTable* StageDataTable;
	// 필드 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage System", meta = (AllowPrivateAccess = "true"))
	class UDataTable* FieldDataTable;
	// 스테이지 캐시
	TMap<FName, FStageTableRow*> StageCache;
	// 필드 캐시
	TMap<FName, FFieldTableRow*> FieldCache;
	// Stage to Field 매핑
	TMap<FName, TArray<FName>> StageToFields;

	const FString DefaultPrefix = DEFAULT_STAGE_PREFIX;

	FName ChosenStageID;
	FName ChosenFieldID;
	
	void BuildCaches();
};
