// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"


// Sets default values
AGrid::AGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrid::GetSlotsInRange(ASlotBase* Slot, int32 INT32, bool bCond, TArray<ASlotBase*>& Array, bool& AllFree)
{
}

void AGrid::GetSlotsInCharacterRange(ACharacterBase* AbilityOwner, int32 INT32, ASlotBase* Slot, bool bCond,
	TArray<ASlotBase*>& SlotsInRange, TArray<ASlotBase*>& EmptySlotsInRange, TArray<ACharacterBase*>& AlliesInRange,
	TArray<ACharacterBase*>& EnemiesInRange)
{
}

