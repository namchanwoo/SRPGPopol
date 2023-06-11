#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
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
class USRPGGameInstance;
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


	/*---	      	    Battle State Management    	      	---*/
public:
	void ChangeBattleState(EBattleState NewBattleState);
	void StartBattle();
	void EndBattle(EBattleState InBattleState);
	void InitializeEvent();
	void DeploymentPhase();
	void PlayerCharacterPlays();
	void EnemyCharacterPlays();


	/*---	      	    Character and Object Spawning    	      	---*/
public:
	void SpawnHero();
	void SpawnDebugHero();
	void SpawnEnemyCharacters();
	void SpawnObstacles();
	void SpawnMapObstacles(TMap<TSubclassOf<AObstacleBase>, int32> InObstacleList);
	bool SpawnPlayerCharacter(const FPlayerCharacterData& InPlayerCharacterData, ASlotBase* InSlot,
	                          ACharacterBase*& OutPlayerCharacter);

private:
	void InitializeBattleHero(const AExploreHeroBase* HeroCDO, int32 InExp, int32 InGold,
												 int32 InCurrentMana, const TArray<TSubclassOf<AEquipmentBase>>& InBackPack,
												 const TArray<TSubclassOf<AEquipmentBase>>& InEquipment);
	
	/*---	      	    Battle Theme Management    	      	---*/
public:
	void PlayBattleTheme();
	void PlayBattleOverTheme();
	void PlayVictoryAnimations();


	/*---	      	    Spell and Ability Management    	      	---*/
public:
	void OnSpellClicked(ASpellBase* InSpell);
	void CancelSpell();
	void DisableCurrentSpell();
	void ApplyDOTs();
	void ApplyNextDOT(TArray<AStatusEffectBase*> InAllStatusEffects);
	void ActiveAbility(ASlotBase* InTargetSlot);
	void UseNextPassiveAbility(ACharacterBase* InPlayingCharacter, const TArray<ACharacterBase*>& InHitCharacters,
	                           EPassiveAbilityUseMoment InPassiveAbilityUseMoment);
	void PlayerCancelActiveAbility();
	void DisableCurrentActiveAbility();
	void ShowSpells();


	/*---	      	    Battle Actions    	      	---*/
public:
	void MoveAction(ASlotBase* InMoveSlot, ASlotBase* InAttackSlot);
	void MeleeAttackAction(ASlotBase* InAttackSlot);
	void Retaliate();
	void RangeAttack(ASlotBase* InAttackSlot);
	void DoubleMeleeAttack();
	void DoubleRangeAttack();
	void AutoPlay();


	/*---	      	    Battle Calculations    	      	---*/
public:
	float GetAttackBonus(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter);
	float GetDefenseReduction(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter);
	float GetRangeReduction(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter, bool IsMelee);
	void GetBaseDamage(ACharacterBase* InAttackingCharacter, int32& OutBaseDamage, int32& OutMinBaseDamage,
	                   int32& OutMaxBaseDamage);
	void CalculateDamage(ACharacterBase* InAttackingCharacter, const TArray<ACharacterBase*>& InTargetCharacters,
	                     bool IsMelee,
	                     TMap<ACharacterBase*, FDamageData>& OutDamageData, int32& OutTotalDamage);
	TMap<ACharacterBase*, FDamageData> GetElementReduction(EElement InAttackingElement,
	                                                       const TMap<ACharacterBase*, FDamageData>& InDamageData);
	bool CanTargetRetaliate();
	bool HasAnyTargetDied(const TArray<ACharacterBase*>& InTargetCharacters);
	void GetCasualties(TMap<TSubclassOf<ACharacterBase>, int32>& PlayerCasualties,
	                   TMap<TSubclassOf<ACharacterBase>, int32>& EnemyCasualties, int32& TotalExpReward);


	/*---	      	    Character and Battle Information    	      	---*/
