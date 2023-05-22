// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRG/Enemies/EnemyExplorePawnBase.h"
#include "UW_LevelUpDialogue.generated.h"



/**
 * 구현해야합니다.
 */
UCLASS()
class SRG_API UUW_LevelUpDialogue : public UUserWidget
{
	GENERATED_BODY()
public:

	
	/*******************************************
	 * Level Up Dialogue
	 *******************************************/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Level Up Dialogue")
	AExploreHeroBase* ExploreHero;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Level Up Dialogue")
	FHeroStats GainedStats;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Level Up Dialogue")
	int32 LevelUpAmount;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Level Up Dialogue")
	int32 CurrentLevel;

	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Delegate Event")
	FOnContinueClicked OnContinueClicked;
};
