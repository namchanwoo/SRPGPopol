#include "AuraBase.h"

#include "Kismet/GameplayStatics.h"
#include "SRG/Core/BattleController.h"
#include "SRG/Gird/Grid.h"
#include "SRG/Gird/Slots/SlotBase.h"
#include "SRG/StatusEffects/Buffs/BuffBase.h"
#include "SRG/StatusEffects/DeBuffs/DeBuffBase.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "SRGCore/Utilities/SRGLog.h"


AAuraBase::AAuraBase()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다. 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAuraBase::ApplyAura()
{
	// BattleController 검증
	SRPG_CHECK(BattleController);

	// 캐릭터 범위 내의 개체들을 수집
	TArray<ASlotBase*> SlotsInRange, EmptySlotsInRange;
	TArray<ACharacterBase*> AliesInRange, EnemiesInRange;
	BattleController->GetGrid()->GetSlotsInCharacterRange(AbilityOwner, Range, AbilityOwner->Slot, true,
	                                                 SlotsInRange, EmptySlotsInRange,
	                                                 AliesInRange, EnemiesInRange);

	// 범위 내의 각 슬롯에 Aura 적용
	for (ASlotBase* InSlot : SlotsInRange)
	{
		InSlot->AddAura(this, AbilityOwner);
	}

	// 범위 내의 각 동맹 캐릭터에 Buffs 적용
	for (ACharacterBase* InCharacter : AliesInRange)
	{
		AddBuffsToCharacter(InCharacter);
	}

	// 범위 내의 각 적대 캐릭터에 DeBuffs 적용
	for (ACharacterBase* InCharacter : EnemiesInRange)
	{
		AddDeBuffsToCharacter(InCharacter);
	}
}

ABuffBase* AAuraBase::SpawnBuff(TSubclassOf<ABuffBase> InBuff)
{
	if (UWorld* World = GetWorld())
	{
		ABuffBase* NewBuff = World->SpawnActorDeferred<ABuffBase>(InBuff, FTransform());
		if (NewBuff)
		{
			NewBuff->TurnDuration = 0;
			NewBuff->bRemoveAfterAttack = false;
			NewBuff->bIsFromAura = true;
			NewBuff->CasterCharacter = AbilityOwner;
			UGameplayStatics::FinishSpawningActor(NewBuff, FTransform());
			return NewBuff;
		}
	}
	return nullptr;
}

void AAuraBase::AddBuff(ACharacterBase* AffectedCharacter, TSubclassOf<ABuffBase> InBuff)
{
	ABuffBase* NewBuff = SpawnBuff(InBuff);
	if (NewBuff)
	{
		ABuffBase* ExistingBuff;
		if (!AffectedCharacter->AlreadyGotEffect<ABuffBase>(NewBuff, ExistingBuff, AffectedCharacter->CurrentBuffs))
		{
			AffectedCharacter->AddEffect(NewBuff,AffectedCharacter->CurrentBuffs);
			NewBuff->InitializeEvent();
		}
	}
}

void AAuraBase::AddBuffsToCharacter(ACharacterBase* AffectedCharacter)
{
	if (AffectedCharacter->IsDead()) return; // 캐릭터가 죽었는지 먼저 확인

	for (const TSubclassOf<ABuffBase> InBuff : Buffs)
	{
		AddBuff(AffectedCharacter, InBuff);
	}
}

ADeBuffBase* AAuraBase::SpawnDeBuff(TSubclassOf<ADeBuffBase> InDeBuff)
{
	if (UWorld* World = GetWorld())
	{
		if (ADeBuffBase* NewDeBuff = World->SpawnActorDeferred<ADeBuffBase>(InDeBuff, FTransform()))
		{
			NewDeBuff->TurnDuration = 0;
			NewDeBuff->bRemoveAfterAttack = false;
			NewDeBuff->bIsFromAura = true;
			NewDeBuff->CasterCharacter = AbilityOwner;
			UGameplayStatics::FinishSpawningActor(NewDeBuff, FTransform());
			return NewDeBuff;
		}
	}
	return nullptr;
}

void AAuraBase::AddDeBuff(ACharacterBase* AffectedCharacter, TSubclassOf<ADeBuffBase> InDeBuff)
{
	if (ADeBuffBase* NewDeBuff = SpawnDeBuff(InDeBuff))
	{
		ADeBuffBase* ExistingDeBuff;
		if (!AffectedCharacter->AlreadyGotEffect<ADeBuffBase>(NewDeBuff, ExistingDeBuff,
		                                                      AffectedCharacter->CurrentDeBuffs))
		{
			AffectedCharacter->AddEffect(NewDeBuff,AffectedCharacter->CurrentDeBuffs);
			NewDeBuff->InitializeEvent();
		}
	}
}

void AAuraBase::AddDeBuffsToCharacter(ACharacterBase* AffectedCharacter)
{
	if (AffectedCharacter->IsDead()) return; // 캐릭터가 죽었는지 먼저 확인

	for (const TSubclassOf<ADeBuffBase> InDeBuff : DeBuffs)
	{
		AddDeBuff(AffectedCharacter, InDeBuff);
	}
}
