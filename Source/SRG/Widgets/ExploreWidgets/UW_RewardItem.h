#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_RewardItem.generated.h"

// class UVerticalBox;
class UTextBlock;
class UImage;
// class UHorizontalBox;
// class UOverlay;
// class USizeBox;
// class UButton;
// class UCanvasPanel;

/**
 * 
 */
UCLASS()
class SRG_API UUW_RewardItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_RewardItem(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateAmount(FText NewAmount);

#pragma region               Designer

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* ItemAmountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* ItemNameText;

#pragma endregion            Designer


#pragma region               RewardItem

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reward Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reward Item")
	FText ItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reward Item")
	UTexture2D* ItemIcon;

#pragma endregion            RewardItem
};
