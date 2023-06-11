#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "SRG/Units/UnitBase.h"
#include "CharacterBase.generated.h"


class AUnitBase;
class UNiagaraSystem;
class ADeBuffBase;
class ABuffBase;
class AAuraBase;
class APassiveAbilityBase;
class AActiveAbilityBase;
class AMultiTargetPassiveAbilityBase;
class ABattleController;
class UWidgetComponent;
class USpringArmComponent;
class UUW_CharacterHealth;
class ACharacterBase;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthSet, int32, NewHealth, bool, IsHeal);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaSet_Character, int32, InMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterMoveToSlot, ACharacterBase*, InCharacter);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterRemovedFromSlot, ACharacterBase*, InCharacter);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatsUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRemovedInstantly);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterMoved);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopAnimationMontage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationEnded_Character);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttacked);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityUsed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTaken);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterRotated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResurrected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusEffectsUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRefreshAuras);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDragStarted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDead, ACharacterBase*, InCharacter);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterHighlighted, bool, IsOn);

/*
 * ACharacterBase는 게임의 모든 캐릭터 유닛에 대한 기본 클래스입니다.
 * 이것은 캐릭터 행동, 능력 및 특성을 정의하는 기초 역할을 합니다.
 * 이 클래스는 AUnitBase에서 제공하는 기능을 확장하고 이동, 전투, 능력, 통계 및 애니메이션과 같은 캐릭터와 관련된 특정 기능 및 동작을 추가합니다.
 * ACharacterBase는 고유한 특성을 가진 다양한 문자 유형을 생성하기 위한 청사진을 제공하고 하위 클래스를 통해 사용자 정의 및 전문화를 허용합니다.
 * 또한 그리드 기반 이동, 능력 사용, 손상 받기, 버프 및 디버프 적용, 체력 및 마나 관리를 포함하여 게임 세계와의 상호 작용을 처리합니다.
 * ACharacterBase에서 상속함으로써 개발자는 게임의 게임 플레이 경험에 기여하는 다양하고 매력적인 캐릭터를 만들 수 있습니다.
 */
UCLASS()
class SRG_API ACharacterBase : public AUnitBase
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

	// MoveOverTime 타임라인 초기화
	void InitializeMoveOverTimeTimeline();

	// RotateOverTime 타임라인 초기화
	void InitializeRotateOverTimeTimeline();

public:
	virtual void Tick(float DeltaTime) override;

	/*******************************************
	 * Initialization and Events
	 *******************************************/
public:
	/**
 	* @brief 캐릭터 이벤트 및 구성 요소를 초기화합니다.
 	*/
	virtual void InitializeEvent() override;

	/**
 	* @brief 캐릭터의 하이라이트 상태를 설정합니다.
 	* @param IsOn 하이라이트 상태를 켜거나 끕니다.
 	*/
	virtual void Highlight(bool IsOn) override;

	/**
 	* @brief 캐릭터에 오라를 적용하고 캐릭터가 슬롯으로 이동할 때 OnCharacterMoveToSlot 이벤트를 브로드캐스트합니다.
 	*/
	virtual void OnMovedToSlot() override;

	/**
 	* @brief 캐릭터가 슬롯에서 제거될 때 OnCharacterRemovedFromSlot 이벤트와 OnRefreshAuras 이벤트를 브로드캐스트합니다.
 	*/
	virtual void OnRemovedFromSlot() override;


	/*******************************************
	* Character Setup and Stats
	*******************************************/
public:
	/**
 	* @brief 캐릭터의 시작 스택 값을 현재 스택 값으로 설정합니다.
 	*/
	void SetStartingStack();

	/**
 	* @brief 캐릭터의 보복 횟수를 새로운 값으로 재설정합니다.
 	* @param NewRetaliationCount 새로운 보복 횟수입니다.
 	*/
	void ResetRetaliation(int32 NewRetaliationCount);

	/**
 	* @brief 캐릭터의 스탯을 설정하고 업데이트합니다.
 	*/
	void SetStats();

	/**
 	* @brief 캐릭터의 능력을 설정합니다.
 	*/
	void SetAbilities();

	/**
 	* @brief 캐릭터의 스탯을 적용합니다.
 	* @param InApplyStat 적용 할 캐릭터 스텟
 	*/
	void ApplyStats(const FCharacterStats& InApplyStat);

	/**
 	* @brief 캐릭터의 원소 감소율을 적용합니다.
 	* @param InElementReductions 적용 할 원소 감소율
 	*/
	void ApplyElementReductions(const FElementReductions& InElementReductions);

	// Aura Handling
	//--------------------------------------
