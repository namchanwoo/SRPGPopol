#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "SRG/Abilities/AbilityBase.h"
#include "SRG/StatusEffects/Buffs/BuffBase.h"
#include "SRG/StatusEffects/DeBuffs/DeBuffBase.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "ActiveAbilityBase.generated.h"

class ADeBuffBase;
class ABuffBase;
class ASlotBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveAbilityUseEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveAbilityUseStarted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveAbilityActivated, bool, IsActive);


/*
* AActiveAbilityBase는 AAbilityBase에서 상속합니다.
* 게임 엔터티에서 사용할 수 있는 게임의 "활성 능력"을 나타내며 비용, 쿨다운 및 애니메이션이 있습니다.
* 여기에는 활성화 방법, 상태 효과 적용, 쿨다운 조정, 애니메이션, AI 의사 결정 및 BattleController와의 상호 작용이 포함됩니다.
* 캐릭터에게 버프와 디버프를 적용할 수 있습니다.
*/
UCLASS()
class SRG_API AActiveAbilityBase : public AAbilityBase
{
	GENERATED_BODY()

public:
	AActiveAbilityBase();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Area Target Active Ability Event")
	TMap<ACharacterBase*, FDamageData> GetAbilityDamage(const TArray<ACharacterBase*>& InTargetCharacters);
	virtual TMap<ACharacterBase*, FDamageData> GetAbilityDamage_Implementation(
		const TArray<ACharacterBase*>& InTargetCharacters);


	/*---	      	    Cooldown and Mana Control    	      	---*/
public:
	/** 쿨다운을 설정합니다.	 */
	void SetCoolDown(int32 NewCurrentCoolDown);

	/**  마나를 감소시킵니다.	 */
	void ReduceMana();

	/*---	      	    Animation    	      	---*/
public:
	/* 능력 애니메이션을 재생합니다. */
	UFUNCTION(BlueprintCallable, Category="Active Ability")
	virtual void PlayAbilityAnimation();

protected:
	/**	 애니메이션이 재생된 후 호출됩니다.	 */
	void OnAnimationPlayed();

	/**	능력 애니메이션이 끝났을 때 호출됩니다.	 */
	UFUNCTION()
	void OnAbilityAnimationEnded();

private:
	/**	 애니메이션이 끝난 후에 수행할 로직을 실행합니다.	 */
	void PostAnimationLogic();

	/*---	      	    Buff and DeBuff Management    	      	---*/
public:
	/**
	 * @brief 효과를 추가합니다.
	 * @tparam EffectType 효과의 타입
	 * @param InAffectedCharacter 효과를 받는 캐릭터
	 * @param InEffect 추가할 효과
	 * @param InTurnDuration 효과의 지속 턴 수
	 * @param CurrentEffects 현재 적용중인 효과 목록
	 */
	template <typename EffectType>
	void AddEffect(ACharacterBase* InAffectedCharacter, TSubclassOf<EffectType> InEffect,
	               int32 InTurnDuration, TArray<EffectType*>& CurrentEffects)
	{
		if (UWorld* World = GetWorld())
		{
			if (EffectType* NewEffect = World->SpawnActorDeferred<EffectType>(InEffect, FTransform()))
			{
				NewEffect->AffectedCharacter = InAffectedCharacter;
				NewEffect->TurnDuration = InTurnDuration;
				NewEffect->bRemoveAfterAttack = bRemoveStatusEffectAfterAttack;
				NewEffect->bIsFromAura = false;
				NewEffect->CasterCharacter = AbilityOwner;
				UGameplayStatics::FinishSpawningActor(NewEffect, FTransform());

				EffectType* ExistingEffect;
				if (!InAffectedCharacter->AlreadyGotEffect<EffectType>(NewEffect, ExistingEffect, CurrentEffects))
				{
					InAffectedCharacter->AddEffect<EffectType>(NewEffect, CurrentEffects);
					NewEffect->InitializeEvent();
				}
				else
				{
					if (NewEffect->TurnDuration <= ExistingEffect->TurnDuration)
					{
						NewEffect->Destroy();
					}
					else
					{
						ExistingEffect->RemovesStatusEffects;
						InAffectedCharacter->AddEffect<EffectType>(NewEffect, CurrentEffects);
						NewEffect->InitializeEvent();
					}
				}
			}
		}
	}

	/**
	 * @brief 버프를 추가합니다.
	 * @param InAffectedCharacter 버프를 받을 캐릭터
	 * @param InBuff 추가할 버프
	 * @param InTurnDuration 버프의 지속 턴 수
	 */
	UFUNCTION(BlueprintCallable, Category="Active Ability Event")
	void AddBuff(ACharacterBase* InAffectedCharacter, TSubclassOf<ABuffBase> InBuff, int32 InTurnDuration);

