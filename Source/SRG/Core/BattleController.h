#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/SRGEnumStruct.h"
#include "BattleController.generated.h"

class UUW_YesNoDialogue;
class UUW_BattleSettingsDialogue;
class UUW_SpellBookUI;
class APassiveAbilityBase;
class AStatusEffectBase;
class ASpellBase;
class ASlotBase;
class AActiveAbilityBase;
class ABattleHeroBase;
class AObstacleBase;
class AExploreHeroBase;
class USRGGameInstance;
class AAILogicBase;
class AGrid;
class ADeploymentController;
class ACharacterBase;
class ARangeCharacterBase;

class UUW_BattleUI;
class UUW_BattleResultUI;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleStateChanged, EBattleState, NewBattleState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewTurn, int32, NewTurn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllPassiveAbilitiesUsed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllDOTsApplied);

/*
 * ABattleController는 게임 내 전투의 로직을 처리하는 클래스입니다. 이 클래스는 전투 관련 모든 작업을 담당하며, 플레이어와 AI의 행동을 조정합니다.
 * ABattleController는 플레이어의 입력을 받아 해당 행동을 수행하거나, AI에게 행동 지시를 내릴 수 있습니다.
 * 또한, 능력의 사용, 캐릭터의 행동 등도 이 클래스에서 관리합니다.
 * 주요 책임은 다음과 같습니다:
 * - 플레이어 입력 처리: 플레이어로부터 입력을 받아 캐릭터가 수행할 행동을 결정합니다.
 * - AI 로직 실행: AI 캐릭터의 행동을 결정하고 지시합니다.
 * - 능력 사용 관리: 캐릭터의 능력 사용을 조정하고 결과를 처리합니다.
 * - 전투 상황 관리: 전투의 진행 상황을 계속 추적하고, 필요에 따라 게임의 상태를 변경합니다.
 * 이 클래스는 게임의 중심적인 부분을 담당하므로, 적절한 분리와 추상화가 필요합니다. 필요에 따라, 책임을 더 세분화하거나 독립된 클래스로 추출할 수 있습니다.
 */
UCLASS()
class SRG_API ABattleController : public AActor
{
	GENERATED_BODY()

public:
	ABattleController();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	void ChangeBattleState(EBattleState NewBattleState);

	void InitializeEvent();

	void DeploymentPhase();

	void PlayerCharacterPlays();

	void EnemyCharacterPlays();

	void SpawnHero();

	void SpawnDebugHero();

	void SpawnEnemyCharacters();

	void SpawnObstacles();

	void SpawnMapObstacles(TMap<TSubclassOf<AObstacleBase>, int32> InObstacleList);

	void PlayBattleTheme();

	void PlayBattleOverTheme();

	void OnSpellClicked(ASpellBase* InSpell);

	void CancelSpell();

	void DisableCurrentSpell();

	void ApplyDOTs();

	void ApplyNextDOT(TArray<AStatusEffectBase*> InAllStatusEffects);

	void MoveAction(ASlotBase* InMoveSlot, ASlotBase* InAttackSlot);

	void MeleeAttackAction(ASlotBase* InAttackSlot);

	void Retaliate();

	void RangeAttack(ASlotBase* InAttackSlot);

	void GetTargetCharacters(ACharacterBase* InAttackingCharacter, ASlotBase* InAttackSlot, ASlotBase* InStandingSlot,
	                         bool IsMelee, TArray<ACharacterBase*>& OutTargetCharacters,
	                         ACharacterBase*& OutMainTargetCharacter);


	float GetAttackBonus(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter);

	float GetDefenseReduction(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter);

	float GetRangeReduction(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter, bool IsMelee);

	void GetBaseDamage(ACharacterBase* InAttackingCharacter, int32& OutBaseDamage, int32& OutMinBaseDamage,
	                   int32& OutMaxBaseDamage);

	void CalculateDamage(ACharacterBase* InAttackingCharacter, TArray<ACharacterBase*> InTargetCharacters, bool IsMelee,
	                     TMap<ACharacterBase*, FDamageData>& OutDamageData, int32& OutTotalDamage);


	bool CanTargetRetaliate();

	void PlayMeleeHitEffect(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter);

	void RemoveBeforeAttackStatusEffects(ACharacterBase* InCharacter);

	void DoubleMeleeAttack();

	void DoubleRangeAttack();

	void ActiveAbility(ASlotBase* InTargetSlot);

	void UseNextPassiveAbility(ACharacterBase* InPlayingCharacter, const TArray<ACharacterBase*>& InHitCharacters,
	                           EPassiveAbilityUseMoment InPassiveAbilityUseMoment);

	void Settings();

	void Surrender();

protected:
	UFUNCTION()
	void OnSurrenderConfirmed();

	UFUNCTION()
	void OnSurrenderCancelled();

	UFUNCTION()
	void OnQuitAccepted();

	UFUNCTION()
	void OnQuitCancelled();

	UFUNCTION()
	void OnContinueHandler();

	UFUNCTION()
	void OnQuitHandler();

