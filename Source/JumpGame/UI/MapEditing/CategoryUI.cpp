// Fill out your copyright notice in the Description page of Project Settings.


#include "CategoryUI.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "JsonObjectConverter.h"
#include "MajorCategoryButtonUI.h"
#include "PropGridUI.h"
#include "SubCategoryButtonUI.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "JumpGame/AIServices/Shared/HttpManagerComponent.h"
#include "JumpGame/AIServices/Shared/HttpMessage.h"
#include "JumpGame/AIServices/Shared/Message.h"
#include "JumpGame/AIServices/Shared/IOHandlers/IOHandlerInterface.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/Core/GameState/MapGameState.h"
#include "JumpGame/MapEditor/CategorySystem/CategorySystem.h"
#include "JumpGame/MapEditor/CategorySystem/ECategoryType.h"
#include "JumpGame/MapEditor/CategorySystem/ImageResponseJson.h"
#include "JumpGame/MapEditor/CategorySystem/PropWrap.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/UI/FileBrowser/FileBrowserUI.h"
#include "JumpGame/UI/MapEditing/MapLoadingUI.h"
#include "JumpGame/UI/MapEditing/SaveResultUI.h"

class ANetworkGameState;

void UCategoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ClearMajorCategoryButtons();
	ClearSubCategoryButtons();
	
	// GameState를 가져와서 모든 MajorCategory 리스트를 가져온다.
	AMapEditorState* GameState = GetWorld()->GetGameState<AMapEditorState>();
	if (GameState)
	{
		// Category 시스템을 가져옴.
		CategorySystem = GameState->GetCategorySystem();
		GameState->HttpManagerComponent->HttpHandlers[EMessageType::HttpMultipartRequest]->OnMessageReceived.AddUObject(this, &UCategoryUI::OnImageSearchResponse);
	}

	if (!CategorySystem)
	{
		UE_LOG(LogTemp, Error, TEXT("CategorySystem is null"));
		return;
	}

	// MajorCategory 버튼들을 미리 만들어두기
	const TArray<EMajorCategoryType>& MajorCategoriesList = CategorySystem->GetMajorCategories();
	for (auto& MajorCategory : MajorCategoriesList)
	{
		UMajorCategoryButtonUI* MajorCategoryButtonUI = CreateWidget<UMajorCategoryButtonUI>(GetWorld(), MajorCategoryButtonUIClass);
		if (MajorCategoryButtonUI)
		{
			MajorCategoryButtons.Add(MajorCategory, MajorCategoryButtonUI);
			
			MajorCategoryButtonUI->SetMajorCategory(MajorCategory);
			MajorCategories->AddChild(MajorCategoryButtonUI);

			MajorCategoryButtonUI->OnMajorButtonClicked.AddDynamic(this, &UCategoryUI::OnMajorCategoryButtonClicked);
			
			if (MajorCategory == EMajorCategoryType::Basic)
			{
				SelectedMajorCategory = MajorCategoryButtonUI;
			}
		}
	}

	// SubCategory 버튼들을 미리 만들어두기
	for (auto& MajorCategory : MajorCategoriesList)
	{
		const TArray<ESubCategoryType>& SubCategoryList = CategorySystem->GetSubCategoriesByMajorWithoutHidden(MajorCategory);
		for (auto& SubCategory : SubCategoryList)
		{
			USubCategoryButtonUI* SubCategoryButtonUI = CreateWidget<USubCategoryButtonUI>(GetWorld(), SubCategoryButtonUIClass);
			
			SubCategoryButtonUI->SetSubCategoryInfo(MajorCategory, SubCategory);

			SubCategoryButtons.Add(SubCategory, SubCategoryButtonUI);

			SubCategoryButtonUI->OnSubCategoryButtonClicked.AddDynamic(this, &UCategoryUI::OnSubCategoryButtonClicked);
		}
	}

	OnMajorCategoryButtonClicked(SelectedMajorCategory->GetMajorCategoryType(), true);
	
	FText Text = FText::FromString(DefaultSearchText);
	SearchText->SetHintText(Text);

	SearchText->OnTextCommitted.AddDynamic(this, &UCategoryUI::OnSearchTextCommitted);
	SearchText->OnTextChanged.AddDynamic(this, &UCategoryUI::OnSearchTextChanged);

	ImageSearchButton->OnClicked.AddDynamic(this, &UCategoryUI::OnImageSearchButtonClicked);

	ImageSearchLoadingUI = CreateWidget<UMapLoadingUI>(GetWorld(), MapLoadingUIClass);
	SaveResultUI = CreateWidget<USaveResultUI>(GetWorld(), SaveResultUIClass);
}

void UCategoryUI::NativeDestruct()
{
	Super::NativeDestruct();

	GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);
}