public:
	/**
 	* @brief 캐릭터의 오라를 적용합니다.
 	*/
	void ApplyAuras();

	/**
 	* @brief 캐릭터가 범위 내의 슬롯에 오라를 적용합니다.
 	*/
	void ReceiveAuras();


	/*******************************************
	 * Health, Mana, and Healing
	 *******************************************/
public:
	/**
 	* @brief 캐릭터의 체력을 설정하고 사망 여부를 확인합니다.
 	* @param NewHealth 새로운 체력 값입니다.
 	* @param IsHeal 체력이 회복되었는지 여부입니다.
 	*/
	void SetHealth(int32 NewHealth, bool IsHeal);

	/**
 	* @brief 캐릭터의 마나를 설정합니다.
 	* @param NewMana 새로운 마나 값입니다.
 	*/
	void SetMana(int32 NewMana);

	/**
 	* @brief 캐릭터의 체력을 회복합니다.
 	* @param InAmount 회복량입니다.
 	*/
	void Heal(int32 InAmount);

	/**
 	* @brief 캐릭터의 마나를 증가시킵니다.
 	* @param InAmount 증가량입니다.
 	*/
	void IncreaseMana(int32 InAmount);

	/**
 	* @brief 캐릭터의 마나를 감소시킵니다.
 	* @param InAmount 감소량입니다.
 	*/
	void ReduceMana(int32 InAmount);

	/**
 	* @brief 캐릭터를 부활시킵니다.
 	* @param InStackAmount 부활 후 스택 값입니다.
 	*/
	void Resurrect(int32 InStackAmount);


	/*******************************************
	 * Damage and Dying
	 *******************************************/
public:
	/**
 	* @brief 캐릭터가 데미지를 받았을 때의 이벤트를 처리합니다.
 	* @param InDamage 데미지 값입니다.
 	* @param InElement 데미지의 원소입니다.
 	*/
	void TakeDamageEvent(int32 InDamage, EElement InElement);

	/**
 	* @brief 캐릭터가 데미지를 받았을 때의 시각적 효과를 표시합니다.
 	* @param InStack 스택 값입니다.
 	* @param InDamage 데미지 값입니다.
 	* @param InHeal 치유 값입니다.
 	* @param InElement 원소 값입니다.
 	* @param IsResurrect 부활 여부입니다.
 	*/
	void ShowDamage(int32 InStack, int32 InDamage, int32 InHeal, EElement InElement, bool IsResurrect);

	/**
 	* @brief 캐릭터가 데미지를 받았을 때 사망한 스택과 남은 체력을 계산합니다.
 	* @param InDamage 데미지 값입니다.
 	* @param DiedStack 사망한 스택 수입니다.
 	* @param RemainingHealth 남은 체력 값입니다.
 	*/
	void CalculateDyingStack(int32 InDamage, int32& DiedStack, int32& RemainingHealth);

	/**
 	* @brief 캐릭터가 사망한 상태인지 확인합니다.
 	* @return 캐릭터가 사망한 상태인지 여부입니다.
 	*/
	bool IsDead();

	/**
 	* @brief 캐릭터의 사망 처리를 수행합니다.
 	*/
	void Die();

	/**
 	* @brief 캐릭터가 사망한 후의 이벤트를 처리합니다.
 	*/
	UFUNCTION()
	void OnDied();


	/*******************************************
	 * Movement and Rotation
	 *******************************************/
