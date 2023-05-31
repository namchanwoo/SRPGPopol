// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ActionButton.generated.h"

/**
 * 
 */
UCLASS()
class SRG_API UUW_ActionButton : public UUserWidget
{
	GENERATED_BODY()


public:
	void ActivateButton(bool IsActivated);
};
