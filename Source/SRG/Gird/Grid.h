// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Slots/SlotBase.h"
#include "Grid.generated.h"

class USplineComponent;

UCLASS()
class SRG_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetSlotsInRange(ASlotBase* Slot, int32 INT32, bool bCond, TArray<ASlotBase*>& Array, bool& AllFree);

	void GetSlotsInCharacterRange(ACharacterBase* AbilityOwner, int32 INT32, ASlotBase* Slot, bool bCond,
	                              TArray<ASlotBase*>& SlotsInRange, TArray<ASlotBase*>& EmptySlotsInRange,
	                              TArray<ACharacterBase*>& AlliesInRange, TArray<ACharacterBase*>& EnemiesInRange);


	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY()
	USplineComponent* PathSpline;
};
