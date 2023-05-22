#pragma once

#include "CoreMinimal.h"
#include "UW_ExploreWidget.h"
#include "Blueprint/UserWidget.h"
#include "SRGCore/SRGEnumStruct.h"
#include "UW_EnemyInfoDialogue.generated.h"


// class UVerticalBox;
// class UTextBlock;

// class UOverlay;
// class USizeBox;
// class UButton;
// class UCanvasPanel;

class UUW_RewardItem;
class UUW_MenuButton;
class UImage;
class UHorizontalBox;



/**
 * 
 */
UCLASS()
class SRG_API UUW_EnemyInfoDialogue : public UUW_ExploreWidget
{
	GENERATED_BODY()

public:
	UUW_EnemyInfoDialogue(const FObjectInitializer& ObjectInitializer);


	virtual void NativeConstruct() override;

public:
	TMap<TSubclassOf<ACharacterBase>, FVector2D> GetEnemyInfo();


	UFUNCTION()
void OnButtonClicked_ContinueButton();
	
	UFUNCTION()
	FEventReply OnMouseButtonDown_BackGround(FGeometry MyGeometry, const FPointerEvent& MouseEvent);


#pragma region               Designer

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UHorizontalBox* EnemyList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UImage* Background;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UUW_MenuButton* ContinueButton;
#pragma endregion            Designer


#pragma region               EnemyInfoDialogue

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyInfoDialogue")
	TSubclassOf<UUW_RewardItem> WBP_RewardItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyInfoDialogue")
	TArray<FEnemyCharacterData> EnemyCharacterList;

	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="EnemyInfoDialogue|Delegate")
	FOnContinueClicked OnContinueClicked;
#pragma endregion            EnemyInfoDialogue
};