	UFUNCTION()
	void OnPassiveAbilityUsed();

	UFUNCTION()
	void OnRotatedForAbility();

	UFUNCTION()
	void OnMovedAbilitiesUsed();

	UFUNCTION()
	void OnMoved();

	UFUNCTION()
	void OnRotatedForMove();

	UFUNCTION()
	void OnAfterMeleeAttackAbilitiesUsed();

	UFUNCTION()
	void OnBeforeMeleeAttackAbilitiesUsed();

	UFUNCTION()
	void OnKillPassiveAbilitiesUsed();

	UFUNCTION()
	void OnRotatedForMeleeAttack();

	UFUNCTION()
	void OnAttacked();

	UFUNCTION()
	void OnAttackAnimationEnded();

	UFUNCTION()
	void OnPlayerCharacterSpawned(bool IsSuccess, ACharacterBase* InPlayerCharacter);

	UFUNCTION()
	void OnPlayerCharacterDead(ACharacterBase* InCharacter);

	UFUNCTION()
	void OnRotatedForRetaliate();

	UFUNCTION()
	void OnBeforeRetaliateAttackAbilitiesUsed();

	UFUNCTION()
	void OnRetaliateAttack();

	UFUNCTION()
	void OnRetaliateAnimationEnded();

	UFUNCTION()
	void OnAfterRetaliateAttackAbilitiesUsed();

	UFUNCTION()
	void OnRetaliateKillPassiveAbilitiesUsed();

	UFUNCTION()
	void OnRotatedForRangeAttack();

	UFUNCTION()
	void OnBeforeRangeAttackAbilitiesUsed();

	UFUNCTION()
	void OnProjectileHitHandler();

	UFUNCTION()
	void OnAfterRangeAttackAbilitiesUsed();

	UFUNCTION()
	void OnRangeKillPassiveAbilitiesUsed();

protected:
	UFUNCTION()
	void OnDOTAppliedHandler(float InDelay);

	UFUNCTION()
	void OnSpellUseStartedHandler();

	UFUNCTION()
	void OnSpellUseEndedHandler();

	UFUNCTION()
	void OnTurnStartAbilitiesUsed();

	UFUNCTION()
	void OnAllDOTsAppliedHandler();

public:
	AGrid* GetGrid() { return Grid; }

	bool HasAnyTargetDied(const TArray<ACharacterBase*>& InTargetCharacters);

	void ClearPassiveAbilityCallback();

	void ClearDOTCallback();

	void UsePassiveAbilities(ACharacterBase* InPlayingCharacter, const TArray<ACharacterBase*>& InHitCharacters,
	                         EPassiveAbilityUseMoment InPassiveAbilityUseMovement);

	TMap<ACharacterBase*, FDamageData> GetElementReduction(EElement InAttackingElement,
	                                                       const TMap<ACharacterBase*, FDamageData>& InDamageData);

	bool SpawnPlayerCharacter(const FPlayerCharacterData& InPlayerCharacterData, ASlotBase* InSlot,
	                          ACharacterBase*& OutPlayerCharacter);

	void IncreaseTurnCount();

	void SetReadyCharacters();

	void SetStarterCharacters();

	void StartBattle();

	void EndBattle(EBattleState InBattleState);

	void PlayVictoryAnimations();

	void ShowBattleResult();

protected:
	UFUNCTION()
	void OnNewTurnAnimationEndedHandler();

	UFUNCTION()
	void OnCharacterTurnStarted();

	UFUNCTION()
	void OnCharacterTurnEnded();

	UFUNCTION()
	void OnTurnEndAbilitiesUsed();

	UFUNCTION()
	void OnBattleResultClosedHandler(EBattleState InResult);

protected:
	UFUNCTION()
	void ShowHeroDetails();

	UFUNCTION()
	void OnDefendAction();

	UFUNCTION()
	void OnWaitAction();

	UFUNCTION()
	void OnEnemyCharacterDead(ACharacterBase* InCharacter);

public:
	ACharacterBase* GetNextReadyCharacter();

	ACharacterBase* GetNextWaitingCharacter();

	void GetCharactersBasedOnMovementRange(TArray<ACharacterBase*>& OutReadyToPlayCharacters,
	                                       TArray<ACharacterBase*>& OutWaitingCharacters);

	void GetCasualties(TMap<TSubclassOf<ACharacterBase>, int32>& PlayerCasualties,
	                   TMap<TSubclassOf<ACharacterBase>, int32>& EnemyCasualties,
	                   int32& TotalExpReward);

public:
	void PlayerCancelActiveAbility();

	void DisableCurrentActiveAbility();

	void AutoPlay();


	void ShowSpells();

protected:
	UFUNCTION()
	void OnSpellBookClosedHandler();

	UFUNCTION()
	void OnSpellClickedHandler(ASpellBase* InSpell);

	UFUNCTION()
	void OnActiveAbilityClicked(AActiveAbilityBase* InActiveAbility);

	UFUNCTION()
	void OnActiveAbilityUseStartedHandler();

