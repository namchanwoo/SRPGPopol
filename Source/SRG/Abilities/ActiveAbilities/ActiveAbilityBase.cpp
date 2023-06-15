#include "ActiveAbilityBase.h"


#include "SRG/Core/BattleController.h"
#include "SRG/Gird/Grid.h"
#include "SRG/StatusEffects/DeBuffs/DeBuffBase.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "SRGCore/Utilities/SRGLog.h"


AActiveAbilityBase::AActiveAbilityBase()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다. 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = false;
}

void AActiveAbilityBase::BeginPlay()
{
	Super::BeginPlay();

	SRPG_CHECK(BattleController);
	BattleController->OnNewTurn.AddDynamic(this, &AActiveAbilityBase::OnNewTurnHandler);

	SetCoolDown(bStartWithCooldown ? Cooldown + 1 : 0);
}

void AActiveAbilityBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	BattleController->OnNewTurn.RemoveDynamic(this, &AActiveAbilityBase::OnNewTurnHandler);
}

void AActiveAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TMap<ACharacterBase*, FDamageData> AActiveAbilityBase::GetAbilityDamage_Implementation(
	const TArray<ACharacterBase*>& InTargetCharacters)
{
	return {};
}

void AActiveAbilityBase::SetCoolDown(int32 NewCurrentCoolDown)
{
	if (NewCurrentCoolDown < 0)
	{
		SRPG_LOG_WARNING(TEXT("쿨다운을 음수 값으로 설정하려고 시도했습니다."));
		return;
	}

	if (NewCurrentCoolDown > Cooldown)
	{
		SRPG_LOG_WARNING(TEXT("현재 쿨다운을 최대 쿨다운보다 큰 값으로 설정하려고 했습니다."));
		return;
	}

	CurrentCooldown = NewCurrentCoolDown;
}

void AActiveAbilityBase::ReduceMana()
{
	if (AbilityOwner == nullptr)
	{
		SRPG_LOG_ERROR(TEXT("어빌리티 오너는 null입니다. 마나를 줄일 수 없습니다."));
		return;
	}

	if (BattleController == nullptr)
	{
		SRPG_LOG_ERROR(TEXT("BattleController는 null입니다. 유닛 능력에 마나를 사용해야 하는지 여부를 확인할 수 없습니다."));
		return;
	}

	if (BattleController->bUseManaForUnitAbilities)
	{
		AbilityOwner->ReduceMana(ManaCost);
	}
}

void AActiveAbilityBase::PlayAbilityAnimation()
{
	CurrentTargets.Empty();
	bAnimationHasEnded = false;
	bAbilityUsed = false;

	SetCoolDown(CurrentCooldown + 1);

	ReduceMana();

	if (OnActiveAbilityUseStarted.IsBound())
		OnActiveAbilityUseStarted.Broadcast();

	if (UseAbilityMontage.Contains(AbilityOwner->GetClass()))
	{
		AbilityOwner->OnAbilityUsed.AddDynamic(this, &AActiveAbilityBase::OnAbilityUsedHandler);
		AbilityOwner->OnAnimationEnded.AddDynamic(this, &AActiveAbilityBase::OnAbilityAnimationEnded);

		if (UseAbilityMontage.Contains(AbilityOwner->GetClass()))
		{
			const auto FindAbilityMontage = *UseAbilityMontage.Find(AbilityOwner->GetClass());
			AbilityOwner->PlayAnimationMontage(FindAbilityMontage);
		}
	}
	else
	{
		OnAnimationPlayed();
		OnAbilityAnimationEnded();
	}
}

void AActiveAbilityBase::OnAnimationPlayed()
{
	TArray<ASlotBase*> SlotsInRange;
	TArray<ASlotBase*> EmptySlotsInRange;
	TArray<ACharacterBase*> AlliesInRange;
	TArray<ACharacterBase*> EnemiesInRange;
	GetAbilitySlots(SlotsInRange, EmptySlotsInRange, AlliesInRange, EnemiesInRange);
	OnAbilityUsed(SlotsInRange, EmptySlotsInRange, AlliesInRange, EnemiesInRange);
}