void UCategoryUI::InitWidget(class UClickHandlerManager* ClickHandlerManager,
                             class UWidgetMapEditDragDropOperation* DragDropOperation)
{
	GridUI->InitWidget(ClickHandlerManager, DragDropOperation, this);
}

void UCategoryUI::OnMajorCategoryButtonClicked(const EMajorCategoryType& MajorCategory, bool bAbsolute)
{
	ClearSubCategoryButtons();

	const TArray<ESubCategoryType>& SubCategoryList = CategorySystem->GetSubCategoriesByMajorWithoutHidden(MajorCategory);
	for (auto& SubCategory : SubCategoryList)
    {
        USubCategoryButtonUI* SubCategoryButtonUI = SubCategoryButtons[SubCategory];
        if (SubCategoryButtonUI)
        {
            SubCategories->AddChild(SubCategoryButtonUI);
            SubCategoryButtonUI->SetVisibility(ESlateVisibility::Visible);
        }
    }
	
	if (SelectedMajorCategory)
	{
		SelectedMajorCategory->ClearSelected();
	}
	SelectedMajorCategory = MajorCategoryButtons[MajorCategory];
	if (SelectedMajorCategory)
	{
		SelectedMajorCategory->SetSelected();
	}

	// 모든 PropData를 업데이트한다.
	GridUI->UpdatePropGrid(MajorCategory, CategorySystem);
}

void UCategoryUI::ClearSubCategoryButtons()
{
	// MajorCategory 버튼을 클릭했을 때, SubCategory 버튼들을 초기화한다.
	TArray<UWidget*> Children = SubCategories->GetAllChildren();
	for (int32 i = Children.Num() - 1; i >= 0; --i)
	{
		UWidget* Child = Children[i];
		
		if (Child && Child->IsA(USubCategoryButtonUI::StaticClass()))
		{
			SubCategories->RemoveChild(Child);
		}
	}
	if (SelectedSubCategory)
	{
		SelectedSubCategory->ClearSelected();
	}
	if (SelectedMajorCategory)
	{
		SelectedMajorCategory->ClearSelected();
	}
	SelectedMajorCategory = nullptr;
}

void UCategoryUI::ClearMajorCategoryButtons()
{
	TArray<UWidget*> Children = MajorCategories->GetAllChildren();

	// 역순 루프로 제거해야 제대로 제거됨
	for (int32 i = Children.Num() - 1; i >= 0; --i)
	{
		UWidget* Child = Children[i];
        
		if (Child && Child->IsA(UMajorCategoryButtonUI::StaticClass()))
		{
			MajorCategories->RemoveChild(Child);
		}
	}
	if (SelectedMajorCategory)
	{
		SelectedMajorCategory->ClearSelected();
	}
	SelectedMajorCategory = nullptr;
}

void UCategoryUI::OnSubCategoryButtonClicked(const EMajorCategoryType& MajorCategory, const ESubCategoryType& SubCategory)
{
    // SubCategory 버튼을 클릭했을 때, PropGridUI를 업데이트한다.
    GridUI->UpdatePropGridBySub(MajorCategory, SubCategory, CategorySystem);

	if (SelectedSubCategory)
	{
		SelectedSubCategory->ClearSelected();
	}
	SelectedSubCategory = SubCategoryButtons[SubCategory];
	if (SelectedSubCategory)
	{
		SelectedSubCategory->SetSelected();
	}
	
}

void UCategoryUI::OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);
	if (CommitMethod == ETextCommit::OnEnter)
	{
		FString InputText = Text.ToString();
		GridUI->UpdatePropGridBySearch(InputText, CategorySystem);
	}
	FString InputText = Text.ToString();
	if (InputText == TEXT("") || InputText == DefaultSearchText)
	{
		GridUI->UpdatePropGrid(SelectedMajorCategory->GetMajorCategoryType(), CategorySystem);
		FText DefaultText = FText::FromString(DefaultSearchText);
		SearchText->SetHintText(DefaultText);
	}
}

void UCategoryUI::OnPropSlotClicked(FName PropID, UClass* InPropClass)
{
	const UPropWrap* Prop = CategorySystem->GetPropsByID(PropID);
	if (!Prop)
	{
		return;
	}
	PropOverviewImage->SetBrushFromTexture(Prop->Data.PropIcon);
}

void UCategoryUI::OnSearchTextChanged(const FText& Text)
{
	GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);
	TWeakObjectPtr<UUserWidget> WeakThis = this;
	GetWorld()->GetTimerManager().SetTimer(SearchTimerHandle, FTimerDelegate::CreateLambda([WeakThis, Text]()
	{
		if (!WeakThis.IsValid())
		{
			return;
		}
		UCategoryUI* CategoryUI = Cast<UCategoryUI>(WeakThis.Get());

		FString InputText = Text.ToString();
		if (InputText == TEXT(""))
		{
			CategoryUI->GridUI->UpdatePropGrid(CategoryUI->SelectedMajorCategory->GetMajorCategoryType(), CategoryUI->CategorySystem);
		}
		else
		{
			CategoryUI->GridUI->UpdatePropGridBySearch(InputText, CategoryUI->CategorySystem);
		}
	}), SearchDelay, false);
}