	/**
	 * @brief 디버프를 추가합니다.
	 * @param InAffectedCharacter 디버프를 받을 캐릭터
	 * @param InDeBuff 추가할 디버프
	 * @param InTurnDuration 디버프의 지속 턴 수
	 */
	UFUNCTION(BlueprintCallable, Category="Active Ability Event")
	void AddDeBuff(ACharacterBase* InAffectedCharacter, TSubclassOf<ADeBuffBase> InDeBuff, int32 InTurnDuration);


	/*---	      	    Ability and Target Management    	      	---*/
public:
	/**
 	* @brief 해당 능력이 영향을 미치는 슬롯과 캐릭터들을 반환합니다.
 	* @param OutSlotsInRange 영향을 받는 슬롯들 
 	* @param OutEmptySlotsInRange 영향을 받는 빈 슬롯들
 	* @param OutAlliesInRange 영향을 받는 아군 캐릭터들
 	* @param OutEnemiesInRange 영향을 받는 적군 캐릭터들
 	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Active Ability Event")
	void GetAbilitySlots(TArray<ASlotBase*>& OutSlotsInRange, TArray<ASlotBase*>& OutEmptySlotsInRange,
	                     TArray<ACharacterBase*>& OutAlliesInRange, TArray<ACharacterBase*>& OutEnemiesInRange);

	virtual void GetAbilitySlots_Implementation(TArray<ASlotBase*>& OutSlotsInRange,
	                                            TArray<ASlotBase*>& OutEmptySlotsInRange,
	                                            TArray<ACharacterBase*>& OutAlliesInRange,
	                                            TArray<ACharacterBase*>& OutEnemiesInRange);


	/**
	 * @brief 능력의 세부 정보 (마나 비용 및 쿨다운)을 반환합니다.
	 * @param OutManaCost 마나 비용 
	 * @param OutCoolDown 쿨다운 시간
	 * @return 캐릭터가 마나 비용을 지불할 수 있는지 여부
	 */
	bool GetAbilityDetails(int32& OutManaCost, int32& OutCoolDown);

	/**
	 * @brief 적을 죽인 경우에 발동되는 패시브 능력을 사용합니다.
	 */
	void UseOnKillPassiveAbilities();

	/**
	 * @brief 주어진 능력을 활성화 또는 비활성화합니다.
	 * @param IsActive 활성화할지 여부
	 */
	virtual void ActivateActiveAbility(bool IsActive);

	/**	  능력이 활성화될 때 실행됩니다.	 */
	virtual void OnActiveAbilityEnabled();

	/**	  능력이 비활성화될 때 실행됩니다.	 */
	virtual void OnActiveAbilityDisabled();

	/* AI에 대한 대상 슬롯을 설정합니다. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Active Ability Event")
	void SetTargetSlotForAI();
	virtual void SetTargetSlotForAI_Implementation();

	/**
	 * @brief 주어진 슬롯을 대상으로 설정합니다.
	 * @param InSlot 대상으로 설정할 슬롯
	 */
	UFUNCTION(BlueprintCallable, Category="Active Ability Event")
	virtual void SetTargetSlot(ASlotBase* InSlot);

	/**	 대상 슬롯을 초기화합니다.	 */
	virtual void ResetTargetSlot();

	/**
	 * @brief 상태 효과를 적용합니다.
	 * @param AllyCharacters 상태 효과를 받는 아군 캐릭터들
	 * @param EnemyCharacters 상태 효과를 받는 적군 캐릭터들
	 * @param IsBeforeAttack 공격 전인지 여부
	 */
	UFUNCTION(BlueprintCallable, Category="Active Ability Event")
	virtual void ApplyStatusEffects(const TArray<ACharacterBase*>& AllyCharacters,
	                                const TArray<ACharacterBase*>& EnemyCharacters, bool IsBeforeAttack);

	/**
	 * @brief 활성화된 능력 사용에 대한 콜백을 제거합니다.
	 */
	void ClearActiveAbilityUseCallbacks();

	/* 주어진 데미지 데이터를 사용하여 대상에게 피해를 입힙니다. */
	UFUNCTION(BlueprintCallable, Category="Active Ability Event")
	void DamageTargets(const TMap<ACharacterBase*, FDamageData>& InDamageData);

	/**
	 * @brief 주어진 공격 유형에 대해 능력이 사용 가능한지 확인합니다.
	 * @param IsMeleeAttack 근접 공격인지 여부
	 * @return 능력이 사용 가능한지 여부
	 */
	virtual bool IsAvailable(bool IsMeleeAttack);

