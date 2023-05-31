// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRGCore/SRGEnumStruct.h"
#include "UW_BattleResultUI.generated.h"

class AEquipmentBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleResultClosed,EBattleState,InResult);
/**
 * 
 */
UCLASS()
class SRG_API UUW_BattleResultUI : public UUserWidget
{
	GENERATED_BODY()

	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY()
	int32 ExpReward;

	UPROPERTY()
	int32 Gold;
	
	UPROPERTY()
	EBattleState Result;

	UPROPERTY()
	TArray<TSubclassOf<AEquipmentBase>> Drops;
	
	UPROPERTY()
	TMap<TSubclassOf<ACharacterBase>, int32> PlayerCasualties;
	
	UPROPERTY()
	TMap<TSubclassOf<ACharacterBase>, int32> EnemyCasualties;

	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="BattleResult|Delegate")
	FOnBattleResultClosed OnBattleResultClosed;
	
};
