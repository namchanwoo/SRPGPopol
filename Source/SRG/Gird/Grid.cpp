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

void AGrid::InitializeEvent()
{
}

void AGrid::GetSlotsInRange(ASlotBase* Slot, int32 INT32, bool bCond, TArray<ASlotBase*>& Array, bool& AllFree)
{
}

void AGrid::GetSlotsInCharacterRange(ACharacterBase* AbilityOwner, int32 INT32, ASlotBase* Slot, bool bCond,
                                     TArray<ASlotBase*>& SlotsInRange, TArray<ASlotBase*>& EmptySlotsInRange,
                                     TArray<ACharacterBase*>& AlliesInRange,
                                     TArray<ACharacterBase*>& EnemiesInRange)
{
}

void AGrid::RemoveEnemySpawnLocation()
{
}

bool AGrid::SpawnCharacter(TSubclassOf<ACharacterBase> InUnitClass, FVector2D InCoordinates, bool IsAI, int32 InStack,
                           const FHeroStats& InHeroStats, ACharacterBase*& SpawnedCharacter)
{
	return false;
}

void AGrid::RemoveUnitOnSlot(ACharacterBase* InUnit)
{
}

TArray<ASlotBase*> AGrid::GetSlotsForObstacleSpawn()
{
	return {};
}

bool AGrid::SpawnObstacle(TSubclassOf<AObstacleBase> InObstacle, FVector2D InCoordinates,
                          AObstacleBase*& SpawnedObstacle)
{
	return false;
}

void AGrid::ResetSlots()
{
}

void AGrid::CheckSlotUnderMouse()
{
}

void AGrid::ShowMovementForPlayerCharacter(ACharacterBase* InCharacter)
{
}

void AGrid::ClearMovementPath()
{
}

void AGrid::SetUnitOnSlot(ACharacterBase* InUnit, ASlotBase* InSlot)
{
}

TMap<FVector2D, ASlotBase*> AGrid::GetAllSlots(int32 InUnitSize)
{
	return {};
}

float AGrid::GetDistanceToSlot(ASlotBase* Slot1, ASlotBase* Slot2)
{
	return 0;
}
