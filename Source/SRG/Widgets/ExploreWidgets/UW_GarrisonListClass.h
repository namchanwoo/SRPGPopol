// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_ExploreWidget.h"
#include "UW_GarrisonListClass.generated.h"


/**
 * 
 */
UCLASS()
class SRG_API UUW_GarrisonListClass : public UUW_ExploreWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Garrision List|Delegate")
	FOnContinueClicked OnContinueClicked;
};