public:
	void GetTargetCharacters(ACharacterBase* InAttackingCharacter, ASlotBase* InAttackSlot, ASlotBase* InStandingSlot,
	                         bool IsMelee, TArray<ACharacterBase*>& OutTargetCharacters,
	                         ACharacterBase*& OutMainTargetCharacter);
	void ShowCharacterDetails(ACharacterBase* InCharacter);
	ACharacterBase* GetNextReadyCharacter();
	ACharacterBase* GetNextWaitingCharacter();


	/*---	      	    Battle Settings and Results    	      	---*/
public:
	void Settings();
	void Surrender();
	void ShowBattleResult();
	void IncreaseTurnCount();
	void SetReadyCharacters();
	void SetStarterCharacters();


	/*---	      	    Event Handlers    	      	---*/
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
	void OnNewTurnAnimationEndedHandler();
	UFUNCTION()
	void OnCharacterTurnStarted();
	UFUNCTION()
	void OnCharacterTurnEnded();
	UFUNCTION()
	void OnTurnEndAbilitiesUsed();
	UFUNCTION()
	void OnBattleResultClosedHandler(EBattleState InResult);
	UFUNCTION()
	void OnSpellBookClosedHandler();
	UFUNCTION()
	void OnSpellClickedHandler(ASpellBase* InSpell);

public:
	UFUNCTION()
	void OnActiveAbilityClicked(AActiveAbilityBase* InActiveAbility);
	UFUNCTION()
	void OnActiveAbilityUseStartedHandler();
	UFUNCTION()
	void OnActiveAbilityUseEndedHandler();


	/*---	      	    Character Actions and Status    	      	---*/
protected:
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
	UFUNCTION()
	void ShowHeroDetails();
	UFUNCTION()
	void OnDefendAction();
	UFUNCTION()
	void OnWaitAction();
	UFUNCTION()
	void OnEnemyCharacterDead(ACharacterBase* InCharacter);


	/*---	      	    Ability and Status Management    	      	---*/
public:
	void ClearPassiveAbilityCallback();
	void ClearDOTCallback();
	void UsePassiveAbilities(ACharacterBase* InPlayingCharacter, const TArray<ACharacterBase*>& InHitCharacters,
	                         EPassiveAbilityUseMoment InPassiveAbilityUseMovement);
	void RemoveBeforeAttackStatusEffects(ACharacterBase* InCharacter);
	void PlayMeleeHitEffect(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter);
	void GetCharactersBasedOnMovementRange(TArray<ACharacterBase*>& OutReadyToPlayCharacters,
	                                       TArray<ACharacterBase*>& OutWaitingCharacters);


	/*---	      	    Grid Management    	      	---*/
public:
	AGrid* GetGrid() { return Grid; }


	/*******************************************
	 * Battle State Management
	 *******************************************/
public:
	// 유닛 능력에 마나가 사용되는지 여부를 결정합니다.
	UPROPERTY(BlueprintReadWrite, Category="Battle State Management")
	bool bUseManaForUnitAbilities;

	// 게임이 자동 플레이 모드인지 확인
	UPROPERTY(BlueprintReadWrite, Category="Battle State Management")
	bool bIsAutoPlay;

	// 두 번째 공격이 가능한지 결정
	UPROPERTY(BlueprintReadWrite, Category="Battle State Management")
	bool bIsSecondAttack;

	// 메인 메뉴 레벨의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle State Management")
	FName MainMenuLevelName;

	// 전투에서 입힌 총 피해
	UPROPERTY(BlueprintReadWrite, Category="Battle State Management")
	int32 TotalDamage;

	// 사용 중인 패시브 능력의 인덱스
	UPROPERTY(BlueprintReadWrite, Category="Battle State Management")
	int32 PassiveAbilityIndex;

	// 지속 피해 지수 효과
	UPROPERTY(BlueprintReadWrite, Category="Battle State Management")
	int32 DOTIndex;

	// 전투의 현재 턴
	UPROPERTY(BlueprintReadWrite, Category="Battle State Management")
	int32 CurrentTurn;

	// 범위 페널티가 적용되는 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle State Management")
	float RangePenaltyDistance = 5;

	// 현재 전투 상황
	UPROPERTY(BlueprintReadWrite, Category="Battle State Management")
	EBattleState BattleState;

	/*******************************************
	 * Game Instance and Controllers
	 *******************************************/
