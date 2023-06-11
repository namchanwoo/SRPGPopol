#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "UW_QuestNotificationListUI.generated.h"

class UUW_QuestNotification;
class AExploreHeroBase;
class AQuestBase;

class UVerticalBox;
// class UTextBlock;
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
class SRG_API UUW_QuestNotificationListUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_QuestNotificationListUI(const FObjectInitializer& ObjectInitializer);

	void ShowQuestNotification(TSubclassOf<AQuestBase> InQuest, FQuestStepData InQuestData, AExploreHeroBase* InExploreHero);


#pragma region               Designer

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UVerticalBox* NotificationList;

#pragma endregion            Designer


#pragma region               QuestNotificationListUI

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	TSubclassOf<UUW_QuestNotification> QuestNotificationClass;
#pragma endregion            QuestNotificationListUI
};
