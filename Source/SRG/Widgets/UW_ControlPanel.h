// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ControlPanel.generated.h"

class UUW_ActionButton;
/**
 * 
 */
UCLASS()
class SRG_API UUW_ControlPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UUW_ActionButton* UW_SpellBookButton;
};
