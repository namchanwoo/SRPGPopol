#include "SRG/Widgets/ExploreWidgets/UW_QuestNotificationListUI.h"

#include "UW_QuestNotification.h"
#include "Components/VerticalBox.h"
#include "SRGCore/AssetTableRef.h"

UUW_QuestNotificationListUI::UUW_QuestNotificationListUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	auto WBP_QuestNotification = DT::FindClass<UUW_QuestNotification>(DT_BLUEPRINT_PATH, FName(TEXT("WBP_QuestNotification")));
	if (WBP_QuestNotification)
	{
		QuestNotificationClass = WBP_QuestNotification;
	}
}

void UUW_QuestNotificationListUI::ShowQuestNotification(TSubclassOf<AQuestBase> InQuest, FQuestStepData InQuestData, AExploreHeroBase* InExploreHero)
{
	if (!QuestNotificationClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("QuestNotificationClass가 없습니다."));
		return;
	}

	UUW_QuestNotification* NewQuestNotification = CreateWidget<UUW_QuestNotification>(GetWorld(), QuestNotificationClass);
	NewQuestNotification->Quest = InQuest;
	NewQuestNotification->QuestStepData = InQuestData;
	NewQuestNotification->ExploreHero = InExploreHero;

	NotificationList->AddChild(NewQuestNotification);
}
