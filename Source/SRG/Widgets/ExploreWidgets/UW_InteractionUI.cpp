#include "SRG/Widgets/ExploreWidgets/UW_InteractionUI.h"

#include "SRGCore/AssetTableRef.h"
#include "UW_InteractionButton.h"
#include "Components/VerticalBox.h"

UUW_InteractionUI::UUW_InteractionUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (const TSubclassOf<UUW_InteractionButton> WBP_InteractionButtonClassSrc = DT::FindClass<UUW_InteractionButton>(
		DT_BLUEPRINT_PATH, FName(TEXT("WBP_InteractionButton"))))
	{
		WBP_InteractionButtonClass = WBP_InteractionButtonClassSrc;
	}
}

void UUW_InteractionUI::SetInteractionTexts(const TArray<FText>& InTexts)
{
	if (!WBP_InteractionButtonClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("WBP_InteractionButtolClass가 nullptr입니다. 위젯을 생성 할 수 없습니다."))
		return;
	}

	InteractionButtonList->ClearChildren();
	for (int32 i = 0; i < InTexts.Num(); i++)
	{
		UUW_InteractionButton* NewInteractionButton =
			CreateWidget<UUW_InteractionButton>(GetWorld(), WBP_InteractionButtonClass);
		if (!NewInteractionButton) return;

		NewInteractionButton->ButtonIndex =i;
		NewInteractionButton->ButtonText = InTexts[i];
		NewInteractionButton->OnInteractionButtonClicked.AddDynamic(this, &UUW_InteractionUI::OnButtonClickedHandler);

		InteractionButtonList->AddChild(NewInteractionButton);
	}
}

void UUW_InteractionUI::ShowInteractionUI()
{
	if (ShowAnimation)
	{
		PlayAnimation(ShowAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
}

void UUW_InteractionUI::HideInteractionUI()
{
	if (ShowAnimation)
	{
		PlayAnimation(ShowAnimation, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f);
	}
}

void UUW_InteractionUI::InstantHide()
{
	InteractionButtonList->SetVisibility(ESlateVisibility::Collapsed);
	SetRenderOpacity(0.0f);
}

void UUW_InteractionUI::OnButtonClickedHandler(int32 ButtonIndex)
{
	if (OnInteractClicked.IsBound())
	{
		OnInteractClicked.Broadcast(ButtonIndex);
	}
}
