// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_YesNoDialogue.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnYesClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoClicked);
/**
 * 
 */
UCLASS()
class SRG_API UUW_YesNoDialogue : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="YesNoDialogue")
	FText DialogueTitle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="YesNoDialogue")
	FText DialogueText;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="YesNoDialogue")
	FText YesButtonText;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="YesNoDialogue")
	FText NoButtonText;

	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="YesNoDialogue|Delegate")
	FOnYesClicked OnYesClicked;

	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="YesNoDialogue|Delegate")
	FOnNoClicked OnNoClicked;
};
