// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_QuestNotificationUI.generated.h"

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



	/*---	Field Members(Variable)	---*/
#pragma region               Designer

public:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	// TObjectPtr<UCanvasPanel> CanvasPanel_Main;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", Meta = (BindWidgetAnim), Transient)


#pragma endregion            Designer


#pragma region               Properties


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestNotificationUI")
	AExploreHeroBase* ExploreHero;
#pragma endregion            Properties
	
};