public:
	/**
	 * @brief 주어진 슬롯을 향해 캐릭터를 회전시킵니다.
	 * @param InTargetSlot 캐릭터가 회전할 목표 슬롯
	 */
	void Rotate(ASlotBase* InTargetSlot);

	/**
	 * @brief 캐릭터의 움직임을 처리합니다.
	 */
	void Move();

	/**
	 * @brief 실제 캐릭터 움직임을 처리하는 메서드입니다.
	 */
	void HandleMovement();

	/**
	 * @brief 이동 타임라인 업데이트를 처리합니다.
	 * @param Value 타임라인에서의 현재 값
	 */
	UFUNCTION()
	void HandleMoveOverTime_Update(float Value);

	/**
	 * @brief 이동 타임라인이 끝날 때 호출됩니다.
	 */
	UFUNCTION()
	void HandleMoveOverTime_Finished();

	/**
	 * @brief 회전 타임라인 업데이트를 처리합니다.
	 * @param Value 타임라인에서의 현재 값
	 */
	UFUNCTION()
	void HandleRotateOverTime_Update(float Value);

	/**
	 * @brief 회전 타임라인이 끝날 때 호출됩니다.
	 */
	UFUNCTION()
	void HandleRotateOverTime_Finished();

	/**
	 * @brief 애니메이션 몽타주의 블렌딩 아웃 이벤트를 처리합니다.
	 * @param Montage 블렌딩 아웃 중인 몽타주
	 * @param bInterrupted 몽타주가 중단되었는지 여부
	 */
	UFUNCTION()
	void OnMontageBlendingOut_Move(UAnimMontage* Montage, bool bInterrupted);

	/**
	 * @brief 애니메이션 몽타주가 끝날 때의 이벤트를 처리합니다.
	 * @param Montage 끝난 몽타주
	 * @param bInterrupted 몽타주가 중단되었는지 여부
	 */
	UFUNCTION()
	void OnMontageEnded_Move(UAnimMontage* Montage, bool bInterrupted);

	/**
	 * @brief 애니메이션 몽타주를 재생합니다.
	 * @param MontageToPlay 재생할 몽타주
	 */
	void PlayAnimationMontage(UAnimMontage* MontageToPlay);

	/**
	 * @brief 애니메이션 몽타주 시작 알림을 처리합니다.
	 * @param NotifyName 알림의 이름
	 * @param BranchingPointPayload 알림의 페이로드
	 */
	UFUNCTION()
	void OnPlayMontageNotifyBegin_PlayAnim(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	/**
	 * @brief 애니메이션 몽타주가 끝날 때의 이벤트를 처리합니다.
	 * @param Montage 끝난 몽타주
	 * @param bInterrupted 몽타주가 중단되었는지 여부
	 */
	UFUNCTION()
	void OnMontageEnded_PlayAnim(UAnimMontage* Montage, bool bInterrupted);

	/**
	 * @brief 애니메이션 몽타주를 정지합니다.
	 */
	void StopAnimationMontage();

	/**
	 * @brief 캐릭터를 즉시 제거합니다.
	 */
	void RemoveCharacterInstantly();


	/*******************************************
	 * Turn and Status Effects
	 *******************************************/
public:
	/**
	 * @brief 새로운 턴이 시작될 때 호출되는 함수
	 * @param NewTurn 새로운 턴 숫자
	 */
	UFUNCTION()
	void OnNewTurnHandler(int32 NewTurn);

	template <class EffectType>
	void AddEffect(EffectType* InEffect, TArray<EffectType*>& CurrentEffects)
	{
		CurrentEffects.Add(InEffect);

		if (OnStatusEffectsUpdated.IsBound())
			OnStatusEffectsUpdated.Broadcast();
	}

	template <class EffectType>
	void RemoveEffect(EffectType* InEffect, TArray<EffectType*>& CurrentEffects)
	{
		CurrentEffects.Remove(InEffect);

		if (OnStatusEffectsUpdated.IsBound())
			OnStatusEffectsUpdated.Broadcast();
	}

	UFUNCTION(BlueprintCallable, Category="Character Event")
	void AddBuff(ABuffBase* NewBuff);

	UFUNCTION(BlueprintCallable, Category="Character Event")
	void AddDeBuff(ADeBuffBase* NewDeBuff);

	UFUNCTION(BlueprintCallable, Category="Character Event")
	void RemoveBuff(ABuffBase* RemoveBuff);

	UFUNCTION(BlueprintCallable, Category="Character Event")
	void RemoveDeBuff(ADeBuffBase* RemoveDeBuff);

	/**
	 * @brief 캐릭터가 방어 상태인지 설정하는 함수
	 * @param IsDefending 방어 상태 여부
	 */
	void SetDefending(bool IsDefending);

	/**
	 * @brief 캐릭터가 대기 상태인지 설정하는 함수
	 * @param IsWaiting 대기 상태 여부
	 */
	void SetWaiting(bool IsWaiting);

	/**
	 * @brief 드래그가 시작될 때 호출되는 함수
	 */
	void DragStarted();

	/**
	 * @brief 승리 애니메이션을 재생하는 함수
	 */
	void PlayVictoryAnimation();

	/**
	 * @brief 캐릭터가 근접 공격 유형인지 확인하는 함수
	 * @return 근접 공격 유형인 경우 true 반환
	 */
	bool IsMeleeCharacter();

	/**
	 * @brief 근접 공격 애니메이션을 재생하는 함수
	 */
	void PlayMeleeAttackAnimation();


	/**
	* @brief 이미 효과가 적용되었는지 확인합니다
	* @tparam EffectType 효과의 유형 (예: ABuffBase 또는 ADeBuffBase)
	* @param InEffect 확인하려는 효과
	* @param ExistingEffect 이미 존재하는 동일한 효과 (있는 경우)
	* @param CurrentEffects 현재 캐릭터에 적용된 모든 효과의 목록
	* @return 효과가 이미 적용되었는지 여부
	*/
	template <class EffectType>
	bool AlreadyGotEffect(EffectType* InEffect, EffectType*& ExistingEffect, TArray<EffectType*>& CurrentEffects)
	{
		// 입력한 효과가 유효하지 않으면 false를 반환
		if (InEffect == nullptr)
		{
			ExistingEffect = nullptr;
			return false;
		}

		// 현재 적용된 모든 효과 확인
		for (EffectType* CurrentEffect : CurrentEffects)
		{
			// 효과의 클래스와 소스가 일치하면 true를 반환합니다.
			if ((CurrentEffect->GetClass() == InEffect->GetClass()) &&
				(CurrentEffect->bIsFromAura == InEffect->bIsFromAura))
			{
				ExistingEffect = CurrentEffect;
				return true;
			}
		}

		// 일치하는 효과가 없으면 false를 반환합니다.
		ExistingEffect = nullptr;
		return false;
	}

	UFUNCTION(BlueprintCallable, Category="Character Event")
	void AlreadyGotBuff(ABuffBase* InBuff, ABuffBase*& ExistingBuff);

	UFUNCTION(BlueprintCallable, Category="Character Event")
	void AlreadyGotDeBuff(ADeBuffBase* InDeBuff, ADeBuffBase*& ExistingDeBuff);

	/**
 	* @brief 근접 공격 애니메이션 및 공격 이벤트에 대한 모든 바운드 델리게이트 제거
 	*/
	void ClearMeleeAttackCallback();

	/**
 	* @brief 능력 애니메이션 및 공격 이벤트에 대한 모든 바운드 대리인을 제거합니다.
 	*/
	void ClearAbilityCallback();
	


	/*******************************************
	 * Field Members
	 *******************************************/

	/*---	      	    Attributes    	      	---*/
public:
	// 인공지능 캐릭터 여부
	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	bool bIsAI; 

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	bool bIsWaiting; // 대기 중인 캐릭터 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	bool bIsBoss; // 보스 캐릭터 여부

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	bool bIsMoving; // 이동 중인 캐릭터 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	bool bCanRotate = true; // 회전 가능 여부

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	bool bIsDefending; // 방어 중인 캐릭터 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	bool bBoss; // Deprecated - 사용되지 않는 변수

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	EMovementType MovementType; // 캐릭터의 이동 유형

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	FText Name; // 캐릭터 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	UTexture2D* Image; // 캐릭터 이미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	UTexture2D* DragImage; // 드래그 이미지

	// 캐릭터 배치 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Setting")
	USoundBase* DeploySound;

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 Stack = 1; // 스택 수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	int32 ExpReward = 1; // 경험치 보상

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	int32 Cost = 1; // 비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	int32 MaxRetaliationCount = 1; // 최대 보복 횟수

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 CurrentRetaliationCount; // 현재 보복 횟수

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 StartingStack; // 시작 스택 수

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 CurrentHealth; // 현재 체력

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 CurrentMana; // 현재 마나

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 CurrenAmmo; // 현재 탄환 수

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 CurrentAttack; // 현재 공격력

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 CurrentDefense; // 현재 방어력

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 CurrentMinDamage; // 현재 최소 피해

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 CurrentMaxDamage; // 현재 최대 피해

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 CurrentAmmo; // 현재 탄환 수

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	int32 CurrentMovementRange; // 현재 이동 범위

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	float CurrentNormalReduction; // 현재 일반 피해 감소율

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	float CurrentFireReduction; // 현재 화염 피해 감소율

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	float CurrentEarthReduction; // 현재 대지 피해 감소율

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	float CurrentAirReduction; // 현재 공기 피해 감소율

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	float CurrentWaterReduction; // 현재 물 피해 감소율

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	float WalkingSpeed = 400.0f; // 걷기 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	float RotationSpeed = 200.0f; // 회전 속도

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	FHeroStats HeroStats; // 영웅 스탯

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	TSubclassOf<AMultiTargetPassiveAbilityBase> MultiTargetPassiveAbility; // 다중 대상 패시브 능력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	FCharacterStats CharacterStats; // 캐릭터 스탯

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	FCharacterStats MainStats; // 주 스탯

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	ABattleController* BattleController; // 전투 컨트롤러

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	UParticleSystem* MeleeHitEffect; // 근접 공격 히트 이펙트

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	UNiagaraSystem* NiagaraMeleeHitEffect; // 근접 공격 Niagara 이펙트

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	UUW_CharacterHealth* HealthUI; // 캐릭터 체력 UI

	UPROPERTY(BlueprintReadWrite, Category="Character|Attributes")
	AMultiTargetPassiveAbilityBase* CurrentMultiTargetPassiveAbility; // 현재 다중 대상 패시브 능력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	TArray<TSubclassOf<AActiveAbilityBase>> ActiveAbilities; // 액티브 능력 목록

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	TArray<TSubclassOf<APassiveAbilityBase>> PassiveAbilities; // 패시브 능력 목록

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes")
	TArray<TSubclassOf<AAuraBase>> Auras; // 오라 목록

	UPROPERTY(BlueprintReadOnly, Category="Character|Attributes")
	TArray<AActiveAbilityBase*> CurrentActiveAbilities; // 현재 액티브 능력 목록

	UPROPERTY(BlueprintReadOnly, Category="Character|Attributes")
	TArray<APassiveAbilityBase*> CurrentPassiveAbilities; // 현재 패시브 능력 목록

	UPROPERTY(BlueprintReadOnly, Category="Character|Attributes")
	TArray<AAuraBase*> CurrentAuras; // 현재 오라 목록

	UPROPERTY(BlueprintReadOnly, Category="Character|Attributes")
	TArray<ABuffBase*> CurrentBuffs; // 현재 버프 목록

	UPROPERTY(BlueprintReadOnly, Category="Character|Attributes")
	TArray<ADeBuffBase*> CurrentDeBuffs; // 현재 디버프 목록

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	TArray<UAnimMontage*> MeleeAttackMontages; // 근접 공격 몽타주 목록

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	UAnimSequence* DeathAnimation; // 죽음 애니메이션

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	UAnimMontage* StartMovingMontage; // 이동 시작 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	UAnimMontage* EndMovingMontage; // 이동 종료 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	UAnimMontage* RotateRightMontage; // 오른쪽 회전 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	UAnimMontage* RotateLeftMontage; // 왼쪽 회전 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	UAnimMontage* DefendMontage; // 방어 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	UAnimMontage* GetHitMontage; // 피격 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	UAnimMontage* VictoryMontage; // 승리 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	UAnimMontage* IdleAnimation; // 대기 애니메이션

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Attributes|Animations")
	UAnimMontage* MoveAnimation; // 이동 애니메이션

	/*---	      	    Component    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Component")
	USkeletalMeshComponent* SkeletalMesh; // 스켈레탈 메시 컴포넌트

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Component")
	USceneComponent* ProjectileHitLocation; // 발사체 히트 위치 컴포넌트

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Component")
	USpringArmComponent* SpringArm; // 스프링 암 컴포넌트

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Component")
	UWidgetComponent* HealthWidget; // 체력 위젯 컴포넌트

	/*---	      	    Delegate    	      	---*/
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnHealthSet OnHealthSet; // 체력 설정 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnManaSet_Character OnManaSet; // 마나 설정 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnStatsUpdated OnStatsUpdated; // 스탯 업데이트 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnCharacterMoved OnCharacterMoved; // 캐릭터 이동 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnStopAnimationMontage OnStopAnimationMontage; // 애니메이션 몽타주 정지 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnAnimationEnded_Character OnAnimationEnded; // 애니메이션 종료 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnAttacked OnAttacked; // 공격 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnAbilityUsed OnAbilityUsed; // 능력 사용 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnCharacterRotated OnCharacterRotated; // 캐릭터 회전 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnDamageTaken OnDamageTaken; // 피해 입을 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnDead OnDead; // 사망 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnResurrected OnResurrected; // 부활 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnRemovedInstantly OnRemovedInstantly; // 즉시 제거 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnStatusEffectsUpdated OnStatusEffectsUpdated; // 상태 효과 업데이트 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnCharacterHighlighted OnCharacterHighlighted; // 캐릭터 강조 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnCharacterMoveToSlot OnCharacterMoveToSlot; // 슬롯으로 이동할 때 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnCharacterRemovedFromSlot OnCharacterRemovedFromSlot; // 슬롯에서 제거될 때 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnRefreshAuras OnRefreshAuras; // 오라 갱신 시 브로드캐스트되는 델리게이트

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Character|Delegate")
	FOnDragStarted OnDragStarted; // 드래그 시작 시 브로드캐스트되는 델리게이트

protected:
	UPROPERTY()
	FTimeline MoveOverTimeLine; // 이동 시간라인

	UPROPERTY()
	FTimeline RotateOverTimeLine; // 회전 시간라인

	UPROPERTY(BlueprintReadWrite, Category="Character|Settings")
	UCurveFloat* MoveOverTimeCurve; // 이동 시간 곡선

	UPROPERTY(BlueprintReadWrite, Category="Character|Settings")
	UCurveFloat* RotateOverTimeCurve; // 회전 시간 곡선

private:
	UPROPERTY()
	APlayerController* PlayerController; // 플레이어 컨트롤러

	UPROPERTY()
	ASlotBase* TargetSlot; // 대상 슬롯
};


