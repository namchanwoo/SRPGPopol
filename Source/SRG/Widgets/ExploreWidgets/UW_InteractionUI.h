#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_InteractionUI.generated.h"

class UVerticalBox;
class UUW_InteractionButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractClicked, int32, ButtonIndex);

/**
 * 
 */
UCLASS()
class SRG_API UUW_InteractionUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_InteractionUI(const FObjectInitializer& ObjectInitializer);
	
	void SetInteractionTexts(const TArray<FText>& InTexts);

	void ShowInteractionUI();

	void HideInteractionUI();

	void InstantHide();

public:
	UFUNCTION()
	void OnButtonClickedHandler(int32 ButtonIndex);

#pragma region               Designer

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UVerticalBox* InteractionButtonList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", Meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowAnimation;

#pragma endregion            Designer


#pragma region               InteractionUI

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionUI|Setting")
	TSubclassOf<UUW_InteractionButton> InteractionButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionUI")
	TArray<FText> InteractionTexts;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="InteractionUI|Delegate")
	FOnInteractClicked OnInteractClicked;
#pragma endregion            InteractionUI
};
