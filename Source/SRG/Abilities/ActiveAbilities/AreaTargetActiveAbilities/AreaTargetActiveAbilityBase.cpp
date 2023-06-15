#include "AreaTargetActiveAbilityBase.h"

#include "SRG/Core/BattleController.h"
#include "SRG/Gird/Grid.h"
#include "SRG/Gird/Slots/SlotBase.h"
#include "SRGCore/Libraries/SRPGUtilityLibrary.h"


AAreaTargetActiveAbilityBase::AAreaTargetActiveAbilityBase()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다. 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = true;
}

void AAreaTargetActiveAbilityBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAreaTargetActiveAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAreaTargetActiveAbilityBase::OnActiveAbilityEnabled()
{
	BattleController->GetGrid()->OnAreaTargetAbilityEnabled(this);
}

void AAreaTargetActiveAbilityBase::OnActiveAbilityDisabled()
{
	BattleController->GetGrid()->OnAreaTargetAbilityDisabled();
}

void AAreaTargetActiveAbilityBase::SetTargetSlot(ASlotBase* InSlot)
{
	TargetSlot = InSlot;
}

void AAreaTargetActiveAbilityBase::SetTargetSlotForAI_Implementation()
{
	if (bHighlightAllies)
	{
		TArray<ACharacterBase*>& CharactersArray = AbilityOwner->bIsAI
			                                           ? BattleController->EnemyCharacters
			                                           : BattleController->PlayedCharacters;

		TargetSlot = USRPGUtilityLibrary::GetRandomElement<ACharacterBase*>(CharactersArray)->Slot;
		return;
	}

	if (bHighlightEnemies)
	{
		TArray<ACharacterBase*>& CharactersArray = AbilityOwner->bIsAI
			                                           ? BattleController->PlayedCharacters
			                                           : BattleController->EnemyCharacters;

		TargetSlot = USRPGUtilityLibrary::GetRandomElement<ACharacterBase*>(CharactersArray)->Slot;
		return;
	}

	TArray<ASlotBase*> SlotsValues;
	BattleController->GetGrid()->Slots.GenerateValueArray(SlotsValues);
	TargetSlot = USRPGUtilityLibrary::GetRandomElement<ASlotBase*>(SlotsValues);
}

void AAreaTargetActiveAbilityBase::ResetTargetSlot()
{
	SetTargetSlot(nullptr);
}

bool AAreaTargetActiveAbilityBase::ShouldAIUse_Implementation(const TArray<ASlotBase*>& SlotsInRange,
                                                              const TArray<ASlotBase*>& EmptySlotsInRange,
                                                              const TArray<ACharacterBase*>& AlliesInRange,
                                                              const TArray<ACharacterBase*>& EnemiesInRange,
                                                              ACharacterBase* WeakestEnemy)
{
	return IsValid(TargetSlot);
}

void AAreaTargetActiveAbilityBase::GetAbilitySlots_Implementation(TArray<ASlotBase*>& OutSlotsInRange,
                                                                  TArray<ASlotBase*>& OutEmptySlotsInRange,
                                                                  TArray<ACharacterBase*>& OutAlliesInRange,
                                                                  TArray<ACharacterBase*>& OutEnemiesInRange)
{
	TArray<ASlotBase*> currentSlotsInRange, currentEmptySlotsInRange;
	TArray<ACharacterBase*> currentAlliesInRange, currentEnemiesInRange;
	if (IsValid(TargetSlot))
	{
		bool AllFree;
		BattleController->GetGrid()->GetSlotsInRange(TargetSlot, Range, false, currentSlotsInRange, AllFree);

		for (ASlotBase* InSlot : currentSlotsInRange)
		{
			if (InSlot->IsEmpty())
			{
				currentEmptySlotsInRange.AddUnique(InSlot);
				continue;
			}

			ACharacterBase* OutCharacter;
			if (InSlot->IsEnemySlot(AbilityOwner, OutCharacter))
			{
				currentEnemiesInRange.AddUnique(OutCharacter);
				continue;
			}

			if (InSlot->IsAllySlot(AbilityOwner, OutCharacter))
			{
				currentAlliesInRange.AddUnique(OutCharacter);
				continue;
			}
		}
	}

	OutSlotsInRange = currentSlotsInRange;
	OutEmptySlotsInRange = currentEmptySlotsInRange;
	OutAlliesInRange = currentAlliesInRange;
	OutEnemiesInRange = currentEnemiesInRange;
}