public:
	// 게임 인스턴스에 대한 참조
	UPROPERTY(BlueprintReadWrite, Category="Game Instance and Controllers")
	USRPGGameInstance* SRPGGameInstance;

	// 배포 컨트롤러에 대한 참조
	UPROPERTY(BlueprintReadWrite, Category="Game Instance and Controllers")
	ADeploymentController* DeploymentController;

	/*******************************************
	 * Character and Ability Management
	 *******************************************/
public:
	// 주요 대상 캐릭터에 대한 참조
	UPROPERTY(BlueprintReadWrite, Category="Character and Ability Management")
	ACharacterBase* MainTargetCharacter;

	// 현재 플레이 중인 캐릭터 참조
	UPROPERTY(BlueprintReadWrite, Category="Character and Ability Management")
	ACharacterBase* PlayingCharacter;

	// 전투 영웅 참조
	UPROPERTY(BlueprintReadWrite, Category="Character and Ability Management")
	ABattleHeroBase* BattleHero;

	// 현재 활성 능력에 대한 참조
	UPROPERTY(BlueprintReadWrite, Category="Character and Ability Management")
	AActiveAbilityBase* CurrentActiveAbility;

	// 현재 주문에 대한 참조
	UPROPERTY(BlueprintReadWrite, Category="Character and Ability Management")
	ASpellBase* CurrentSpell;

	// 보스 캐릭터 참고
	UPROPERTY(BlueprintReadWrite, Category="Character and Ability Management")
	ACharacterBase* Boss;


	/*******************************************
	 * Character Lists
	 *******************************************/
public:
	// 사망한 플레이어 캐릭터 목록
	UPROPERTY(BlueprintReadOnly, Category="Character Lists")
	TArray<FPlayerCharacterData> DeadPlayerCharacters;

	// 자신의 턴을 플레이한 캐릭터 목록
	UPROPERTY(BlueprintReadOnly, Category="Character Lists")
	TArray<ACharacterBase*> PlayedCharacters;

	// 잠재적 대상 캐릭터 목록
	UPROPERTY(BlueprintReadOnly, Category="Character Lists")
	TArray<ACharacterBase*> TargetCharacters;

	// 자신의 턴을 플레이할 준비가 된 캐릭터 목록
	UPROPERTY(BlueprintReadOnly, Category="Character Lists")
	TArray<ACharacterBase*> ReadyToPlayCharacters;

	// 차례를 기다리는 캐릭터 목록
	UPROPERTY(BlueprintReadOnly, Category="Character Lists")
	TArray<ACharacterBase*> WaitingCharacters;

	// 플레이어 캐릭터 일람
	UPROPERTY(BlueprintReadOnly, Category="Character Lists")
	TArray<ACharacterBase*> PlayerCharacters;

	// 적 캐릭터 목록
	UPROPERTY(BlueprintReadOnly, Category="Character Lists")
	TArray<ACharacterBase*> EnemyCharacters;

	// 사망한 적 캐릭터 목록
	UPROPERTY(BlueprintReadOnly, Category="Character Lists")
	TArray<FPlayerCharacterData> DeadEnemyCharacters;

	/*******************************************
 	* Ability and Effect Lists
 	*******************************************/
public:
	// 현재 지속 능력 목록
	UPROPERTY(BlueprintReadOnly, Category="Ability and Effect Lists")
	TArray<APassiveAbilityBase*> CurrentPassiveAbilities;

	// 모든 상태 효과 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability and Effect Lists")
	TArray<AStatusEffectBase*> AllStatusEffects;

	// 적 캐릭터 데이터 일람
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability and Effect Lists")
	TArray<FEnemyCharacterData> EnemyCharacterList;

	/*******************************************
	 * Damage and Character Data
	 *******************************************/