void AActiveAbilityBase::PostAnimationLogic()
{
	if (bAbilityUsed)
	{
		UseOnKillPassiveAbilities();
	}
	else
	{
		bAnimationHasEnded = true;
	}
}

void AActiveAbilityBase::OnAbilityAnimationEnded()
{
	AbilityOwner->ClearAbilityCallback();

	FTimerHandle AbilityAnimationHandle; // 타이머의 핸들을 정의합니다.
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("PostAnimationLogic"));
	GetWorld()->GetTimerManager().SetTimer(AbilityAnimationHandle, TimerDel, AbilityAnimationDelay, false);
}


void AActiveAbilityBase::AddBuff(ACharacterBase* InAffectedCharacter, TSubclassOf<ABuffBase> InBuff,
                                 int32 InTurnDuration)
{
	AddEffect(InAffectedCharacter, InBuff, InTurnDuration, InAffectedCharacter->CurrentBuffs);
}

void AActiveAbilityBase::AddDeBuff(ACharacterBase* InAffectedCharacter, TSubclassOf<ADeBuffBase> InDeBuff,
                                   int32 InTurnDuration)
{
	AddEffect(InAffectedCharacter, InDeBuff, InTurnDuration, InAffectedCharacter->CurrentDeBuffs);
}

void AActiveAbilityBase::GetAbilitySlots_Implementation(TArray<ASlotBase*>& OutSlotsInRange,
                                                        TArray<ASlotBase*>& OutEmptySlotsInRange,
                                                        TArray<ACharacterBase*>& OutAlliesInRange,
                                                        TArray<ACharacterBase*>& OutEnemiesInRange)
{
	BattleController->GetGrid()->GetSlotsInCharacterRange(AbilityOwner, Range, AbilityOwner->Slot, false,
	                                                      OutSlotsInRange, OutEmptySlotsInRange,
	                                                      OutAlliesInRange, OutEnemiesInRange);
}


bool AActiveAbilityBase::GetAbilityDetails(int32& OutManaCost, int32& OutCoolDown)
{
	if (BattleController->bUseManaForUnitAbilities)
	{
		OutManaCost = ManaCost;
		OutCoolDown = Cooldown;
		return AbilityOwner->CurrentMana >= ManaCost;
	}

	OutManaCost = 0;
	OutCoolDown = Cooldown;
	return true;
}

void AActiveAbilityBase::UseOnKillPassiveAbilities()
{
	if (!BattleController->HasAnyTargetDied(CurrentTargets))
	{
		if (OnActiveAbilityUseEnded.IsBound())
			OnActiveAbilityUseEnded.Broadcast();
	}
	else
	{
		BattleController->OnAllPassiveAbilitiesUsed.AddDynamic(this, &AActiveAbilityBase::OnKillPassiveAbilitiesUsed);
		BattleController->UsePassiveAbilities(AbilityOwner, CurrentTargets, EPassiveAbilityUseMoment::OnKill);
	}
}

void AActiveAbilityBase::ActivateActiveAbility(bool IsActive)
{
	if (OnActiveAbilityActivated.IsBound())
		OnActiveAbilityActivated.Broadcast(IsActive);

	if (IsActive)
	{
		OnActiveAbilityEnabled();
	}
	else
	{
		OnActiveAbilityDisabled();
	}
}

void AActiveAbilityBase::OnActiveAbilityEnabled()
{
}

void AActiveAbilityBase::OnActiveAbilityDisabled()
{
}

void AActiveAbilityBase::SetTargetSlotForAI_Implementation()
{
}

void AActiveAbilityBase::ApplyStatusEffects(const TArray<ACharacterBase*>& AllyCharacters,
                                            const TArray<ACharacterBase*>& EnemyCharacters, bool IsBeforeAttack)
{
}

void AActiveAbilityBase::SetTargetSlot(ASlotBase* InSlot)
{
}

