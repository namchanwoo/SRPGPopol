#include "PassiveAbilityBase.h"

#include "Kismet/GameplayStatics.h"
#include "SRG/Core/BattleController.h"
#include "SRG/Gird/Grid.h"
#include "SRG/StatusEffects/Buffs/BuffBase.h"
#include "SRG/StatusEffects/DeBuffs/DeBuffBase.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "SRGCore/Utilities/SRGLog.h"


APassiveAbilityBase::APassiveAbilityBase()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다. 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = true;

	UseMoment = EPassiveAbilityUseMoment::OnTurnStart;
	PassiveAbilityTarget = EPassiveAbilityTarget::Self;
}

void APassiveAbilityBase::BeginPlay()
{
	Super::BeginPlay();
}

void APassiveAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APassiveAbilityBase::PlayAbilityAnimation()
{
	if (UseAbilityMontage.Contains(AbilityOwner->GetClass()))
	{
		UAnimMontage* FindAbilityMontage = *UseAbilityMontage.Find(AbilityOwner->GetClass());
		if (!IsValid(FindAbilityMontage)) { return; }

		AbilityOwner->OnAbilityUsed.AddDynamic(this, &APassiveAbilityBase::OnAbilityUsedHandler);
		AbilityOwner->OnAnimationEnded.AddDynamic(this, &APassiveAbilityBase::OnAbilityAnimationEnded);

		AbilityOwner->PlayAnimationMontage(FindAbilityMontage);
	}
	else
	{
		OnAnimationPlayed();
		OnAbilityAnimationEnded();
	}
}

void APassiveAbilityBase::OnAnimationPlayed()
{
	TArray<ASlotBase*> OutSlotsInRange, OutEmptySlotsInRange;
	TArray<ACharacterBase*> OutAlliesInRange, OutEnemiesInRange;
	BattleController->GetGrid()->GetSlotsInCharacterRange(AbilityOwner, Range, AbilityOwner->Slot, false,
	                                                      OutSlotsInRange, OutEmptySlotsInRange,
	                                                      OutAlliesInRange, OutEnemiesInRange);

	OnAbilityUsed(OutSlotsInRange, OutEmptySlotsInRange, OutAlliesInRange, OutEnemiesInRange);
}

void APassiveAbilityBase::OnAbilityAnimationEnded()
{
	AbilityOwner->ClearAbilityCallback();

	// 타이머의 핸들을 정의하십시오.
	FTimerHandle AbilityAnimationEndedTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AbilityAnimationEndedTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if (OnPassiveAbilityUseEnded.IsBound())
			OnPassiveAbilityUseEnded.Broadcast();

		GetWorld()->GetTimerManager().ClearTimer(AbilityAnimationEndedTimerHandle);
	}), AbilityAnimationDelay, false);
}

void APassiveAbilityBase::UsePassiveAbility(const TArray<ACharacterBase*> InHitCharacters)
{
	HitCharacters = InHitCharacters;

	PlayAbilityAnimation();
}

void APassiveAbilityBase::OnAbilityUsed_Implementation(const TArray<ASlotBase*>& InSlotsInRange,
	const TArray<ASlotBase*>& InEmptySlotsInRange, const TArray<ACharacterBase*>& InAlliesInRange,
	const TArray<ACharacterBase*>& InEnemiesInRange)
{
	for (TTuple<TSubclassOf<ABuffBase>, int> Buff : Buffs)
	{
		if (PassiveAbilityTarget == EPassiveAbilityTarget::Self)
		{
			AddBuff(AbilityOwner, Buff.Key, Buff.Value);
			return;
		}
		if (PassiveAbilityTarget == EPassiveAbilityTarget::CharactersInRange)
		{
			for (ACharacterBase* AlliesInRange : InAlliesInRange)
			{
				if (AlliesInRange->IsDead()) { continue; }
				AddBuff(AlliesInRange, Buff.Key, Buff.Value);
			}
			return;
		}
	}

	for (TTuple<TSubclassOf<ADeBuffBase>, int> DeBuff : DeBuffs)
	{
		if (PassiveAbilityTarget == EPassiveAbilityTarget::CharactersInRange)
		{
			for (ACharacterBase* EnemiesInRange : InEnemiesInRange)
			{
				if (EnemiesInRange->IsDead()) { continue; }
				AddDeBuff(EnemiesInRange, DeBuff.Key, DeBuff.Value);
			}
			return;
		}
		if (PassiveAbilityTarget == EPassiveAbilityTarget::HitCharacters)
		{
			for (ACharacterBase* HitCharacter : HitCharacters)
			{
				if (!IsValid(HitCharacter)) { continue; }
				if (HitCharacter->IsDead()) { continue; }
				AddDeBuff(HitCharacter, DeBuff.Key, DeBuff.Value);
			}
		}
	}
}

