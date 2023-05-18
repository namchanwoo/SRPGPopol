#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MenuButton.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

/**
 * 
 */
UCLASS()
class SRG_API UUW_MenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_MenuButton(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void SetButtonText(FText InText);

	UFUNCTION()
	void OnClicked_MenuButton();

#pragma region               Designer

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UButton* MenuButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* ButtonText;


#pragma endregion            Designer


#pragma region               MenuButton

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuButton")
	float FontSize = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuButton")
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuButton")
	FVector2D ButtonSize;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="MenuButton|DelegateEvent")
	FOnButtonClicked OnButtonClicked;


#pragma endregion            MenuButton
};
