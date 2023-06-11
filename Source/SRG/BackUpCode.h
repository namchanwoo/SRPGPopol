

#pragma region BattleController


/*
public:
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

	void ShowCharacterDetails(ACharacterBase* InCharacter);

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

public:
	AGrid* GetGrid() { return Grid; }
	*/


/*
	Field Members
	
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
		USRPGGameInstance* GameInstance;
	
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
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* BattleTheme;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* InitializationTheme;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* DeploymentPhaseTheme;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* WaitingForPlayerActionTheme;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* PlayerIsCastingSpellTheme;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* PlayerIsPlayingTheme;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* WaitingForEnemyActionTheme;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* EnemyIsPlayingTheme;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* VictoryTheme;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* DefeatTheme;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle Controller")
		USoundBase* OverTheme;
	
	private:
		UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		AGrid* Grid;
	
		UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		AAILogicBase* AILogic;
	
		UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TSubclassOf<AExploreHeroBase> DebugHero;
	
		UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TMap<TSubclassOf<AObstacleBase>, int32> DebugObstacleList;
	
	
		UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TArray<FPlayerCharacterData> DebugPlayerCharacterList;
	
		UPROPERTY(EditAnywhere, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TArray<FEnemyCharacterData> DebugEnemyCharacterList;
	
		UPROPERTY(EditDefaultsOnly, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TSubclassOf<UUW_BattleUI> WBP_BattleUIClass;
	
		UPROPERTY(EditDefaultsOnly, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TSubclassOf<UUW_BattleResultUI> WBP_BattleResultUIClass;
	
		UPROPERTY(EditDefaultsOnly, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TSubclassOf<UUW_SpellBookUI> WBP_SpellBookUIClass;
	
		UPROPERTY(EditDefaultsOnly, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TSubclassOf<UUW_BattleSettingsDialogue> WBP_BattleSettingsDialogueClass;
	
		UPROPERTY(EditDefaultsOnly, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TSubclassOf<UUW_YesNoDialogue> WBP_YesNoDialogueClass;
	
		UPROPERTY(EditDefaultsOnly, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TSubclassOf<APassiveAbilityBase> BP_NoEnemyRetaliationAbility;
	
		UPROPERTY(EditDefaultsOnly, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
		TSubclassOf<APassiveAbilityBase> BP_StrikeTwiceAbility;
	
		UPROPERTY(EditDefaultsOnly, Category="Battle Controller|Setting", meta=(AllowPrivateAccess="true"))
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
		USceneComponent* DefaultSceneRoot;
	
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
		*/


#pragma endregion BattleController