	/**
	 * @brief AI가 능력을 사용해야 하는지 판단합니다.
	 * @param SlotsInRange 영향을 받는 슬롯들
	 * @param EmptySlotsInRange 영향을 받는 빈 슬롯들
	 * @param AlliesInRange 영향을 받는 아군 캐릭터들
	 * @param EnemiesInRange 영향을 받는 적군 캐릭터들
	 * @param WeakestEnemy 가장 약한 적 캐릭터
	 * @return AI가 능력을 사용해야 하는지 여부
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Active Ability Event")
	bool ShouldAIUse(const TArray<ASlotBase*>& SlotsInRange, const TArray<ASlotBase*>& EmptySlotsInRange,
	                 const TArray<ACharacterBase*>& AlliesInRange, const TArray<ACharacterBase*>& EnemiesInRange,
	                 ACharacterBase* WeakestEnemy);

	virtual bool ShouldAIUse_Implementation(const TArray<ASlotBase*>& SlotsInRange,
	                                        const TArray<ASlotBase*>& EmptySlotsInRange,
	                                        const TArray<ACharacterBase*>& AlliesInRange,
	                                        const TArray<ACharacterBase*>& EnemiesInRange,
	                                        ACharacterBase* WeakestEnemy);

	/*---	      	    Turn Management    	      	---*/
public:
	/* 턴을 종료합니다. */
	UFUNCTION(BlueprintCallable, Category="Active Ability Event")
	void EndTurn();

protected:
	/**	 턴 종료 이벤트 핸들러입니다.	 */
	UFUNCTION()
	void OnEndTurnHandle();

	/**	  새로운 턴 이벤트 핸들러입니다.	 */
	UFUNCTION()
	void OnNewTurnHandler(int32 NewTurn);

	/*---	      	    Ability Use Callback    	      	---*/
public:
	virtual void OnAbilityUsed_Implementation(const TArray<ASlotBase*>& InSlotsInRange,
	                                          const TArray<ASlotBase*>& InEmptySlotsInRange,
	                                          const TArray<ACharacterBase*>& InAlliesInRange,
	                                          const TArray<ACharacterBase*>& InEnemiesInRange) override;

protected:
	/**	  능력 사용 시 핸들러 함수입니다.	 */
	UFUNCTION()
	void OnAbilityUsedHandler();

	/**	 패시브 능력 사용 완료 시 핸들러 함수입니다.	 */
	UFUNCTION()
	void OnKillPassiveAbilitiesUsed();


	/*******************************************
 	* Configuration and Control Variables
 	*******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Configuration")
	int32 Cooldown; // 쿨다운 시간을 나타내는 변수입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Configuration")
	int32 ManaCost; // 이 능력을 사용하는데 필요한 마나의 양입니다.

	UPROPERTY(BlueprintReadWrite, Category="Active Ability|Configuration")
	int32 CurrentCooldown; // 현재 쿨다운 상태를 나타내는 변수입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Configuration")
	bool bStartWithCooldown; // 능력이 쿨다운 상태로 시작하는지를 결정하는 변수입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Control")
	bool bAbilityUsed; // 능력이 이미 사용되었는지를 나타내는 변수입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Control")
	bool bAnimationHasEnded; // 애니메이션이 종료되었는지를 나타내는 변수입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Control")
	bool bRemoveStatusEffectAfterAttack; // 공격 후 상태 효과를 제거해야 하는지를 나타내는 변수입니다.

	/*--- Appearance Variables ---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Appearance")
	UTexture2D* ActiveAbilityIcon; // 활성 능력 아이콘을 나타내는 변수입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Appearance")
	UTexture2D* CooldownIcon; // 쿨다운 아이콘을 나타내는 변수입니다.


	/*--- Gameplay Variables ---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Gameplay")
	EElement Element; // 능력의 원소 유형을 나타내는 변수입니다.

	UPROPERTY(BlueprintReadWrite, Category="Active Ability|Gameplay")
	TArray<ACharacterBase*> CurrentTargets; // 현재 목표 캐릭터들을 저장하는 배열입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Gameplay")
	TMap<TSubclassOf<ABuffBase>, int32> Buffs; // 능력에 의해 부여되는 버프와 그 수치를 저장하는 맵입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Gameplay")
	TMap<TSubclassOf<ADeBuffBase>, int32> DeBuffs; // 능력에 의해 부여되는 디버프와 그 수치를 저장하는 맵입니다.

	/*--- Animation Variables ---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Active Ability|Animation")
	TMap<TSubclassOf<ACharacterBase>, UAnimMontage*> UseAbilityMontage; // 캐릭터별 능력 사용 애니메이션을 저장하는 맵입니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Active Ability|Animation")
	float AbilityAnimationDelay; // 능력 애니메이션 지연을 설정하는 변수입니다.

	/*--- Delegate ---*/
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Active Ability|Delegate")
	FOnActiveAbilityUseEnded OnActiveAbilityUseEnded; // 능력 사용이 종료될 때 호출되는 대리자입니다.

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Active Ability|Delegate")
	FOnActiveAbilityUseStarted OnActiveAbilityUseStarted; // 능력 사용이 시작될 때 호출되는 대리자입니다.

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Active Ability|Delegate")
	FOnActiveAbilityActivated OnActiveAbilityActivated; // 능력이 활성화될 때 호출되는 대리자입니다.

	/*--- Timer ---*/
protected:
	FTimerHandle EndTurnTimerHandle; // 턴 종료를 관리하는 타이머 핸들입니다.
};