void AActiveAbilityBase::ResetTargetSlot()
{
}

void AActiveAbilityBase::ClearActiveAbilityUseCallbacks()
{
	OnActiveAbilityUseStarted.RemoveAll(this);
	OnActiveAbilityUseEnded.RemoveAll(this);
}

void AActiveAbilityBase::DamageTargets(const TMap<ACharacterBase*, FDamageData>& InDamageData)
{
	TMap<ACharacterBase*, FDamageData> ElementReduction = BattleController->GetElementReduction(Element, InDamageData);

	ElementReduction.GetKeys(CurrentTargets);

	for (ACharacterBase* CurrentTarget : CurrentTargets)
	{
		const FDamageData CurrentCharacterDamageData = *ElementReduction.Find(CurrentTarget);
		CurrentTarget->TakeDamageEvent(CurrentCharacterDamageData.Damage, CurrentCharacterDamageData.Element);
	}
}

bool AActiveAbilityBase::IsAvailable(bool IsMeleeAttack)
{
	if (BattleController->bUseManaForUnitAbilities)
	{
		return (Cooldown <= 0) && (AbilityOwner->CurrentMana >= ManaCost);
	}
	else
	{
		return Cooldown <= 0;
	}
}

bool AActiveAbilityBase::ShouldAIUse_Implementation(const TArray<ASlotBase*>& SlotsInRange,
                                                    const TArray<ASlotBase*>& EmptySlotsInRange,
                                                    const TArray<ACharacterBase*>& AlliesInRange,
                                                    const TArray<ACharacterBase*>& EnemiesInRange,
                                                    ACharacterBase* WeakestEnemy)
{
	return false;
}


void AActiveAbilityBase::EndTurn()
{
	UWorld* World = GetWorld();
	if (!World) return;
	auto& TimerManager = World->GetTimerManager();

	if (bAnimationHasEnded)
	{
		if (TimerManager.IsTimerActive(EndTurnTimerHandle))
		{
			TimerManager.ClearTimer(EndTurnTimerHandle);
		}
		TimerManager.SetTimer(EndTurnTimerHandle, this, &AActiveAbilityBase::OnEndTurnHandle, 2.0f, false);
	}
	else
	{
		bAbilityUsed = true;
	}
}

void AActiveAbilityBase::OnEndTurnHandle()
{
	UseOnKillPassiveAbilities();
}

void AActiveAbilityBase::OnNewTurnHandler(int32 NewTurn)
{
	SetCoolDown(FMath::Max(CurrentCooldown - 1, 0));
}

void AActiveAbilityBase::OnAbilityUsed_Implementation(const TArray<ASlotBase*>& InSlotsInRange,
                                                      const TArray<ASlotBase*>& InEmptySlotsInRange,
                                                      const TArray<ACharacterBase*>& InAlliesInRange,
                                                      const TArray<ACharacterBase*>& InEnemiesInRange)
{
	bAbilityUsed = true;

	for (TTuple<TSubclassOf<ABuffBase>, int32> InBuff : Buffs)
	{
		for (ACharacterBase* InCharacter : InAlliesInRange)
		{
			if (InCharacter->IsDead()) { continue; }
			AddBuff(InCharacter, InBuff.Key, InBuff.Value);
		}
	}

	for (TTuple<TSubclassOf<ADeBuffBase>, int32> InDeBuff : DeBuffs)
	{
		for (ACharacterBase* InCharacter : InEnemiesInRange)
		{
			if (InCharacter->IsDead()) { continue; }
			AddDeBuff(InCharacter, InDeBuff.Key, InDeBuff.Value);
		}
	}
}

void AActiveAbilityBase::OnAbilityUsedHandler()
{
	OnAnimationPlayed();
}

void AActiveAbilityBase::OnKillPassiveAbilitiesUsed()
{
	BattleController->ClearPassiveAbilityCallback();

	if (OnActiveAbilityUseEnded.IsBound())
		OnActiveAbilityUseEnded.Broadcast();
}
