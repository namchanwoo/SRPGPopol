// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRG/Heros/BattleHeroBase.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "UW_BattleUI.generated.h"

class UUW_BossHP;
class UUW_ControlPanel;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewTurnAnimationEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDefendAction);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaitAction);

/**
 * 
 */
UCLASS()
class SRG_API UUW_BattleUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeEvent();
	void EnableActionButtons(bool IsCharacterWaiting);
	void ActivateControlPanel(bool IsOn);
	void DisableActionButtons();
	void DisableActiveAbilities();
	
	void ShowHeroDetails(ABattleHeroBase* InHero);
	
	void StartDeploymentPhase();
	void HideActionButtons();
	void HideActiveAbilityList();
	
	void ShowMessage(const FText& InMessage, bool IsWarning);
	void PlayNewTurnAnimation(const FText& NewTurnText);
	void UpdateBattleQueue();
	void HideBattleQueue();
	void HideCharacterDetails();
	void HideControlPanel();
	void ShowActionButtons();
	void ShowActiveAbilityList(ACharacterBase* CharacterBase);
	void OnBattleStarted();
	void ShowBattleQueue();

public:
	UPROPERTY()
	ABattleController* BattleController;

	UPROPERTY()
	UUW_ControlPanel* UW_ControlPanel;

	UPROPERTY()
	UUW_BossHP* UW_BossHP;;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle UI|Delegate")
	FOnNewTurnAnimationEnded OnNewTurnAnimationEnded;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle UI|Delegate")
	FOnDefendAction OnDefendAction;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle UI|Delegate")
	FOnWaitAction OnWaitAction;
	
};
