#pragma once

#include "CoreMinimal.h"
#include "UW_ExploreWidget.h"
#include "Blueprint/UserWidget.h"
#include "UW_InfoDialogue.generated.h"

// class UVerticalBox;
// class UHorizontalBox;
// class UOverlay;
// class USizeBox;
// class UCanvasPanel;

class UImage;
class UUW_MenuButton;
class UTextBlock;
class UButton;



/**
 * 
 */
UCLASS()
class SRG_API UUW_InfoDialogue : public UUW_ExploreWidget
{
	GENERATED_BODY()

public:
	UUW_InfoDialogue(const FObjectInitializer& ObjectInitializer);


	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;


#pragma region   	Field Members

public:
	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	FEventReply OnBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

#pragma endregion	Field Members

	/*---	Field Members(Variable)	---*/
#pragma region               Designer

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UImage* Background;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UUW_MenuButton* OkButton;

#pragma endregion            Designer


#pragma region               Properties

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info Dialogue")
	FText DialogueTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info Dialogue")
	FText DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info Dialogue")
	FText YesButtonText;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Info Dialogue|Delegate")
	FOnOkClicked OnOkClicked;
#pragma endregion            Properties
};
