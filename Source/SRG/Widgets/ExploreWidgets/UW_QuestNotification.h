#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "UW_QuestNotification.generated.h"

class AExploreHeroBase;
class UProgressBar;
class AQuestBase;
class UTextBlock;
// class UVerticalBox;
// class UImage;
// class UHorizontalBox;
// class UOverlay;
// class USizeBox;
// class UButton;
// class UCanvasPanel;


/**
 * 
 */
UCLASS()
class SRG_API UUW_QuestNotification : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_QuestNotification(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	void PlayAppearAnimation();

	void PlayDisappearAnimation();

	void PlayQuestSound(USoundBase* InSound);

	void SetQuestStatus();

	void NewQuest();

	void UpdateQuest();

	void CompletedQuest();


#pragma region               Designer

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* ProgressDetailsText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* QuestStatusText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UProgressBar* QuestBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", Meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Appear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", Meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Disappear;


#pragma endregion            Designer


#pragma region               QuestNotification

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Notification", meta=(ExposeOnSpawn="true"))
	FQuestStepData QuestStepData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Notification", meta=(ExposeOnSpawn="true"))
	TSubclassOf<AQuestBase> Quest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer")
	AExploreHeroBase* ExploreHero;
#pragma endregion            QuestNotification
};