	UFUNCTION()
	void OnActiveAbilityUseEndedHandler();

	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	bool bUseManaForUnitAbilities;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	bool bIsAutoPlay;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	bool bIsSecondAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	FName MainMenuLevelName;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	int32 TotalDamage;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	int32 PassiveAbilityIndex;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	int32 DOTIndex;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	int32 CurrentTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	float RangePenaltyDistance = 5;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	EBattleState BattleState;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	USRGGameInstance* GameInstance;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	AAILogicBase* AILogic;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	ADeploymentController* DeploymentController;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	ACharacterBase* MainTargetCharacter;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	ACharacterBase* PlayingCharacter;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	ABattleHeroBase* BattleHero;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TArray<FPlayerCharacterData> DeadPlayerCharacters;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TArray<APassiveAbilityBase*> CurrentPassiveAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Controller")
	TArray<FEnemyCharacterData> DebugEnemyCharacterList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Controller")
	TArray<AStatusEffectBase*> AllStatusEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Controller")
	TArray<FEnemyCharacterData> EnemyCharacterList;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TArray<ACharacterBase*> PlayedCharacters;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TArray<ACharacterBase*> TargetCharacters;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TArray<ACharacterBase*> ReadyToPlayCharacters;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	AActiveAbilityBase* CurrentActiveAbility;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	ASpellBase* CurrentSpell;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	ACharacterBase* Boss;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TArray<ACharacterBase*> WaitingCharacters;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TArray<ACharacterBase*> PlayerCharacters;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TArray<ACharacterBase*> EnemyCharacters;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TArray<FPlayerCharacterData> DeadEnemyCharacters;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TMap<ACharacterBase*, FDamageData> DamageDealt;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TMap<TSubclassOf<ACharacterBase>, int32> PlayerStarterCharacters;

	UPROPERTY(BlueprintReadOnly, Category="Battle Controller")
	TMap<TSubclassOf<ACharacterBase>, int32> EnemyStarterCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* BattleTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* InitializationTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* DeploymentPhaseTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* WaitingForPlayerActionTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* PlayerIsCastingSpellTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* PlayerIsPlayingTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* WaitingForEnemyActionTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* EnemyIsPlayingTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* VictoryTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* DefeatTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	USoundBase* OverTheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	TSubclassOf<AExploreHeroBase> DebugHero;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Controller")
	TMap<TSubclassOf<AObstacleBase>, int32> DebugObstacleList;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	AGrid* Grid;

private:
	UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUW_BattleUI> WBP_BattleUIClass;

	UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUW_BattleResultUI> WBP_BattleResultUIClass;

	UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUW_SpellBookUI> WBP_SpellBookUIClass;

	UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUW_BattleSettingsDialogue> WBP_BattleSettingsDialogueClass;

	UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUW_YesNoDialogue> WBP_YesNoDialogueClass;

	UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
	TSubclassOf<APassiveAbilityBase> BP_NoEnemyRetaliationAbility;

	UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
	TSubclassOf<APassiveAbilityBase> BP_StrikeTwiceAbility;

	UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
	TSubclassOf<APassiveAbilityBase> BP_DoubleArrowAbility;

	UPROPERTY()
	UUW_BattleUI* UW_BattleUI;

	UPROPERTY()
	UUW_BattleResultUI* NewBattleResultUI;

	UPROPERTY()
	UUW_SpellBookUI* UW_SpellBookUI;

	UPROPERTY()
	UUW_BattleSettingsDialogue* UW_BattleSettingsDialogue;

	UPROPERTY()
	UUW_YesNoDialogue* UW_BattleSettingDialogue_YesNoDialogue;

	UPROPERTY()
	UUW_YesNoDialogue* UW_Surrender_YesNoDialogue;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Controller|Component")
	UAudioComponent* BattleThemeAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Controller|Component")
	UAudioComponent* BattleOverTheme;

private:
	UPROPERTY()
	ASlotBase* CurrentMoveSlot;

	UPROPERTY()
	ASlotBase* CurrentAttackSlot;

	UPROPERTY()
	ASlotBase* CurrentMeleeAttackSlot;

	UPROPERTY()
	ASlotBase* CurrentRangeAttackSlot;

	UPROPERTY()
	ARangeCharacterBase* RangePlayingCharacter;

	UPROPERTY()
	ASlotBase* CurrentActiveAbilitySlot;

	UPROPERTY()
	ACharacterBase* CurrentPassiveAbilityCharacter;

	UPROPERTY()
	TArray<ACharacterBase*> PassiveAbilityHitCharacters;

	EPassiveAbilityUseMoment CurrentPassiveAbilityUseMoment;

	/*---	      	    Delegate Event    	      	---*/
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Controller|Delegate")
	FOnBattleStateChanged OnBattleStateChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Controller|Delegate")
	FOnNewTurn OnNewTurn;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Controller|Delegate")
	FOnAllPassiveAbilitiesUsed OnAllPassiveAbilitiesUsed;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Controller|Delegate")
	FOnAllDOTsApplied OnAllDOTsApplied;
};
