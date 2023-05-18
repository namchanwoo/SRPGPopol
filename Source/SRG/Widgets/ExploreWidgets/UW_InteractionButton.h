#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_InteractionButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionButtonClicked, int32, ButtonIndex);

/**
 * 
 */
UCLASS()
class SRG_API UUW_InteractionButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;


	UFUNCTION()
	void OnButtonClicked_InteractionButton();

#pragma region               Designer

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UButton* InteractionButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* InteractionText;


#pragma endregion            Designer


#pragma region               InteractionButton

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionButton")
	FText ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionButton")
	int32 ButtonIndex;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="InteractionButton|Delegate")
	FOnInteractionButtonClicked OnInteractionButtonClicked;

#pragma endregion            InteractionButton
};
