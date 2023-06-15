#include "EnemyTargetActiveAbilityBase.h"

#include "SRG/Abilities/MultiTargetPassiveAbilities/MultiTargetPassiveAbilityBase.h"
#include "SRG/Core/BattleController.h"
#include "SRG/Gird/Grid.h"
#include "SRGCore/Utilities/SRGLog.h"


AEnemyTargetActiveAbilityBase::AEnemyTargetActiveAbilityBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyTargetActiveAbilityBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyTargetActiveAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AEnemyTargetActiveAbilityBase::IsAvailable(bool IsMeleeAttack)
{
	return Super::IsAvailable(IsMeleeAttack) &&
		(IsMeleeAttack == bIsMelee) &&
		(BattleController->GetGrid()->EnemySlots.Num() > 0);
}

void AEnemyTargetActiveAbilityBase::OnActiveAbilityEnabled()
{
	if (IsValid(MultiTargetAbility))
	{
		DefaultMultiTargetAbility = AbilityOwner->CurrentMultiTargetPassiveAbility;

		UWorld* World = GetWorld();
		SRPG_CHECK(World);
		const FTransform SpawnTransform;
		CurrentMultiTargetAbility = World->SpawnActorDeferred<AMultiTargetPassiveAbilityBase>(
			MultiTargetAbility, SpawnTransform);
		if (CurrentMultiTargetAbility)
		{
			CurrentMultiTargetAbility->AbilityOwner = AbilityOwner;
			CurrentMultiTargetAbility->BattleController = BattleController;
			UGameplayStatics::FinishSpawningActor(CurrentMultiTargetAbility, SpawnTransform);
			AbilityOwner->CurrentMultiTargetPassiveAbility = CurrentMultiTargetAbility;
		}
	}
}

void AEnemyTargetActiveAbilityBase::OnActiveAbilityDisabled()
{
	if (IsValid(MultiTargetAbility))
	{
		CurrentMultiTargetAbility->Destroy();
		AbilityOwner->CurrentMultiTargetPassiveAbility = DefaultMultiTargetAbility;
	}
}

void AEnemyTargetActiveAbilityBase::PlayAbilityAnimation()
{
	SetCoolDown(Cooldown + 1);
	ReduceMana();
}

void AEnemyTargetActiveAbilityBase::ApplyStatusEffects(const TArray<ACharacterBase*>& AllyCharacters,
                                                       const TArray<ACharacterBase*>& EnemyCharacters,
                                                       bool IsBeforeAttack)
{
	if (IsBeforeAttack == bRemoveStatusEffectAfterAttack)
	{
		const TArray<ASlotBase*> SlotArray;
		OnAbilityUsed(SlotArray, SlotArray, AllyCharacters, EnemyCharacters);
	}
}
