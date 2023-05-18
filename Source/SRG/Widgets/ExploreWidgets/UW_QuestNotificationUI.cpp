// Fill out your copyright notice in the Description page of Project Settings.


#include "SRG/Widgets/ExploreWidgets/UW_QuestNotificationUI.h"

#include "UW_QuestNotificationListUI.h"

UUW_QuestNotificationUI::UUW_QuestNotificationUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_QuestNotificationUI::ShowQuestNotification(TSubclassOf<AQuestBase> InQuest, FQuestStepData InQuestData)
{
	WBP_QuestNotificationListUI->ShowQuestNotification(InQuest, InQuestData, ExploreHero);
}
