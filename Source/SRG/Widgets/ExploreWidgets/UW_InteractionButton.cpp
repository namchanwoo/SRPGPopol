#include "SRG/Widgets/ExploreWidgets/UW_InteractionButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUW_InteractionButton::NativeConstruct()
{
	Super::NativeConstruct();

	InteractionText->SetText(ButtonText);

	InteractionButton->OnClicked.AddDynamic(this, &UUW_InteractionButton::OnButtonClicked_InteractionButton);
}

void UUW_InteractionButton::OnButtonClicked_InteractionButton()
{
	if (OnInteractionButtonClicked.IsBound())
		OnInteractionButtonClicked.Broadcast(ButtonIndex);
}
