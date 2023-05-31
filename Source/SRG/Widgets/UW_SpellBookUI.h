// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRG/Core/BattleController.h"
#include "SRG/Heros/BattleHeroBase.h"
#include "UW_SpellBookUI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpellClicked, ASpellBase*, InSpell);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpellBookClosed);

/**
 * 
 */
UCLASS()
class SRG_API UUW_SpellBookUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	ABattleHeroBase* BattleHero;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Spell Book UI|Delegate")
	FOnSpellClicked OnSpellClicked;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Spell Book UI|Delegate")
	FOnSpellBookClosed OnSpellBookClosed;
};
