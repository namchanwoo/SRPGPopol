#include "AllyTargetActiveAbilityBase.h"

#include "SRG/Core/BattleController.h"
#include "SRG/Gird/Grid.h"
#include "SRGCore/Libraries/SRPGUtilityLibrary.h"


AAllyTargetActiveAbilityBase::AAllyTargetActiveAbilityBase()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다. 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = true;

	Range = 0;
}

void AAllyTargetActiveAbilityBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAllyTargetActiveAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAllyTargetActiveAbilityBase::OnActiveAbilityEnabled()
{
	BattleController->GetGrid()->OnAllyTargetAbilityEnabled();
}

void AAllyTargetActiveAbilityBase::OnActiveAbilityDisabled()
{
	BattleController->GetGrid()->OnAllyTargetAbilityDisabled();
}

void AAllyTargetActiveAbilityBase::SetTargetSlot(ASlotBase* InSlot)
{
	TargetSlot = InSlot;
}

void AAllyTargetActiveAbilityBase::SetTargetSlotForAI_Implementation()
{
	TargetSlot = USRPGUtilityLibrary::GetRandomElement<ASlotBase*>(BattleController->GetGrid()->AllySlots);
}

void AAllyTargetActiveAbilityBase::ResetTargetSlot()
{
	SetTargetSlot(nullptr);
}

void AAllyTargetActiveAbilityBase::GetAbilitySlots_Implementation(TArray<ASlotBase*>& OutSlotsInRange,
                                                                  TArray<ASlotBase*>& OutEmptySlotsInRange,
                                                                  TArray<ACharacterBase*>& OutAlliesInRange,
                                                                  TArray<ACharacterBase*>& OutEnemiesInRange)
{
	TArray<ACharacterBase*> CurrentAllies;
	if (IsValid(TargetSlot))
	{
		bool AllFree;
		TArray<ASlotBase*> l_SlotsInRange;
		BattleController->GetGrid()->GetSlotsInRange(TargetSlot, Range, false, l_SlotsInRange, AllFree);

		for (ASlotBase* InSlotsInRange : l_SlotsInRange)
		{
			ACharacterBase* outAllyCharacter;
			if (InSlotsInRange->IsAllySlot(AbilityOwner, outAllyCharacter))
			{
				CurrentAllies.AddUnique(outAllyCharacter);
			}
		}
	}

	OutAlliesInRange = CurrentAllies;
}

bool AAllyTargetActiveAbilityBase::IsAvailable(bool IsMeleeAttack)
{
	return Super::IsAvailable(IsMeleeAttack) && BattleController->GetGrid()->AllySlots.Num() > 0;
}
