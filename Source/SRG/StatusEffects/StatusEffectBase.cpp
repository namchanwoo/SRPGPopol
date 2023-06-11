#include "StatusEffectBase.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "SRGCore/Utilities/SRGLog.h"
#include "SRG/StatusEffects/Buffs/BuffBase.h"
#include "SRG/StatusEffects/DeBuffs/DeBuffBase.h"
#include "SRG/Core/BattleController.h"
#include "SRG/Units/Characters/CharacterBase.h"


AStatusEffectBase::AStatusEffectBase()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다. 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
}

void AStatusEffectBase::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsFromAura)
	{
		PlaySound();
		PlayParticleEffect();
	}
}

void AStatusEffectBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AffectedCharacter->OnDead.RemoveDynamic(this, &AStatusEffectBase::OnCharacterDies);
	AffectedCharacter->BattleController->OnNewTurn.RemoveDynamic(this, &AStatusEffectBase::OnNewTurnHandler);
	CasterCharacter->OnRefreshAuras.RemoveDynamic(this, &AStatusEffectBase::OnRefreshAurasHandler);
	AffectedCharacter->OnCharacterRemovedFromSlot.RemoveDynamic(
		this, &AStatusEffectBase::OnCasterCharacterRemovedFromSlot);
}

void AStatusEffectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStatusEffectBase::InitializeEvent()
{
	SRPG_CHECK(AffectedCharacter);

	AffectedCharacter->OnDead.AddDynamic(this, &AStatusEffectBase::OnCharacterDies);
	AffectedCharacter->BattleController->OnNewTurn.AddDynamic(this, &AStatusEffectBase::OnNewTurnHandler);

	SetAuraLogic();

	RemoveOtherStatusEffects();

	ApplyStats(StatAdditions);

	const FCharacterStats MultipliedStats = GetMultipliedStats();
	ApplyStats(MultipliedStats);

	ApplyElementReductions(ElementReductions);

	OnStatusEffectStarted();
}

void AStatusEffectBase::OnStatusEffectStarted()
{
}

void AStatusEffectBase::OnStatusEffectEnded()
{
	Destroy();
}

void AStatusEffectBase::RemoveFromCharacter()
{
}

void AStatusEffectBase::OnDOT()
{
	if (OnDOTApplied.IsBound())
	{
		OnDOTApplied.Broadcast(0.0f);
	}
}

void AStatusEffectBase::SetAuraLogic()
{
	if (bIsFromAura && (AffectedCharacter != CasterCharacter))
	{
		SRPG_CHECK(CasterCharacter);
		CasterCharacter->OnRefreshAuras.AddDynamic(this, &AStatusEffectBase::OnRefreshAurasHandler);
		SRPG_CHECK(AffectedCharacter);
		AffectedCharacter->OnCharacterRemovedFromSlot.AddDynamic(
			this, &AStatusEffectBase::OnCasterCharacterRemovedFromSlot);
	}
}

void AStatusEffectBase::RemoveOtherStatusEffects()
{
	// 모든 상태 효과 검색
	TArray<AStatusEffectBase*> AllStatusEffects = GetAllStatusEffects();

	// 상태 효과를 역순으로 반복
	for (int i = AllStatusEffects.Num() - 1; i >= 0; --i)
	{
		// 현재 상태 효과를 제거해야 하는지 확인
		if (!bIsFromAura && !AllStatusEffects[i]->bIsFromAura &&
			RemovesStatusEffects.Contains(AllStatusEffects[i]->GetClass()))
		{
			// 상태 효과 제거
			AllStatusEffects[i]->RemoveStatusEffect();
		}
	}
}

void AStatusEffectBase::RemoveStats()
{
	const FCharacterStats NegativeStatAdditions = StatAdditions.Negative();
	ApplyStats(NegativeStatAdditions);

	FCharacterStats MultipliedStats = GetMultipliedStats();

	const FCharacterStats NegativeMultipliedStats = MultipliedStats.Negative();
	ApplyStats(NegativeMultipliedStats);
}

void AStatusEffectBase::RemoveElementReductions()
{
	FElementReductions NegativeElementReductions = ElementReductions.Negative();
	ApplyElementReductions(NegativeElementReductions);
}

void AStatusEffectBase::RemoveStatusEffect()
{
	RemoveStats();
	RemoveElementReductions();
	RemoveFromCharacter();
	OnStatusEffectEnded();
}