public:
	// 캐릭터가 입힌 데미지에 대한 캐릭터 맵
	UPROPERTY(BlueprintReadOnly, Category="Damage and Character Data")
	TMap<ACharacterBase*, FDamageData> DamageDealt;

	// 플레이어 스타터 캐릭터의 카운트에 대한 캐릭터 클래스 맵
	UPROPERTY(BlueprintReadOnly, Category="Damage and Character Data")
	TMap<TSubclassOf<ACharacterBase>, int32> PlayerStarterCharacters;

	// 적 스타터 캐릭터의 카운트에 대한 캐릭터 클래스 맵
	UPROPERTY(BlueprintReadOnly, Category="Damage and Character Data")
	TMap<TSubclassOf<ACharacterBase>, int32> EnemyStarterCharacters;

	/*******************************************
 	* Sound Themes
 	*******************************************/
public:
	// 전투를 위한 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* BattleTheme;

	// 초기화를 위한 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* InitializationTheme;

	// 배포 단계의 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* DeploymentPhaseTheme;

	// 플레이어 동작 대기를 위한 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* WaitingForPlayerActionTheme;

	// 플레이어 캐스팅 주문의 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* PlayerIsCastingSpellTheme;

	// 플레이어 플레이를 위한 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* PlayerIsPlayingTheme;

	// 적의 행동을 기다리는 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* WaitingForEnemyActionTheme;

	// 적의 연주를 위한 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* EnemyIsPlayingTheme;

	// 승리를 위한 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* VictoryTheme;

	// 패배를 위한 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* DefeatTheme;

	// 게임 오버 사운드 테마
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound Themes")
	USoundBase* OverTheme;

	/*******************************************
	 * Debug Settings
	 *******************************************/
