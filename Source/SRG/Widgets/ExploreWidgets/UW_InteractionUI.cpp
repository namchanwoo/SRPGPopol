#include "SRG/Widgets/ExploreWidgets/UW_InteractionUI.h"

#include "UW_InteractionButton.h"
#include "Components/VerticalBox.h"
#include "SRGCore/AssetTableRef.h"

UUW_InteractionUI::UUW_InteractionUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TSubclassOf<UUW_InteractionButton> InteractionButtonClassSrc = DT::FindClass<UUW_InteractionButton>(
		DT_BLUEPRINT_PATH, FName(TEXT("WBP_InteractionButton")));

	if (InteractionButtonClassSrc)
	{
		InteractionButtonClass = InteractionButtonClassSrc;
	}
}

void UUW_InteractionUI::SetInteractionTexts(const TArray<FText>& InTexts)
{
	for (int32 i = 0; i < InTexts.Num(); i++)
	{
		UUW_InteractionButton* NewInteractionButton = CreateWidget<UUW_InteractionButton>(GetWorld(), InteractionButtonClass);
		NewInteractionButton->ButtonIndex = i;
		NewInteractionButton->ButtonText = InTexts[i];
		InteractionButtonList->AddChild(NewInteractionButton);

		NewInteractionButton->OnInteractionButtonClicked.AddDynamic(this, &UUW_InteractionUI::OnButtonClickedHandler);
	}
}

void UUW_InteractionUI::ShowInteractionUI()
{
	PlayAnimationForward(ShowAnimation);
}

void UUW_InteractionUI::HideInteractionUI()
{
	PlayAnimationReverse(ShowAnimation);
}

void UUW_InteractionUI::InstantHide()
{
	InteractionButtonList->SetVisibility(ESlateVisibility::Collapsed);
	SetRenderOpacity(0.0f);
}

void UUW_InteractionUI::OnButtonClickedHandler(int32 ButtonIndex)
{
	if (OnInteractClicked.IsBound())
		OnInteractClicked.Broadcast(ButtonIndex);
}