void AStatusEffectBase::ApplyStats(const FCharacterStats& InStat)
{
	AffectedCharacter->ApplyStats(InStat);
}

void AStatusEffectBase::ApplyElementReductions(const FElementReductions& InElementReductions)
{
	AffectedCharacter->ApplyElementReductions(InElementReductions);
}

void AStatusEffectBase::PlaySound()
{
	if (IsValid(Sound))
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}
}

void AStatusEffectBase::PlayParticleEffect()
{
	if (IsValid(ParticleEffect))
	{
		const FVector SpawnLocation = AffectedCharacter->ProjectileHitLocation->GetComponentLocation();
		UGameplayStatics::SpawnEmitterAtLocation(this, ParticleEffect, SpawnLocation);
	}
	else
	{
		if (IsValid(NiagaraParticleEffect))
		{
			const FVector SpawnLocation = AffectedCharacter->ProjectileHitLocation->GetComponentLocation();
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraParticleEffect, SpawnLocation);
		}
	}
}

FCharacterStats AStatusEffectBase::GetMultipliedStats()
{
	FCharacterStats MultipliedStats;

	// 일부 통계를 명시적으로 초기화
	MultipliedStats.MaxHealth = 0;
	MultipliedStats.MaxMana = 10;

	// 영향을 받는 캐릭터의 주요 통계에 대한 참조 가져오기
	const FCharacterStats& MainStats = AffectedCharacter->MainStats;

	// 관련 통계에 통계 승수를 곱하십시오.
	MultipliedStats.Attack = FMath::Floor(MainStats.Attack * StatMultiplications.Attack);
	MultipliedStats.Defense = FMath::Floor(MainStats.Defense * StatMultiplications.Defense);
	MultipliedStats.MinDamage = FMath::Floor(MainStats.MinDamage * StatMultiplications.MinDamage);
	MultipliedStats.MaxDamage = FMath::Floor(MainStats.MaxDamage * StatMultiplications.MaxDamage);
	MultipliedStats.Ammo = FMath::Floor(MainStats.Ammo * StatMultiplications.Ammo);
	MultipliedStats.MovementRange = FMath::Floor(MainStats.MovementRange * StatMultiplications.MovementRange);

	return MultipliedStats;
}

TArray<AStatusEffectBase*> AStatusEffectBase::GetAllStatusEffects()
{
	// 모든 상태 효과를 저장하기 위해 빈 배열을 초기화합니다.
	TArray<AStatusEffectBase*> CurrentAllStatusEffects;

	// 영향을 받는 캐릭터의 모든 현재 버프 및 디버프를 어레이에 추가합니다.
	const auto& Buffs = AffectedCharacter->CurrentBuffs;
	const auto& DeBuffs = AffectedCharacter->CurrentDeBuffs;

	CurrentAllStatusEffects.Reserve(Buffs.Num() + DeBuffs.Num()); // 선택 사항: 효율성을 위해 메모리 예약
	CurrentAllStatusEffects.Append(Buffs);
	CurrentAllStatusEffects.Append(DeBuffs);

	return CurrentAllStatusEffects;
}


void AStatusEffectBase::OnCharacterDies(ACharacterBase* InCharacter)
{
	Destroy();
}

void AStatusEffectBase::OnNewTurnHandler(int32 NewTurn)
{
	// 상태 효과가 오라에서 오는 것이 아니라면...
	if (!bIsFromAura)
	{
		// 턴 지속 시간 감소
		TurnDuration--;

		// 바인딩된 리스너가 있는 경우 이벤트 브로드캐스트
		if (OnTurnDurationUpdated.IsBound())
		{
			OnTurnDurationUpdated.Broadcast();
		}

		// 상태 효과의 지속 시간이 끝나면 제거하십시오.
		if (TurnDuration <= 0)
		{
			RemoveStatusEffect();
		}
	}
}

void AStatusEffectBase::OnRefreshAurasHandler()
{
	RemoveStatusEffect();

	SRPG_CHECK(AffectedCharacter);
	AffectedCharacter->ReceiveAuras();
}

void AStatusEffectBase::OnCasterCharacterRemovedFromSlot(ACharacterBase* InCharacter)
{
	RemoveStatusEffect();
}
