#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "FileBrowserUI.generated.h"

DECLARE_DELEGATE_TwoParams(FOnFileSelected, const FString& /*Path*/, bool /*bSuccess*/);

UCLASS()
class JUMPGAME_API UFileBrowserUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void LoadDirectoryContents(const FString& DirectoryPath);
	UFUNCTION()
	void CreateDirectoryButton(const FString& DirName, const FString& FullPath);
	UFUNCTION()
	void CreateFileButton(const FString& FileName, const FString& FullPath);

	UFUNCTION()
	void OnDirectorySelected(const FString& FullPath);
	UFUNCTION()
	void OnFileSelected(const FString& FullPath);
	UFUNCTION()
	void OnItemClicked(const FString& FileName, class UItemUI* ItemUI);
	UFUNCTION()
	void OnSelectButtonClicked();

	UFUNCTION()
	void CreateBackButton();
	UFUNCTION()
	void OnCloseButtonClicked();
	UFUNCTION()
	void SetHintText(const FString& HintText);
	UFUNCTION()
	void SetInfoText(const FString& InfoText);

	FOnFileSelected OnFileSelectedDelegate;

	SETTER(TArray<FString>, Suffixes)

private:
	FString EllipsisLastFolders(const FString& InPath, int32 NumFoldersToKeep = 2);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class UItemUI> FileItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class UItemUI> DirectoryItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class UItemUI> BackButtonClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UScrollBox* FileListScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UEditableText* CurrentFileText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UButton* SelectButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UButton* CloseButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UTextBlock* CurrentDirectoryText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UTextBlock* InformationText;

	FString CurrentDirectory;
	FString CurrentFilePath;
	FString DefaultString = TEXT(R"(파일을 선택하세요)");

	UPROPERTY()
	class UItemUI* CurrentSelectedItem;
	
	TArray<FString> Suffixes;
};
