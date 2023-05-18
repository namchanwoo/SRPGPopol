#include "SRG/Widgets/ExploreWidgets/UW_MenuButton.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

UUW_MenuButton::UUW_MenuButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ButtonSize = FVector2D(250.0f, 50.0f);
}

void UUW_MenuButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(Text);
	
	UWidgetLayoutLibrary::SlotAsCanvasSlot(MenuButton)->SetSize(ButtonSize);
	
	FSlateFontInfo NewFontInfo = ButtonText->GetFont();
	NewFontInfo.Size = FontSize;
	ButtonText->SetFont(NewFontInfo);
}

void UUW_MenuButton::SetButtonText(FText InText)
{
	ButtonText->SetText(InText);
}

void UUW_MenuButton::OnClicked_MenuButton()
{
	if (OnButtonClicked.IsBound())
		OnButtonClicked.Broadcast();
}
