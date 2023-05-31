// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "UW_BossHP.generated.h"

/**
 * 
 */
UCLASS()
class SRG_API UUW_BossHP : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeEvent(ACharacterBase* Boss);
};
