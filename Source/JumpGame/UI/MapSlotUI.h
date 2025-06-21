#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "MapSlotUI.generated.h"

DECLARE_DELEGATE_OneParam(FOnMapSlotClicked, class UMapSlotUI* /*Map Full Path*/);

UCLASS()
class JUMPGAME_API UMapSlotUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void Init(const FString& InMapFullPath, const FString& InMapName);

	FOnMapSlotClicked OnClicked;

	GETTER(FString, MapFullPath);
	GETTER(FString, MapName);
	GETTER(class UTexture2D*, SavedThumbnail);
	
	UPROPERTY()
	TArray<uint8> ImageData; // 이미지 데이터를 저장할 배열
private:
	UFUNCTION()
	void OnBtnMapSlotClicked();
	UFUNCTION()
	class UTexture2D* GetMapThumbnail(const FString& InMapFullPath);
	UFUNCTION()
	class UTexture2D* MakeTextureFromBytes(const TArray<uint8>& FileData);
	
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapSlot", meta = (AllowPrivateAccess = "true"))
	class UButton* Btn_MapSlot;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapSlot", meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Text_MapName;
	
	UPROPERTY()
	FString MapFullPath;
	UPROPERTY()
	FString MapName;
	UPROPERTY()
	class UTexture2D* SavedThumbnail;
};