#pragma region 삭제코드들
/*/**
 * @brief 입력된 버프와 동일한 버프가 이미 적용되었는지 확인하는 함수
 * @param InBuff 확인할 버프
 * @param ExistingBuff 이미 존재하는 동일한 버프(있는 경우)
 * @return 동일한 버프가 이미 존재하면 true, 그렇지 않으면 false
 #1#
bool AlreadyGotBuff(ABuffBase* InBuff, ABuffBase*& ExistingBuff);

/**
 * @brief 입력된 디버프와 동일한 디버프가 이미 적용되었는지 확인하는 함수
 * @param InBuff 확인할 디버프
 * @param ExistingBuff 이미 존재하는 동일한 디버프(있는 경우)
 * @return 동일한 디버프가 이미 존재하면 true, 그렇지 않으면 false
 #1#
bool AlreadyGotDeBuff(ADeBuffBase* InBuff, ADeBuffBase*& ExistingBuff);*/

/**
	 * @brief 캐릭터에게 버프를 추가하는 함수
	 * @param InBuff 추가할 버프
	 */
/*void AddBuff(ABuffBase* InBuff);*/


/*
/**
 * @brief 캐릭터에서 버프를 제거하는 함수
 * @param InBuff 제거할 버프
 #1#
void RemoveBuff(ABuffBase* InBuff);

/**
 * @brief 캐릭터에게 디버프를 추가하는 함수
 * @param InDeBuff 추가할 디버프
 #1#
void AddDeBuff(ADeBuffBase* InDeBuff);

/**
 * @brief 캐릭터에서 디버프를 제거하는 함수
 * @param InDeBuff 제거할 디버프
 #1#
void RemoveDeBuff(ADeBuffBase* InDeBuff);
*/


/*void ACharacterBase::AddBuff(ABuffBase* InBuff)
{
	CurrentBuffs.Add(InBuff);

	if (OnStatusEffectsUpdated.IsBound())
		OnStatusEffectsUpdated.Broadcast();
}

void ACharacterBase::RemoveBuff(ABuffBase* InBuff)
{
	CurrentBuffs.Remove(InBuff);

	if (OnStatusEffectsUpdated.IsBound())
		OnStatusEffectsUpdated.Broadcast();
}

void ACharacterBase::AddDeBuff(ADeBuffBase* InDeBuff)
{
	CurrentDeBuffs.Add(InDeBuff);

	if (OnStatusEffectsUpdated.IsBound())
		OnStatusEffectsUpdated.Broadcast();
}

void ACharacterBase::RemoveDeBuff(ADeBuffBase* InDeBuff)
{
	CurrentDeBuffs.Remove(InDeBuff);

	if (OnStatusEffectsUpdated.IsBound())
		OnStatusEffectsUpdated.Broadcast();
}*/

#pragma endregion  삭제코드들