void APassiveAbilityBase::ClearPassiveAbilityUseCallback()
{
	if (OnPassiveAbilityUseEnded.IsBound())
		OnPassiveAbilityUseEnded.Broadcast();
}

void APassiveAbilityBase::AddBuff(ACharacterBase* InAffectedCharacter, TSubclassOf<ABuffBase> InBuff,
                                  int InTurnDuration)
{
	UWorld* World = GetWorld();
	SRPG_CHECK(World);

	FTransform SpawnTransform;
	if (ABuffBase* NewBuff = World->SpawnActorDeferred<ABuffBase>(InBuff, SpawnTransform))
	{
		NewBuff->AffectedCharacter = InAffectedCharacter;
		NewBuff->TurnDuration = InTurnDuration;
		NewBuff->bRemoveAfterAttack = UseMoment == EPassiveAbilityUseMoment::BeforeAttack;
		NewBuff->bIsFromAura = false;
		NewBuff->CasterCharacter = AbilityOwner;
		UGameplayStatics::FinishSpawningActor(NewBuff, SpawnTransform);


		ABuffBase* ExistingBuff;
		bool bAlreadyGotBuff = InAffectedCharacter->AlreadyGotBuff(NewBuff, ExistingBuff);

		if (!bAlreadyGotBuff)
		{
			InAffectedCharacter->AddBuff(NewBuff);
			NewBuff->InitializeEvent();
		}
		else
		{
			if (NewBuff->TurnDuration <= ExistingBuff->TurnDuration)
			{
				NewBuff->Destroy();
			}
			else
			{
				ExistingBuff->RemoveStatusEffect();
				InAffectedCharacter->AddBuff(NewBuff);
				NewBuff->InitializeEvent();
			}
		}
	}
}

void APassiveAbilityBase::AddDeBuff(ACharacterBase* InAffectedCharacter, TSubclassOf<ADeBuffBase> InDeBuff,
                                    int InTurnDuration)
{
	UWorld* World = GetWorld();
	SRPG_CHECK(World);

	FTransform SpawnTransform;
	if (ADeBuffBase* NewDeBuff = World->SpawnActorDeferred<ADeBuffBase>(InDeBuff, SpawnTransform))
	{
		NewDeBuff->AffectedCharacter = InAffectedCharacter;
		NewDeBuff->TurnDuration = InTurnDuration;
		NewDeBuff->bRemoveAfterAttack = UseMoment == EPassiveAbilityUseMoment::BeforeAttack;
		NewDeBuff->bIsFromAura = false;
		NewDeBuff->CasterCharacter = AbilityOwner;
		UGameplayStatics::FinishSpawningActor(NewDeBuff, SpawnTransform);


		ADeBuffBase* ExistingDeBuff;
		bool bAlreadyGotDeBuff = InAffectedCharacter->AlreadyGotDeBuff(NewDeBuff, ExistingDeBuff);

		if (!bAlreadyGotDeBuff)
		{
			InAffectedCharacter->AddDeBuff(NewDeBuff);
			NewDeBuff->InitializeEvent();
		}
		else
		{
			if (NewDeBuff->TurnDuration <= ExistingDeBuff->TurnDuration)
			{
				NewDeBuff->Destroy();
			}
			else
			{
				ExistingDeBuff->RemoveStatusEffect();
				InAffectedCharacter->AddDeBuff(NewDeBuff);
				NewDeBuff->InitializeEvent();
			}
		}
	}
}

void APassiveAbilityBase::OnAbilityUsedHandler()
{
	OnAnimationPlayed();
}