private:
	// 디버그 설정을 위한 그리드
	UPROPERTY(EditAnywhere, Category="Debug Settings", meta=(AllowPrivateAccess="true"))
	AGrid* Grid;

	// 디버그 설정을 위한 AI 로직
	UPROPERTY(EditAnywhere, Category="Debug Settings", meta=(AllowPrivateAccess="true"))
	AAILogicBase* AILogic;

	// 디버그 설정용 Hero
	UPROPERTY(EditAnywhere, Category="Debug Settings", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AExploreHeroBase> DebugHero;

	// 디버그 설정을 위한 장애물 목록
	UPROPERTY(EditAnywhere, Category="Debug Settings", meta=(AllowPrivateAccess="true"))
	TMap<TSubclassOf<AObstacleBase>, int32> DebugObstacleList;

	// 디버그 설정을 위한 플레이어 캐릭터 목록
	UPROPERTY(EditAnywhere, Category="Debug Settings", meta=(AllowPrivateAccess="true"))
	TArray<FPlayerCharacterData> DebugPlayerCharacterList;

	// 디버그 설정을 위한 적 캐릭터 목록
	UPROPERTY(EditAnywhere, Category="Debug Settings", meta=(AllowPrivateAccess="true"))
	TArray<FEnemyCharacterData> DebugEnemyCharacterList;


	/*******************************************
 	* UI Widgets
 	*******************************************/
private:
	// 전투 UI 위젯
	UPROPERTY(EditDefaultsOnly, Category="UI Widgets", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUW_BattleUI> WBP_BattleUIClass;

	// 전투 결과 UI 위젯
	UPROPERTY(EditDefaultsOnly, Category="UI Widgets", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUW_BattleResultUI> WBP_BattleResultUIClass;

	// 마법서 UI 위젯
	UPROPERTY(EditDefaultsOnly, Category="UI Widgets", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUW_SpellBookUI> WBP_SpellBookUIClass;

	// 전투 설정 대화 UI 위젯
	UPROPERTY(EditDefaultsOnly, Category="UI Widgets", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUW_BattleSettingsDialogue> WBP_BattleSettingsDialogueClass;

	// YesNo 대화 UI 위젯
	UPROPERTY(EditDefaultsOnly, Category="UI Widgets", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUW_YesNoDialogue> WBP_YesNoDialogueClass;


	/*******************************************
	 * Passive Abilities
	 *******************************************/
private:
	// 적의 보복 능력 없음
	UPROPERTY(EditDefaultsOnly, Category="Passive Abilities", meta=(AllowPrivateAccess="true"))
	TSubclassOf<APassiveAbilityBase> BP_NoEnemyRetaliationAbility;

	// 2회 공격 능력
	UPROPERTY(EditDefaultsOnly, Category="Passive Abilities", meta=(AllowPrivateAccess="true"))
	TSubclassOf<APassiveAbilityBase> BP_StrikeTwiceAbility;

	// 이중 화살표 능력
	UPROPERTY(EditDefaultsOnly, Category="Passive Abilities", meta=(AllowPrivateAccess="true"))
	TSubclassOf<APassiveAbilityBase> BP_DoubleArrowAbility;

	/*******************************************
	 * Current Gameplay State
	 *******************************************/
private:
	// 현재 패시브 능력 사용 순간                                        
	EPassiveAbilityUseMoment CurrentPassiveAbilityUseMoment;

	// 현재 이동 슬롯
	UPROPERTY()
	ASlotBase* CurrentMoveSlot;

	// 현재 공격 슬롯
	UPROPERTY()
	ASlotBase* CurrentAttackSlot;

	// 현재 근접 공격 슬롯
	UPROPERTY()
	ASlotBase* CurrentMeleeAttackSlot;

	// 현재 범위 공격 슬롯
	UPROPERTY()
	ASlotBase* CurrentRangeAttackSlot;

	// 레인지 플레잉 캐릭터
	UPROPERTY()
	ARangeCharacterBase* RangePlayingCharacter;

	// 현재 활성 능력 슬롯
	UPROPERTY()
	ASlotBase* CurrentActiveAbilitySlot;

	// 현재 패시브 능력 캐릭터
	UPROPERTY()
	ACharacterBase* CurrentPassiveAbilityCharacter;

	// 패시브 능력에 맞은 캐릭터
	UPROPERTY()
	TArray<ACharacterBase*> PassiveAbilityHitCharacters;


	/*******************************************
	* UI Instances
	*******************************************/
private:
	// Instance of Battle UI
	UPROPERTY()
	UUW_BattleUI* UW_BattleUI;

	// Instance of Battle Result UI
	UPROPERTY()
	UUW_BattleResultUI* NewBattleResultUI;

	// Instance of Spell Book UI
	UPROPERTY()
	UUW_SpellBookUI* UW_SpellBookUI;

	// Instance of Battle Settings Dialogue
	UPROPERTY()
	UUW_BattleSettingsDialogue* UW_BattleSettingsDialogue;

	// Instance of Yes/No Dialogue for Battle Settings
	UPROPERTY()
	UUW_YesNoDialogue* UW_BattleSettingDialogue_YesNoDialogue;

	// Instance of Yes/No Dialogue for Surrender
	UPROPERTY()
	UUW_YesNoDialogue* UW_Surrender_YesNoDialogue;

	/*******************************************
	 * BattleController Components
	 *******************************************/
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle Controller|Components")
	USceneComponent* DefaultSceneRoot;

	// 배틀 테마용 오디오 구성 요소
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle Controller|Components")
	UAudioComponent* BattleThemeAudio;

	// Battle Over Theme의 오디오 구성 요소
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle Controller|Components")
	UAudioComponent* BattleOverTheme;


	/*******************************************
	 * Delegate Events
	 *******************************************/
public:
	// 전투 상태가 변경될 때 이벤트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegate Events")
	FOnBattleStateChanged OnBattleStateChanged;

	// 새로운 턴이 시작될 때 이벤트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegate Events")
	FOnNewTurn OnNewTurn;

	// 패시브 능력을 모두 사용했을 때의 이벤트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegate Events")
	FOnAllPassiveAbilitiesUsed OnAllPassiveAbilitiesUsed;

	// 모든 DOT 적용 시 이벤트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegate Events")
	FOnAllDOTsApplied OnAllDOTsApplied;
};
