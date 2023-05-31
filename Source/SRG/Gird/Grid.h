// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Slots/SlotBase.h"
#include "SRG/Units/Obstacles/ObstacleBase.h"
#include "SRGCore/SRGEnumStruct.h"
#include "Grid.generated.h"

class AEnemySpawnLocation;
class AHeroSpawnLocation;
class ASpellBase;
class AActiveAbilityBase;
class USplineComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCursorChanged, EBattleCursorType, InCursorType, AActiveAbilityBase*,
                                              InActiveAbility, ASpellBase*, InSpell, TSubclassOf<ACharacterBase>,
                                              InCharacter);

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

public:
	void InitializeEvent();

	UFUNCTION(BlueprintCallable, Category="Grid Event")
	void GetSlotsInRange(ASlotBase* Slot, int32 INT32, bool bCond, TArray<ASlotBase*>& Array, bool& AllFree);

	UFUNCTION(BlueprintCallable, Category="Grid Event")
	void GetSlotsInCharacterRange(ACharacterBase* AbilityOwner, int32 INT32, ASlotBase* Slot, bool bCond,
	                              TArray<ASlotBase*>& SlotsInRange, TArray<ASlotBase*>& EmptySlotsInRange,
	                              TArray<ACharacterBase*>& AlliesInRange, TArray<ACharacterBase*>& EnemiesInRange);

	void RemoveEnemySpawnLocation();

	bool SpawnCharacter(TSubclassOf<ACharacterBase> InUnitClass, FVector2D InCoordinates, bool IsAI, int32 InStack,
	                    const FHeroStats& InHeroStats, ACharacterBase*& SpawnedCharacter);

	void RemoveUnitOnSlot(ACharacterBase* InUnit);

	TArray<ASlotBase*> GetSlotsForObstacleSpawn();

	bool SpawnObstacle(TSubclassOf<AObstacleBase> InObstacle, FVector2D InCoordinates, AObstacleBase*& SpawnedObstacle);
	
	void ResetSlots();
	
	void CheckSlotUnderMouse();
	
	void ShowMovementForPlayerCharacter(ACharacterBase* InCharacter);
	
	void ClearMovementPath();
	
	void SetUnitOnSlot(ACharacterBase* InUnit, ASlotBase* InSlot);
	
	TMap<FVector2D,ASlotBase*> GetAllSlots(int32 InUnitSize);
	
	float GetDistanceToSlot(ASlotBase* Slot1, ASlotBase* Slot2);


	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY()
	USplineComponent* PathSpline;

	UPROPERTY()
	AHeroSpawnLocation* HeroSpawnLocation;

	UPROPERTY()
	TArray<AEnemySpawnLocation*> EnemySpawnLocations;

	UPROPERTY()
	TMap<FVector2D,ASlotBase*> Slots;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Grid|Delegate")
	FOnCursorChanged OnCursorChanged;
	
};