// TODO: OpenFileDialog를 사용하지 않게 해야 함.
void UCategoryUI::OnImageSearchButtonClicked()
{
	SearchText->SetText(FText::FromString(TEXT("")));
	SearchText->SetHintText(FText::FromString(TEXT("이미지 검색 중 입니다...")));
	SearchText->SetIsReadOnly(true);
	GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);

	AMapEditorState* GameState = GetWorld()->GetGameState<AMapEditorState>();
	if (!GameState)
	{
		FFastLogger::LogConsole(TEXT("GameState is null"));
		SetTextToDefault();
		SetGridToDefault();
		return;
	}

	GameState->GetLoadMapComponent()->GetFileBrowserUI()->OnFileSelectedDelegate.Unbind();
	GameState->GetLoadMapComponent()->GetFileBrowserUI()->OnFileSelectedDelegate.BindUObject(this, &UCategoryUI::OnImageSearchButtonResponse);
	GameState->GetLoadMapComponent()->GetFileBrowserUI()->SetSuffixes({TEXT(".jpg"), TEXT(".png")});
	GameState->GetLoadMapComponent()->GetFileBrowserUI()->SetInfoText(TEXT("검색할 이미지를 선택해 주세요.\nAI가 자동으로 해당 계절에 맞는 에셋을 추천해줍니다."));
	
	FString RelativeDir = FPaths::ProjectDir();

	FString AbsoluteDir = FPaths::ConvertRelativePathToFull(RelativeDir);
	FPaths::MakePlatformFilename(AbsoluteDir);
	
	GameState->GetLoadMapComponent()->GetFileBrowserUI()->SetVisibility(ESlateVisibility::Visible);
	GameState->GetLoadMapComponent()->GetFileBrowserUI()->LoadDirectoryContents(AbsoluteDir);
}

bool UCategoryUI::OpenFileDialog(FString& OutFilePath)
{
	TArray<FString> OutFiles;
	
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		return false;
	}
	if (!GEngine)
	{
		return false;
	}
	if (!GEngine->GameViewport)
	{
		return false;
	}

	void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();

	uint32 SelectionFlag = 0;
	// A value of 0 represents single file selection while a value of 1 represents multiple file selection
	bool bOpened = DesktopPlatform->OpenFileDialog(ParentWindowHandle, TEXT("이미지 선택"), TEXT(""), TEXT(""),
		TEXT("Image Files (*.png;*.jpg)|*.png;*.jpg"), SelectionFlag, OutFiles);
	
	if (bOpened && OutFiles.Num() > 0)
	{
		FFastLogger::LogConsole(TEXT("OutFiles[0] : %s"), *OutFiles[0]);
		OutFilePath = OutFiles[0];
		return true;
	}
	
	return false;
}

bool UCategoryUI::SendImageRequest(const FString& ImagePath)
{
	AMapEditorState* GameState = GetWorld()->GetGameState<AMapEditorState>();
	if (!GameState)
	{
		UE_LOG(LogTemp, Error, TEXT("GameState is null"));
		SetTextToDefault();
		SetGridToDefault();
		return false;
	}
	
	TArray<uint8> ImageData;
	if (!FFileHelper::LoadFileToArray(ImageData, *ImagePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load image file: %s"), *ImagePath);
		SetTextToDefault();
		SetGridToDefault();
		return false;
	}

	FHttpMultipartField ImageField;
	FString FileNameWithExtension = FPaths::GetCleanFilename(ImagePath);
	ImageField.FieldName = TEXT("file");
	ImageField.FileName = FileNameWithExtension;
	ImageField.Data = ImageData;
	// ImageField.ContentType = TEXT("image/jpeg");

	FString Extension = FPaths::GetExtension(ImagePath, false).ToLower();
	if (Extension == TEXT("jpg") || Extension == TEXT("jpeg"))
	{
		ImageField.ContentType = TEXT("image/jpeg");
	}
	else if (Extension == TEXT("png"))
	{
		ImageField.ContentType = TEXT("image/png");
	}

	FHttpMultipartRequest Request;
	FHTTPHandlerInitInfo HttpInfo = GameState->HttpManagerComponent->GetHttpHandlerInitInfo();
	Request.ServerURL = HttpInfo.HttpsRequestImageURL;
	Request.RequestPath = HttpInfo.HttpsRequestImagePath;
	// Request.AdditionalHeaders.Add(TEXT("Authorization"), TEXT("Bearer token"));
	Request.MultipartFields.Add(ImageField);

	FHttpMessageWrapper Message;
	Message.Header = FMessageHeader();
	Message.Header.Type = EMessageType::HttpMultipartRequest;
	Message.HttpMessage = Request;

	if (!GameState->HttpManagerComponent->SendHttpMessage(Message))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to send image request"));
		SetTextToDefault();
		SetGridToDefault();
		return false;
	}

	// 로딩 UI 띄우기
	ImageSearchLoadingUI->AddToViewport(99);
	ImageSearchLoadingUI->SetLoadingText(TEXT("이미지 검색 중..."));
	ImageSearchLoadingUI->PlayLoadingAnim();
	
	return true;
}

