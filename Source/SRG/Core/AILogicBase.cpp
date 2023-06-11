#include "AILogicBase.h"

#include "BattleController.h"
#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "SRG/Abilities/ActiveAbilities/AllyTargetActiveAbilityBase.h"
#include "SRG/Abilities/ActiveAbilities/AreaTargetActiveAbilityBase.h"
#include "SRG/Abilities/ActiveAbilities/EnemyTargetActiveAbility/EnemyTargetActiveAbilityBase.h"
#include "SRG/Gird/Grid.h"
#include "SRG/Gird/Slots/SlotBase.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "SRGCore/Libraries/SRPGUtilityLibrary.h"
#include "SRGCore/Utilities/SRGLog.h"


AAILogicBase::AAILogicBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAILogicBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAILogicBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAILogicBase::InitializeEvent(ABattleController* InBattleController, AGrid* InGrid)
{
	Grid = InGrid;
	BattleController = InBattleController;
}

void AAILogicBase::RunAILogic(ACharacterBase* InCharacter)
{
	SRPG_NOT_IMPLEMENTED();
	return;

	CurrentPlayingCharacter = InCharacter;
	ClosestSlotToMove = nullptr;
	WeakestEnemy = nullptr;

	CalculateMovableSlots(CurrentPlayingCharacter, Grid->bIsMeleeAttack, Grid->EnemySlots, Grid->AllySlots,
	                      ClosestSlotToMove);

	WeakestEnemy = GetWeakestEnemy(Grid->EnemySlots);

	CurrentAvailableAbility = GetActiveAbility();

	if (IsValid(CurrentAvailableAbility))
	{
		BattleController->OnActiveAbilityClicked(CurrentAvailableAbility);


		//AllyTarget
		AAllyTargetActiveAbilityBase* currentAllyTargetActiveAbilityBase =
			Cast<AAllyTargetActiveAbilityBase>(CurrentAvailableAbility);

		//AreaTarget
		AAreaTargetActiveAbilityBase* currentAreaTargetActiveAbilityBase =
			Cast<AAreaTargetActiveAbilityBase>(CurrentAvailableAbility);

		//EnemyTarget
		AEnemyTargetActiveAbilityBase* currentEnemyTargetActiveAbilityBase =
			Cast<AEnemyTargetActiveAbilityBase>(CurrentAvailableAbility);

		ASlotBase* targetSlot = IsValid(currentAreaTargetActiveAbilityBase)
			                        ? currentAreaTargetActiveAbilityBase->TargetSlot
			                        : currentAllyTargetActiveAbilityBase->TargetSlot;

		if (currentAllyTargetActiveAbilityBase || currentAreaTargetActiveAbilityBase)
		{
			BattleController->ActiveAbility(targetSlot);
			return;
		}

		//InstantCast
		if (!currentEnemyTargetActiveAbilityBase)
		{
			CurrentAvailableAbility->PlayAbilityAnimation();
			return;
		}
	}


	if (IsValid(WeakestEnemy))
	{
	}
}

void AAILogicBase::CalculateMovableSlots(ACharacterBase* InCharacter, bool& IsMeleeAttack,
                                         TArray<ASlotBase*>& OutEnemySlots, TArray<ASlotBase*>& OutAllySlots,
                                         ASlotBase*& OutClosetSlotToMove)
{
	SRPG_NOT_IMPLEMENTED();
}

ACharacterBase* AAILogicBase::GetWeakestEnemy(const TArray<ASlotBase*>& InEnemySlots)
{
	ACharacterBase* currentEnemy = nullptr;
	int32 currentDamage = 0;

	for (ASlotBase* InEnemySlot : InEnemySlots)
	{
		ACharacterBase* slotCharacter = Cast<ACharacterBase>(InEnemySlot->ContainedUnit);
		if (slotCharacter == nullptr) { continue; }

		int32 totalDamage;
		TMap<ACharacterBase*, FDamageData> damageData;
		TArray<ACharacterBase*> TargetCharacters{slotCharacter};

		BattleController->CalculateDamage(BattleController->PlayingCharacter, TargetCharacters,
		                                  BattleController->GetGrid()->bIsMeleeAttack, damageData, totalDamage);

		if (totalDamage >= currentDamage)
		{
			currentDamage = totalDamage;
			currentEnemy = slotCharacter;
		}
	}

	return currentEnemy;
}

AActiveAbilityBase* AAILogicBase::GetActiveAbility()
{
	TArray<AActiveAbilityBase*> availableAbilities;

	for (AActiveAbilityBase* InCurrentActiveAbility : CurrentPlayingCharacter->CurrentActiveAbilities)
	{
		if (InCurrentActiveAbility->IsAvailable(Grid->bIsMeleeAttack) == false) { continue; }

		InCurrentActiveAbility->ResetTargetSlot();
		InCurrentActiveAbility->SetTargetSlotForAI();

		TArray<ASlotBase*> outSlotsInRange;
		TArray<ASlotBase*> outEmptySlotsInRange;
		TArray<ACharacterBase*> outAlliesInRange;
		TArray<ACharacterBase*> outEnemiesInRang;
		InCurrentActiveAbility->GetAbilitySlots(outSlotsInRange, outEmptySlotsInRange, outAlliesInRange,
		                                        outEnemiesInRang);

		auto isAIUse = InCurrentActiveAbility->ShouldAIUse(outSlotsInRange, outEmptySlotsInRange, outAlliesInRange,
		                                                   outEnemiesInRang, WeakestEnemy);

		if (isAIUse == false) { continue; }

		availableAbilities.Add(InCurrentActiveAbility);
	}

	return USRPGUtilityLibrary::GetRandomElement(availableAbilities);
}
