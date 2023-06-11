// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "UW_QuestNotificationUI.generated.h"

class AQuestBase;
class UUW_QuestNotificationListUI;
class AExploreHeroBase;

// class UVerticalBox;
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
class SRG_API UUW_QuestNotificationUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_QuestNotificationUI(const FObjectInitializer& ObjectInitializer);


	void ShowQuestNotification(TSubclassOf<AQuestBase> InQuest, FQuestStepData InQuestData);

#pragma region               Designer

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UUW_QuestNotificationListUI* WBP_QuestNotificationListUI;

#pragma endregion            Designer


#pragma region               Properties

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestNotificationUI", meta=(ExposeOnSpawn="true"))
	AExploreHeroBase* ExploreHero;

#pragma endregion            Properties
};
