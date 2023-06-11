// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "UW_BattleCursor.generated.h"

class UHorizontalBox;
class UImage;
class ACharacterBase;
class AActiveAbilityBase;
class ASpellBase;

/**
 * 
 */
UCLASS()
class SRG_API UUW_BattleCursor : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_BattleCursor(const FObjectInitializer& ObjectInitializer);

public:
	void ChangeCursor(EBattleCursorType InCursorType, AActiveAbilityBase* InActiveAbility, ASpellBase* InSpell,
	                  TSubclassOf<ACharacterBase> InCharacter);

	void SetCursorAngle(float InAngle);

	void SetCursorImage(AActiveAbilityBase* InActiveAbility, ASpellBase* InSpell,
					  TSubclassOf<ACharacterBase> InCharacter);


	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Designer", meta = (BindWidget))
	UHorizontalBox* Container;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Designer", meta = (BindWidget))
	UImage* CursorImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Designer", meta = (BindWidget))
	UImage* Image;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Cursor|Setting")
	TMap<EBattleCursorType,UTexture2D*> Cursors;
};
