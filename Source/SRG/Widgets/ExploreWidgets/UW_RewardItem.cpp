#include "SRG/Widgets/ExploreWidgets/UW_RewardItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

UUW_RewardItem::UUW_RewardItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_RewardItem::NativeConstruct()
{
	Super::NativeConstruct();

	ItemNameText->SetText(ItemName);

	ItemImage->SetBrushFromTexture(ItemIcon);

	ItemAmountText->SetText(ItemAmount);
}

void UUW_RewardItem::UpdateAmount(FText NewAmount)
{
	ItemAmountText->SetText(NewAmount);
}