void UCategoryUI::OnImageSearchResponse(bool bSuccess)
{
	// UI 지우기
	ImageSearchLoadingUI->RemoveFromParent();
	
	if (!bSuccess)
	{
		FFastLogger::LogConsole(TEXT("Image Search Response failed"));
		ShowResultUI(false, TEXT("이미지 검색에 실패했습니다."));
		SetTextToDefault();
		SetGridToDefault();
		return;
	}
	
	FHttpMessageWrapper Response;
	AMapEditorState* GameState = GetWorld()->GetGameState<AMapEditorState>();
	if (!GameState)
	{
		FFastLogger::LogConsole(TEXT("GameState is null"));
		ShowResultUI(false, TEXT("이미지 검색에 실패했습니다."));
		SetTextToDefault();
		SetGridToDefault();
		return;
	}
	
	if (!GameState->HttpManagerComponent->PopHttpMessage(EMessageType::HttpMultipartResponse, Response))
	{
		SetTextToDefault();
		SetGridToDefault();
		return ;
	}

	// FHttpMultipartResponse HttpResponse = Response.HttpMessage.Get<FHttpMultipartResponse>();
	FHttpMultipartResponse* HttpResponse = std::get_if<FHttpMultipartResponse>(&Response.HttpMessage);
	if (!HttpResponse)
	{
		FFastLogger::LogConsole(TEXT("Invalid HttpMessage type"));
		ShowResultUI(false, TEXT("이미지 검색에 실패했습니다."));
		SetTextToDefault();
		SetGridToDefault();
		return;
	}
	// 응답코드가 2xx가 아닐 경우
	if (HttpResponse->ResponseCode / 100 != 2)
	{
		FFastLogger::LogConsole(TEXT("Image search failed: %d"), HttpResponse->ResponseCode);
		ShowResultUI(false, TEXT("이미지 검색에 실패했습니다."));
		SetTextToDefault();
		SetGridToDefault();
		return;
	}
	
	// Json을 카테고리 리스트로 변환
	FImageResponseJson ImageResponse;
	FJsonObjectConverter::JsonObjectStringToUStruct(HttpResponse->ResponseText, &ImageResponse);
	if (!ImageResponse.SubCategoryList.Num())
	{
		FFastLogger::LogConsole(TEXT("No subcategories found"));
		ShowResultUI(false, TEXT("이미지 검색에 실패했습니다."));
		SetTextToDefault();
		SetGridToDefault();
		return;
	}

	ShowResultUI(true, TEXT("이미지 검색에 성공했습니다."));
	SetTextToDefault();
	GridUI->UpdatePropGridByImageSearch(ImageResponse.SubCategoryList, CategorySystem);
}

void UCategoryUI::SetTextToDefault()
{
	SearchText->SetHintText(FText::FromString(DefaultSearchText));
	SearchText->SetIsReadOnly(false);
}

void UCategoryUI::SetGridToDefault()
{
	GridUI->UpdatePropGrid(SelectedMajorCategory->GetMajorCategoryType(), CategorySystem);
}

void UCategoryUI::OnImageSearchButtonResponse(const FString& ImgPath, bool bSuccess)
{
	FFastLogger::LogScreen(FColor::Red, TEXT("OnImageSearchButtonResponse : %s"), *ImgPath);
	if (!bSuccess)
	{
		SetTextToDefault();
		SetGridToDefault();
		return;
	}
	
	SendImageRequest(ImgPath);
	
	FString FilePath = FPaths::GetPath(ImgPath);
	FString FileNameWithExtension = FPaths::GetCleanFilename(ImgPath);
	SearchText->SetText(FText::FromString(FileNameWithExtension));
}

void UCategoryUI::ShowResultUI(bool bSuccess, const FString& ResultText)
{
	if (!SaveResultUI)
	{
		FFastLogger::LogConsole(TEXT("SaveResultUI is null"));
		return;
	}
	SaveResultUI->AddToViewport();
	SaveResultUI->SetResultImage(bSuccess);
	SaveResultUI->SetResultText(ResultText);
	SaveResultUI->PlayResultAnim();
}
