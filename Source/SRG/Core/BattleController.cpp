#include "BattleController.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "RPGSaveGame.h"
#include "SRPGGameInstance.h"

#include "SRGCore/Libraries/SRPGUtilityLibrary.h"
#include "SRGCore/Utilities/SRGLog.h"

#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "SRG/Abilities/MultiTargetPassiveAbilities/MultiTargetPassiveAbilityBase.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "SRG/Gird/EnemySpawnLocation.h"
#include "SRG/Gird/Grid.h"
#include "SRG/Gird/HeroSpawnLocation.h"
#include "SRG/Heros/BattleHeroBase.h"
#include "SRG/Libraries/SRPGFunctionLibrary.h"
#include "SRG/Spells/SpellBase.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "SRG/Units/Obstacles/ObstacleBase.h"
#include "SRG/StatusEffects/Buffs/BuffBase.h"
#include "SRG/Gird/Slots/SlotBase.h"
#include "SRG/StatusEffects/DeBuffs/DeBuffBase.h"
#include "SRG/Units/Characters/RangeCharacters/RangeCharacterBase.h"
#include "SRG/Abilities/PassiveAbilities/PassiveAbilityBase.h"

#include "AILogicBase.h"
#include "DeploymentController.h"

#include "SRG/Widgets/UW_ActionButton.h"
#include "SRG/Widgets/UW_BattleResultUI.h"
#include "SRG/Widgets/UW_BattleUI.h"
#include "SRG/Widgets/UW_ControlPanel.h"
#include "SRG/Widgets/UW_BattleSettingsDialogue.h"
#include "SRG/Widgets/ExploreWidgets/UW_YesNoDialogue.h"
#include "SRG/Widgets/UW_BossHP.h"
#include "SRG/Widgets/UW_SpellBookUI.h"


ABattleController::ABattleController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	// BattleOverTheme 오디오 구성 요소 만들기
	BattleOverTheme = CreateDefaultSubobject<UAudioComponent>(TEXT("BattleOverTheme"));
	BattleOverTheme->bIsUISound = true;
	BattleOverTheme->bAutoActivate = false;
	BattleOverTheme->SetupAttachment(RootComponent);

	// BattleThemeAudio 오디오 구성 요소 만들기
	BattleThemeAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("BattleThemeAudio"));
	BattleThemeAudio->bIsUISound = true;
	BattleThemeAudio->bAutoActivate = false;
	BattleThemeAudio->SetupAttachment(RootComponent);

	MainMenuLevelName = FName(TEXT("MainMenu"));
}

void ABattleController::BeginPlay()
{
	Super::BeginPlay();
	ChangeBattleState(EBattleState::Initialization);
}

void ABattleController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UW_BattleUI)
	{
		UW_BattleUI->OnDefendAction.RemoveDynamic(this, &ABattleController::OnDefendAction);
		UW_BattleUI->OnWaitAction.RemoveDynamic(this, &ABattleController::OnWaitAction);
	}

	if (BattleHero)
	{
		BattleHero->OnRightClicked.RemoveDynamic(this, &ABattleController::ShowHeroDetails);
	}
}

void ABattleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::ChangeBattleState(EBattleState NewBattleState)
{
	BattleState = NewBattleState;

	if (OnBattleStateChanged.IsBound())
		OnBattleStateChanged.Broadcast(BattleState);

	switch (BattleState)
	{
	case EBattleState::Initialization:
		InitializeEvent();
		break;

	case EBattleState::DeploymentPhase:
		DeploymentPhase();
		break;

	case EBattleState::WaitingForPlayerAction:
		HandleWaitingForPlayerAction();
		break;

	case EBattleState::PlayerIsCastingSpell:
		HandlePlayerIsCastingSpell();
		break;

	case EBattleState::PlayerIsPlaying:
		HandlePlayerIsPlaying();
		break;

	case EBattleState::WaitingForEnemyAction:
		HandleWaitingForEnemyAction();
		break;

	default: break;
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::InitializeEvent()
{
	// 게임 인스턴스를 검색하고 SRPGGameInstance 클래스로 캐스팅합니다.
	SRPGGameInstance = Cast<USRPGGameInstance>(UGameplayStatics::GetGameInstance(this));

	// 캐스팅이 성공했는지 확인
	SRPG_IF_CHECK(SRPGGameInstance!=nullptr, TEXT("SRPGGameInstance 캐스팅에 실패했습니다."));

	// 그리드 초기화
	Grid->InitializeEvent();

	// AI 로직 초기화
	AILogic->InitializeEvent(this, Grid);

	// 영웅 캐릭터 스폰
	SpawnHero();

	// 적 캐릭터 스폰
	SpawnEnemyCharacters();

	// 장애물 생성
	SpawnObstacles();

	// 배틀 테마 플레이
	PlayBattleTheme();

	// WBP_BattleUIClass가 설정되어 있는지 확인
	SRPG_IF_CHECK(WBP_BattleUIClass, TEXT("WBP_BattleUIClass 클래스를 설정하지 않았습니다."));

	// 전투 UI 위젯 생성 및 초기화
	UW_BattleUI = CreateWidget<UUW_BattleUI>(GetWorld(), WBP_BattleUIClass);
	UW_BattleUI->BattleController = this;
	UW_BattleUI->AddToViewport();
	UW_BattleUI->InitializeEvent();
	UW_BattleUI->OnDefendAction.AddDynamic(this, &ABattleController::OnDefendAction);
	UW_BattleUI->OnWaitAction.AddDynamic(this, &ABattleController::OnWaitAction);

	// 플레이어 컨트롤러를 가져오고 입력 모드를 게임 및 UI로 설정합니다.
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UW_BattleUI);

	// 전투 상태를 배치 단계로 변경
	ChangeBattleState(EBattleState::DeploymentPhase);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::DeploymentPhase()
{
	if (UWorld* World = GetWorld())
	{
		const FTransform SpawnTransform = FTransform(FVector(999999.0f, 999999.0f, 999999.0f));
		ADeploymentController* NewDeploymentController = World->SpawnActorDeferred<ADeploymentController>(
			ADeploymentController::StaticClass(), SpawnTransform);
		if (NewDeploymentController)
		{
			NewDeploymentController->BattleController = this;
			NewDeploymentController->Grid = Grid;
			UGameplayStatics::FinishSpawningActor(NewDeploymentController, SpawnTransform);
		}

		DeploymentController = NewDeploymentController;
		DeploymentController->InitializeEvent();
		UW_BattleUI->StartDeploymentPhase();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::HandleWaitingForPlayerAction()
{
	WaitingCharacters.Remove(PlayingCharacter);
	PlayerCharacterPlays();

	if (!bIsAutoPlay)
	{
		UW_BattleUI->EnableActionButtons(PlayingCharacter->bIsWaiting);
		UW_BattleUI->ActivateControlPanel(!bIsAutoPlay);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::HandlePlayerIsCastingSpell()
{
	if (Grid->OnCursorChanged.IsBound())
		Grid->OnCursorChanged.Broadcast(EBattleCursorType::Wait, nullptr, nullptr, nullptr);

	UW_BattleUI->DisableActionButtons();
	UW_BattleUI->DisableActiveAbilities();
	UW_BattleUI->ActivateControlPanel(false);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::HandlePlayerIsPlaying()
{
	UW_BattleUI->DisableActionButtons();
	UW_BattleUI->DisableActiveAbilities();
	UW_BattleUI->ActivateControlPanel(false);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::HandleWaitingForEnemyAction()
{
	WaitingCharacters.Remove(PlayingCharacter);
	EnemyCharacterPlays();
	PlayingCharacter->SetWaiting(false);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::PlayerCharacterPlays()
{
	if (bIsAutoPlay)
	{
		ChangeBattleState(EBattleState::PlayerIsPlaying);
		AILogic->RunAILogic(PlayingCharacter);
	}
	else
	{
		Grid->ShowMovementForPlayerCharacter(PlayingCharacter);
		UW_BattleUI->ShowActionButtons();
		UW_BattleUI->ShowActiveAbilityList(PlayingCharacter);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::EnemyCharacterPlays()
{
	FTimerHandle EnemyCharacterPlaysTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(EnemyCharacterPlaysTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		ChangeBattleState(EBattleState::EnemyIsPlaying);
		AILogic->RunAILogic(PlayingCharacter);

		GetWorld()->GetTimerManager().ClearTimer(EnemyCharacterPlaysTimerHandle);
	}), 1.0f, false);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::SpawnHero()
{
	if (IsValid(SRPGGameInstance->RpgSaveGame))
	{
		int32 loadExp, loadGold, loadCurrentMana;
		TArray<FPlayerCharacterData> loadPlayerCharacterList;
		TSubclassOf<AExploreHeroBase> loadExploreHero;
		TMap<TSubclassOf<AQuestBase>, FQuestStepData> loadActiveQuestData;
		TMap<TSubclassOf<AQuestBase>, int32> loadCompletedQuestData;
		TArray<TSubclassOf<AEquipmentBase>> loadBackPack, loadEquipment;
		SRPGGameInstance->RpgSaveGame->LoadHero(loadExp, loadGold, loadCurrentMana, loadPlayerCharacterList,
		                                        loadExploreHero, loadActiveQuestData, loadCompletedQuestData,
		                                        loadBackPack, loadEquipment);

		const AExploreHeroBase* loadExploreHeroCDO = loadExploreHero->GetDefaultObject<AExploreHeroBase>();
		SpawnBattleHero(loadExploreHeroCDO, loadExp, loadGold, loadCurrentMana, loadBackPack, loadEquipment);
	}
	else
	{
		SpawnDebugHero();
	}
}

void ABattleController::SpawnDebugHero()
{
	SRPG_IF_CHECK(DebugHero, TEXT("DebugHero를 설정하지 않았습니다."));

	const AExploreHeroBase* DebugHeroCDO = DebugHero->GetDefaultObject<AExploreHeroBase>();
	const int32 HeroLevel = USRPGFunctionLibrary::GetHeroLevel(DebugHeroCDO->Exp, DebugHeroCDO->ExpExponent,
	                                                           DebugHeroCDO->MaxLevel);
	const FHeroStats HeroStats = USRPGFunctionLibrary::CalculateHeroStats(
		HeroLevel, DebugHeroCDO->MaxLevel, DebugHeroCDO->InitialStats, DebugHeroCDO->FirstLevelStats,
		DebugHeroCDO->LastLevelStats);

	const int32 CurrentMana = HeroStats.Knowledge * 10;
	SpawnBattleHero(DebugHeroCDO, DebugHeroCDO->Exp, DebugHeroCDO->Gold, CurrentMana, DebugHeroCDO->BackPack,
	                DebugHeroCDO->Equipment);
}

void ABattleController::SpawnBattleHero(const AExploreHeroBase* HeroCDO, int32 InExp, int32 InGold,
                                        int32 InCurrentMana, const TArray<TSubclassOf<AEquipmentBase>>& InBackPack,
                                        const TArray<TSubclassOf<AEquipmentBase>>& InEquipment)
{
	const int32 HeroLevel = USRPGFunctionLibrary::GetHeroLevel(InExp, HeroCDO->ExpExponent, HeroCDO->MaxLevel);
	const FHeroStats HeroStats = USRPGFunctionLibrary::CalculateHeroStats(
		HeroLevel, HeroCDO->MaxLevel, HeroCDO->InitialStats, HeroCDO->FirstLevelStats, HeroCDO->LastLevelStats);

	UWorld* World = GetWorld();
	SRPG_CHECK(World);

	const FTransform SpawnTransform = Grid->HeroSpawnLocation->GetActorTransform();
	BattleHero = World->SpawnActorDeferred<ABattleHeroBase>(HeroCDO->BattleHero, SpawnTransform);
	SRPG_CHECK(BattleHero);

	BattleHero->BattleController = this;
	BattleHero->HeroStats = HeroStats;
	BattleHero->BackPack = InBackPack;
	BattleHero->Equipment = InEquipment;
	BattleHero->Level = HeroLevel;
	BattleHero->Gold = InGold;
	BattleHero->CurrentMana = InCurrentMana;
	BattleHero->Name = HeroCDO->Name;
	BattleHero->HeroImage = HeroCDO->HeroImage;
	BattleHero->HeroDetailsImage = HeroCDO->HeroDetailsImage;
	BattleHero->Spells = HeroCDO->Spells;

	UGameplayStatics::FinishSpawningActor(BattleHero, SpawnTransform);

	BattleHero->InitializeEvent();
	BattleHero->OnRightClicked.AddDynamic(this, &ABattleController::ShowHeroDetails);
	Grid->HeroSpawnLocation->Destroy();
}

void ABattleController::SpawnEnemyCharacters()
{
	// 생성할 적 캐릭터 목록을 선택합니다.
	TArray<FEnemyCharacterData>& SelectEnemyCharacterList = SRPGGameInstance->EnemyCharacterList.Num() > 0
		                                                        ? SRPGGameInstance->EnemyCharacterList
		                                                        : DebugEnemyCharacterList;

	int32 Index = 0;
	for (const FEnemyCharacterData& InEnemyCharacterList : SelectEnemyCharacterList)
	{
		// 인덱스가 유효한지 확인하고 그렇지 않으면 false를 반환합니다.
		if (!Grid->EnemySpawnLocations.IsValidIndex(Index))
		{
			Index++;
			continue;
		}

		// 인덱스의 스폰 위치가 유효한지 확인하고 유효하지 않으면 false를 반환합니다.
		if (!IsValid(Grid->EnemySpawnLocations[Index]))
		{
			Index++;
			continue;
		}

		// 문자 데이터가 유효한지 확인하고 유효하지 않으면 false를 반환합니다.
		if (!IsValid(InEnemyCharacterList.Character))
		{
			Index++;
			continue;
		}

		// 생성된 캐릭터를 보관할 캐릭터 포인터를 선언합니다.
		ACharacterBase* SpawnedCharacter = nullptr;

		// 캐릭터 생성을 시도하고 실패하면 인덱스를 증가시키고 계속합니다.
		if (!Grid->SpawnCharacter(InEnemyCharacterList.Character,
		                          Grid->EnemySpawnLocations[Index]->Slot->Coordinates, true,
		                          FMath::RandRange(InEnemyCharacterList.MinStack, InEnemyCharacterList.MaxStack),
		                          FHeroStats(), SpawnedCharacter))
		{
			Index++;
			continue;
		}

		// 캐릭터가 성공적으로 생성되면 적 목록에 추가하고 해당 이벤트를 초기화한 다음 OnEnemyCharacterDead 메서드를 해당 OnDead 이벤트에 추가합니다.
		EnemyCharacters.Add(SpawnedCharacter);
		SpawnedCharacter->InitializeEvent();
		SpawnedCharacter->OnDead.AddDynamic(this, &ABattleController::OnEnemyCharacterDead);

		// 스폰된 캐릭터가 보스인 경우 보스 변수에 할당합니다.
		if (SpawnedCharacter->bIsBoss)
		{
			Boss = SpawnedCharacter;
		}

		// 인덱스를 증가시킵니다.
		Index++;
	}

	// 모든 캐릭터를 생성한 후 적의 생성 위치를 제거합니다.
	Grid->RemoveEnemySpawnLocation();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::SpawnObstacles()
{
	if (IsValid(SRPGGameInstance))
	{
		SpawnMapObstacles(SRPGGameInstance->ObstacleList);
	}
	else
	{
		SpawnMapObstacles(DebugObstacleList);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::SpawnMapObstacles(TMap<TSubclassOf<AObstacleBase>, int32> InObstacleList)
{
	auto CurrentObstacleList = InObstacleList;
	TArray<ASlotBase*> AvailableSlots = Grid->GetSlotsForObstacleSpawn();


	for (TTuple<TSubclassOf<AObstacleBase>, int32> InObstacleListPair : CurrentObstacleList)
	{
		for (int32 i = 0; i <= InObstacleListPair.Value; i++)
		{
			USRPGUtilityLibrary::ShuffleArray<ASlotBase*>(AvailableSlots);

			for (int32 j = 0; j < AvailableSlots.Num(); j++)
			{
				const auto ObstacleCDO = InObstacleListPair.Key->GetDefaultObject<AObstacleBase>();
				bool AllFree;
				TArray<ASlotBase*> SlotsInRange;
				Grid->GetSlotsInRange(AvailableSlots[j], ObstacleCDO->UnitSize, true, SlotsInRange, AllFree);

				if (!AllFree)
				{
					continue;
				}

				AObstacleBase* SpawnedObstacle;
				Grid->SpawnObstacle(InObstacleListPair.Key, AvailableSlots[j]->Coordinates, SpawnedObstacle);

				for (int32 k = SlotsInRange.Num() - 1; k >= 0; --k)
				{
					AvailableSlots.Remove(SlotsInRange[j]);
				}

				break;
			}
		}
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::PlayBattleTheme()
{
	if (IsValid(BattleTheme))
	{
		BattleThemeAudio->SetSound(BattleTheme);
		BattleThemeAudio->Play();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::PlayBattleOverTheme()
{
	BattleThemeAudio->FadeOut(1.0f, 0.0f);

	USoundBase* BattleStateTheme = nullptr;;
	switch (BattleState)
	{
	case EBattleState::Initialization:
		BattleStateTheme = InitializationTheme;
		break;

	case EBattleState::DeploymentPhase:
		BattleStateTheme = DeploymentPhaseTheme;
		break;

	case EBattleState::WaitingForPlayerAction:
		BattleStateTheme = WaitingForPlayerActionTheme;
		break;

	case EBattleState::PlayerIsCastingSpell:
		BattleStateTheme = PlayerIsCastingSpellTheme;
		break;

	case EBattleState::PlayerIsPlaying:
		BattleStateTheme = PlayerIsPlayingTheme;
		break;

	case EBattleState::WaitingForEnemyAction:
		BattleStateTheme = WaitingForEnemyActionTheme;
		break;

	case EBattleState::EnemyIsPlaying:
		BattleStateTheme = EnemyIsPlayingTheme;
		break;

	case EBattleState::Victory:
		BattleStateTheme = VictoryTheme;
		break;

	case EBattleState::Defeat:
		BattleStateTheme = DefeatTheme;
		break;

	case EBattleState::Over:
		BattleStateTheme = OverTheme;
		break;

	default:
		break;
	}

	if (IsValid(BattleStateTheme))
	{
		BattleOverTheme->SetSound(BattleStateTheme);
		BattleOverTheme->Play();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnSpellClicked(ASpellBase* InSpell)
{
	DisableCurrentSpell();
	PlayerCancelActiveAbility();

	if (BattleHero->CurrentMana >= InSpell->ManaCost)
	{
		CurrentSpell = InSpell;
		CurrentSpell->OnSpellUseEnded.AddDynamic(this, &ABattleController::OnSpellUseEndedHandler);

		Grid->ResetSlots();

		CurrentSpell->OnSpellUseStarted.AddDynamic(this, &ABattleController::OnSpellUseStartedHandler);

		CurrentSpell->OnSpellEnabled();
	}
	else
	{
		const FText Message = FText::FromString(TEXT("Not enough mana for this spell"));
		UW_BattleUI->ShowMessage(Message, true);
		UW_BattleUI->UW_ControlPanel->UW_SpellBookButton->ActivateButton(false);
		CurrentSpell = nullptr;
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::CancelSpell()
{
	if (BattleState == EBattleState::WaitingForPlayerAction && IsValid(CurrentSpell))
	{
		DisableCurrentSpell();
		ChangeBattleState(EBattleState::WaitingForPlayerAction);
		Grid->CheckSlotUnderMouse();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::DisableCurrentSpell()
{
	if (IsValid(CurrentSpell))
	{
		CurrentSpell->OnSpellDisabled();
		UW_BattleUI->UW_ControlPanel->UW_SpellBookButton->ActivateButton(false);
		CurrentSpell = nullptr;
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::ApplyDOTs()
{
	DOTIndex = 0;
	AllStatusEffects.Append(PlayingCharacter->CurrentBuffs);
	AllStatusEffects.Append(PlayingCharacter->CurrentDeBuffs);
	ApplyNextDOT(AllStatusEffects);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::ApplyNextDOT(TArray<AStatusEffectBase*> InAllStatusEffects)
{
	if (DOTIndex >= InAllStatusEffects.Num())
	{
		if (OnAllDOTsApplied.IsBound())
		{
			OnAllDOTsApplied.Broadcast();
		}
	}
	else
	{
		if (IsValid(PlayingCharacter))
		{
			if (!PlayingCharacter->IsDead())
			{
				if (InAllStatusEffects[DOTIndex]->bIsDOT)
				{
					InAllStatusEffects[DOTIndex]->OnDOTApplied.
					                              AddDynamic(this, &ABattleController::OnDOTAppliedHandler);
					InAllStatusEffects[DOTIndex]->PlayParticleEffect();
					InAllStatusEffects[DOTIndex]->PlaySound();
					InAllStatusEffects[DOTIndex]->OnDOT();
					return;
				}
			}
			OnDOTAppliedHandler(0.0f);
		}
		else
		{
			OnDOTAppliedHandler(0.0f);
		}
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::MoveAction(ASlotBase* InMoveSlot, ASlotBase* InAttackSlot)
{
	CurrentMoveSlot = InMoveSlot;
	CurrentAttackSlot = InAttackSlot;

	PlayingCharacter->OnCharacterRotated.AddDynamic(this, &ABattleController::OnRotatedForMove);

	ASlotBase* NewTargetSlot = nullptr;
	if (PlayingCharacter->MovementType == EMovementType::Walk)
	{
		CurrentMoveSlot->Path[0];
	}
	else if (PlayingCharacter->MovementType == EMovementType::Fly)
	{
		NewTargetSlot = CurrentMoveSlot;
	}

	if (IsValid(NewTargetSlot))
	{
		PlayingCharacter->Rotate(NewTargetSlot);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::MeleeAttackAction(ASlotBase* InAttackSlot)
{
	CurrentMeleeAttackSlot = InAttackSlot;
	PlayingCharacter->OnCharacterRotated.AddDynamic(this, &ABattleController::OnRotatedForMeleeAttack);

	TMap<FVector2D, ASlotBase*> AllSlots = Grid->GetAllSlots(CurrentMeleeAttackSlot->ContainedUnit->UnitSize);

	if (!AllSlots.Contains(CurrentMeleeAttackSlot->ContainedUnit->Slot->Coordinates))
	{
		return;
	}

	ASlotBase* NewTargetSlot = *AllSlots.Find(CurrentMeleeAttackSlot->ContainedUnit->Slot->Coordinates);
	PlayingCharacter->Rotate(NewTargetSlot);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::Retaliate()
{
	MainTargetCharacter->OnCharacterRotated.AddDynamic(this, &ABattleController::OnRotatedForRetaliate);

	auto AllSlots = Grid->GetAllSlots(PlayingCharacter->UnitSize);
	if (!AllSlots.Contains(PlayingCharacter->Slot->Coordinates))
	{
		return;
	}
	ASlotBase* NewTargetSlot = *AllSlots.Find(PlayingCharacter->Slot->Coordinates);
	MainTargetCharacter->Rotate(NewTargetSlot);

	MainTargetCharacter->CurrentRetaliationCount--;
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::RangeAttack(ASlotBase* InAttackSlot)
{
	CurrentRangeAttackSlot = InAttackSlot;
	PlayingCharacter->OnCharacterRotated.AddDynamic(this, &ABattleController::OnRotatedForRangeAttack);
	auto AllSlots = Grid->GetAllSlots(CurrentRangeAttackSlot->ContainedUnit->UnitSize);
	if (!AllSlots.Contains(CurrentRangeAttackSlot->ContainedUnit->Slot->Coordinates))
	{
		return;
	}
	ASlotBase* NewTargetSlot = *AllSlots.Find(CurrentRangeAttackSlot->ContainedUnit->Slot->Coordinates);
	PlayingCharacter->Rotate(NewTargetSlot);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::GetTargetCharacters(ACharacterBase* InAttackingCharacter, ASlotBase* InAttackSlot,
                                            ASlotBase* InStandingSlot, bool IsMelee,
                                            TArray<ACharacterBase*>& OutTargetCharacters,
                                            ACharacterBase*& OutMainTargetCharacter)
{
	if (IsValid(InAttackingCharacter->CurrentMultiTargetPassiveAbility))
	{
		if (IsMelee == InAttackingCharacter->CurrentMultiTargetPassiveAbility->bIsMelee)
		{
			OutTargetCharacters = InAttackingCharacter->CurrentMultiTargetPassiveAbility->GetEnemiesInAbilityRange(
				InAttackSlot, InStandingSlot);
			OutMainTargetCharacter = Cast<ACharacterBase>(InAttackSlot->ContainedUnit);
			return;
		}
	}

	OutMainTargetCharacter = Cast<ACharacterBase>(InAttackSlot->ContainedUnit);
	const TArray<ACharacterBase*> NewTargetCharacters = {OutMainTargetCharacter};
	OutTargetCharacters = NewTargetCharacters;
}

/*ToDo: 기능 구현 확인되지 않음*/
float ABattleController::GetAttackBonus(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter)
{
	return FMath::Clamp((InAttackingCharacter->CurrentAttack - InTargetCharacter->CurrentDefense) * 0.05f, 0.0f, 3.0f);
}

/*ToDo: 기능 구현 확인되지 않음*/
float ABattleController::GetDefenseReduction(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter)
{
	return FMath::Clamp((InTargetCharacter->CurrentDefense - InAttackingCharacter->CurrentAttack) * 0.025f, 0.0f, 0.7f);
}

/*ToDo: 기능 구현 확인되지 않음*/
float ABattleController::GetRangeReduction(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter,
                                           bool IsMelee)
{
	ARangeCharacterBase* CastRangeCharacter = Cast<ARangeCharacterBase>(InAttackingCharacter);
	if (CastRangeCharacter)
	{
		const float Distance = Grid->GetDistanceToSlot(InAttackingCharacter->Slot, InTargetCharacter->Slot);
		const float DistanceValue = Distance > RangePenaltyDistance ? 0.5f : 0.0f;
		return IsMelee ? 0.5f : DistanceValue;
	}

	return 0.0f;
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::GetBaseDamage(ACharacterBase* InAttackingCharacter, int32& OutBaseDamage,
                                      int32& OutMinBaseDamage, int32& OutMaxBaseDamage)
{
	auto currentAttackingCharacter = InAttackingCharacter;

	int32 LastIndex = FMath::Clamp(currentAttackingCharacter->Stack, 1, 10);

	int32 currentBaseDamage = 0;

	for (int32 i = 1; i <= LastIndex; i++)
	{
		currentBaseDamage = currentBaseDamage + FMath::RandRange(currentAttackingCharacter->CurrentMinDamage,
		                                                         currentAttackingCharacter->CurrentMaxDamage);
	}

	const int32 StackDamage = FMath::Floor(currentBaseDamage * (currentAttackingCharacter->Stack / 10.0f));
	OutBaseDamage = currentAttackingCharacter->Stack > 10 ? StackDamage : currentBaseDamage;
	OutMinBaseDamage = currentAttackingCharacter->CurrentMinDamage * currentAttackingCharacter->Stack;
	OutMaxBaseDamage = currentAttackingCharacter->CurrentMaxDamage * currentAttackingCharacter->Stack;
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::CalculateDamage(ACharacterBase* InAttackingCharacter,
                                        const TArray<ACharacterBase*>& InTargetCharacters, bool IsMelee,
                                        TMap<ACharacterBase*, FDamageData>& OutDamageData,
                                        int32& OutTotalDamage)
{
	ACharacterBase* currentAttackingCharacter = InAttackingCharacter;
	EElement currentAttackingElement = {};

	if (IsValid(CurrentActiveAbility))
	{
		TArray<ACharacterBase*> AllyCharacters = {currentAttackingCharacter};
		CurrentActiveAbility->ApplyStatusEffects(AllyCharacters, InTargetCharacters, true);
		currentAttackingElement = CurrentActiveAbility->Element;
	}

	bool currentIsMelee = IsMelee;

	TMap<ACharacterBase*, FDamageData> currentDamageData;
	for (ACharacterBase* currentTargetCharacter : InTargetCharacters)
	{
		FDamageData currentDamage;

		const float increase1 = GetAttackBonus(currentAttackingCharacter, currentTargetCharacter);
		const float reduction1 = GetDefenseReduction(currentAttackingCharacter, currentTargetCharacter);
		const float reduction2 = GetRangeReduction(currentAttackingCharacter, currentTargetCharacter, currentIsMelee);
		const float MultiplierValue = ((1 + increase1) * (1 - reduction1)) * (1 - reduction2);

		int32 BaseDamage, MinBaseDamage, MaxBaseDamage;
		GetBaseDamage(currentAttackingCharacter, BaseDamage, MinBaseDamage, MaxBaseDamage);

		currentDamage.Damage = FMath::Max(FMath::Floor(BaseDamage * MultiplierValue), 1);
		currentDamage.MinDamage = FMath::Max(FMath::Floor(MinBaseDamage * MultiplierValue), 1);
		currentDamage.MaxDamage = FMath::Max(FMath::Floor(MaxBaseDamage * MultiplierValue), 1);
		currentDamage.DamageModifier = MultiplierValue;
		currentDamage.Element = EElement::Normal;

		currentDamageData.Add(currentTargetCharacter, currentDamage);
		RemoveBeforeAttackStatusEffects(currentTargetCharacter);
	}

	RemoveBeforeAttackStatusEffects(currentAttackingCharacter);

	auto ElementReduction = GetElementReduction(currentAttackingElement, currentDamageData);

	int32 currentTotalDamage = 0;
	for (const TTuple<ACharacterBase*, FDamageData>& InReduction : ElementReduction)
	{
		currentTotalDamage += InReduction.Value.Damage;
	}

	OutDamageData = ElementReduction;
	OutTotalDamage = currentTotalDamage;
}

/*ToDo: 기능 구현 확인되지 않음*/
bool ABattleController::CanTargetRetaliate()
{
	return !PlayingCharacter->PassiveAbilities.Contains(BP_NoEnemyRetaliationAbility) &&
		MainTargetCharacter->CurrentRetaliationCount > 0;
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::PlayMeleeHitEffect(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter)
{
	if (IsValid(InAttackingCharacter->MeleeHitEffect))
	{
		const FVector SpawnLocation = InTargetCharacter->ProjectileHitLocation->GetComponentLocation();
		UGameplayStatics::SpawnEmitterAtLocation(this, InAttackingCharacter->MeleeHitEffect, SpawnLocation);
		return;
	}
	if (IsValid(InAttackingCharacter->NiagaraMeleeHitEffect))
	{
		const FVector SpawnLocation = InTargetCharacter->ProjectileHitLocation->GetComponentLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, InAttackingCharacter->NiagaraMeleeHitEffect,
		                                               SpawnLocation);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::RemoveBeforeAttackStatusEffects(ACharacterBase* InCharacter)
{
	for (int i = InCharacter->CurrentBuffs.Num() - 1; i >= 0; --i)
	{
		if (InCharacter->CurrentBuffs[i]->bRemoveAfterAttack)
		{
			InCharacter->CurrentBuffs[i]->RemoveStatusEffect();
		}
	}

	for (int i = InCharacter->CurrentDeBuffs.Num() - 1; i >= 0; --i)
	{
		if (InCharacter->CurrentDeBuffs[i]->bRemoveAfterAttack)
		{
			InCharacter->CurrentDeBuffs[i]->RemoveStatusEffect();
		}
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::DoubleMeleeAttack()
{
	if (PlayingCharacter->PassiveAbilities.Contains(BP_StrikeTwiceAbility) && !bIsSecondAttack)
	{
		bIsSecondAttack = true;
		MeleeAttackAction(MainTargetCharacter->Slot);
	}
	else
	{
		OnCharacterTurnEnded();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::DoubleRangeAttack()
{
	if (PlayingCharacter->CurrentAmmo > 0 && !bIsSecondAttack &&
		PlayingCharacter->PassiveAbilities.Contains(BP_DoubleArrowAbility))
	{
		bIsSecondAttack = true;
		RangeAttack(MainTargetCharacter->Slot);
	}
	else
	{
		OnCharacterTurnEnded();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::ActiveAbility(ASlotBase* InTargetSlot)
{
	CurrentActiveAbilitySlot = InTargetSlot;
	PlayingCharacter->OnCharacterRotated.AddDynamic(this, &ABattleController::OnRotatedForAbility);
	if (IsValid(CurrentActiveAbilitySlot->ContainedUnit))
	{
		TMap<FVector2D, ASlotBase*> AllSlots = Grid->GetAllSlots(CurrentActiveAbilitySlot->ContainedUnit->UnitSize);
		ASlotBase* FindSlot = *AllSlots.Find(CurrentActiveAbilitySlot->ContainedUnit->Slot->Coordinates);
		PlayingCharacter->Rotate(FindSlot);
	}
	else
	{
		PlayingCharacter->Rotate(CurrentActiveAbilitySlot);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::UseNextPassiveAbility(ACharacterBase* InPlayingCharacter,
                                              const TArray<ACharacterBase*>& InHitCharacters,
                                              EPassiveAbilityUseMoment InPassiveAbilityUseMoment)
{
	CurrentPassiveAbilityCharacter = InPlayingCharacter;
	PassiveAbilityHitCharacters = InHitCharacters;
	CurrentPassiveAbilityUseMoment = InPassiveAbilityUseMoment;
	if (PassiveAbilityIndex >= CurrentPassiveAbilities.Num())
	{
		if (OnAllPassiveAbilitiesUsed.IsBound())
		{
			OnAllPassiveAbilitiesUsed.Broadcast();
		}
	}
	else
	{
		if (CurrentPassiveAbilities[PassiveAbilityIndex]->UseMoment == InPassiveAbilityUseMoment &&
			UKismetMathLibrary::RandomBoolWithWeight(CurrentPassiveAbilities[PassiveAbilityIndex]->UseChance))
		{
			OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnPassiveAbilityUsed);
			CurrentPassiveAbilities[PassiveAbilityIndex]->UsePassiveAbility(InHitCharacters);
		}
		else
		{
			OnPassiveAbilityUsed();
		}
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::Settings()
{
	UW_BattleSettingsDialogue = CreateWidget<UUW_BattleSettingsDialogue>(GetWorld(), WBP_BattleSettingsDialogueClass);
	UW_BattleSettingsDialogue->OnContinueClicked_BattleSetting.AddDynamic(this, &ABattleController::OnContinueHandler);
	UW_BattleSettingsDialogue->OnQuitClicked.AddDynamic(this, &ABattleController::OnQuitHandler);
	UW_BattleSettingsDialogue->AddToViewport();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::Surrender()
{
	UW_Surrender_YesNoDialogue = CreateWidget<UUW_YesNoDialogue>(GetWorld(), WBP_YesNoDialogueClass);
	UW_Surrender_YesNoDialogue->DialogueTitle = FText::FromString(TEXT("Surrender"));
	UW_Surrender_YesNoDialogue->DialogueText = FText::FromString(
		TEXT("Are you sure you want to surrender? You will lose all your characters."));
	UW_Surrender_YesNoDialogue->YesButtonText = FText::FromString(TEXT("Yes"));
	UW_Surrender_YesNoDialogue->NoButtonText = FText::FromString(TEXT("No"));

	UW_Surrender_YesNoDialogue->OnYesClicked.AddDynamic(this, &ABattleController::OnSurrenderConfirmed);
	UW_Surrender_YesNoDialogue->OnYesClicked.AddDynamic(this, &ABattleController::OnSurrenderCancelled);

	UW_Surrender_YesNoDialogue->AddToViewport();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::ShowCharacterDetails(ACharacterBase* InCharacter)
{
	UW_BattleUI->ShowCharacterDetails(InCharacter);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnSurrenderConfirmed()
{
	if (IsValid(UW_Surrender_YesNoDialogue))
	{
		UW_Surrender_YesNoDialogue->RemoveFromParent();
		UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnSurrenderCancelled()
{
	if (IsValid(UW_Surrender_YesNoDialogue))
	{
		UW_Surrender_YesNoDialogue->RemoveFromParent();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnQuitAccepted()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnQuitCancelled()
{
	UW_BattleSettingDialogue_YesNoDialogue->RemoveFromParent();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnContinueHandler()
{
	if (IsValid(UW_BattleSettingsDialogue))
	{
		UW_BattleSettingsDialogue->RemoveFromParent();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnQuitHandler()
{
	SRPG_CHECK(WBP_YesNoDialogueClass);
	UW_BattleSettingDialogue_YesNoDialogue = CreateWidget<UUW_YesNoDialogue>(GetWorld(), WBP_YesNoDialogueClass);
	UW_BattleSettingDialogue_YesNoDialogue->DialogueTitle = FText::FromString(TEXT("Quit"));
	UW_BattleSettingDialogue_YesNoDialogue->DialogueText = FText::FromString(
		TEXT("Are you sure you want to Quit? You will lose your progress."));
	UW_BattleSettingDialogue_YesNoDialogue->YesButtonText = FText::FromString(TEXT("Yes"));
	UW_BattleSettingDialogue_YesNoDialogue->NoButtonText = FText::FromString(TEXT("No"));

	UW_BattleSettingDialogue_YesNoDialogue->OnYesClicked.AddDynamic(this, &ABattleController::OnQuitAccepted);
	UW_BattleSettingDialogue_YesNoDialogue->OnNoClicked.AddDynamic(this, &ABattleController::OnQuitCancelled);
	UW_BattleSettingDialogue_YesNoDialogue->AddToViewport();

	FTimerHandle BattleSettingsDialogueTimerHandle; // 타이머의 핸들을 정의하십시오.
	GetWorld()->GetTimerManager().SetTimer(BattleSettingsDialogueTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if (IsValid(UW_BattleSettingsDialogue))
		{
			UW_BattleSettingsDialogue->RemoveFromParent();
		}
		GetWorld()->GetTimerManager().ClearTimer(BattleSettingsDialogueTimerHandle);
	}), 0.15f, false);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnPassiveAbilityUsed()
{
	CurrentPassiveAbilities[PassiveAbilityIndex]->ClearPassiveAbilityUseCallback();
	PassiveAbilityIndex++;
	UseNextPassiveAbility(CurrentPassiveAbilityCharacter, PassiveAbilityHitCharacters, CurrentPassiveAbilityUseMoment);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnRotatedForAbility()
{
	PlayingCharacter->OnCharacterRotated.RemoveDynamic(this, &ABattleController::OnRotatedForAbility);
	CurrentActiveAbility->SetTargetSlot(CurrentActiveAbilitySlot);
	CurrentActiveAbility->PlayAbilityAnimation();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnMovedAbilitiesUsed()
{
	ClearPassiveAbilityCallback();
	OnCharacterTurnEnded();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnMoved()
{
	PlayingCharacter->OnCharacterMoved.RemoveDynamic(this, &ABattleController::OnMoved);
	Grid->ClearMovementPath();
	Grid->ResetSlots();
	ASlotBase* NewAnchorSlot = Grid->Slots.Contains(CurrentMoveSlot->Coordinates)
		                           ? *Grid->Slots.Find(CurrentMoveSlot->Coordinates)
		                           : nullptr;

	PlayingCharacter->SetUnitAnchorSlot(NewAnchorSlot);
	Grid->SetUnitOnSlot(PlayingCharacter, CurrentMoveSlot);

	if (IsValid(CurrentAttackSlot))
	{
		MeleeAttackAction(CurrentAttackSlot);;
	}
	else
	{
		OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnMovedAbilitiesUsed);
		UsePassiveAbilities(PlayingCharacter, TargetCharacters, EPassiveAbilityUseMoment::OnMoved);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnRotatedForMove()
{
	PlayingCharacter->OnCharacterRotated.RemoveDynamic(this, &ABattleController::OnRotatedForMove);
	PlayingCharacter->OnCharacterMoved.AddDynamic(this, &ABattleController::OnMoved);
	PlayingCharacter->Move();
	Grid->RemoveUnitOnSlot(PlayingCharacter);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnAfterMeleeAttackAbilitiesUsed()
{
	ClearPassiveAbilityCallback();
	if (!HasAnyTargetDied(TargetCharacters))
	{
		if (CanTargetRetaliate())
		{
			Retaliate();
		}
		else
		{
			DoubleMeleeAttack();
		}
	}
	else
	{
		OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnKillPassiveAbilitiesUsed);
		UsePassiveAbilities(PlayingCharacter, TargetCharacters, EPassiveAbilityUseMoment::OnKill);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnBeforeMeleeAttackAbilitiesUsed()
{
	ClearPassiveAbilityCallback();

	if (IsValid(CurrentActiveAbility))
	{
		CurrentActiveAbility->PlayAbilityAnimation();

		UAnimMontage* CurrentActiveAbilityMontage = *CurrentActiveAbility->UseAbilityMontage.Find(
			CurrentActiveAbility->AbilityOwner->GetClass());

		if (IsValid(CurrentActiveAbilityMontage))
		{
			PlayingCharacter->OnAbilityUsed.AddDynamic(this, &ABattleController::OnAttacked);
			PlayingCharacter->PlayAnimationMontage(CurrentActiveAbilityMontage);
			PlayingCharacter->OnAnimationEnded.AddDynamic(this, &ABattleController::OnAttackAnimationEnded);
			return;
		}
	}

	PlayingCharacter->OnAttacked.AddDynamic(this, &ABattleController::OnAttacked);
	PlayingCharacter->PlayMeleeAttackAnimation();
	PlayingCharacter->OnAnimationEnded.AddDynamic(this, &ABattleController::OnAttackAnimationEnded);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnKillPassiveAbilitiesUsed()
{
	ClearPassiveAbilityCallback();
	const TArray<ACharacterBase*> NewTargetCharacters = {MainTargetCharacter};
	if (!HasAnyTargetDied(NewTargetCharacters))
	{
		if (CanTargetRetaliate())
		{
			Retaliate();
		}
		else
		{
			DoubleMeleeAttack();
		}
	}
	else
	{
		OnCharacterTurnEnded();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnRotatedForMeleeAttack()
{
	PlayingCharacter->OnCharacterRotated.RemoveDynamic(this, &ABattleController::OnRotatedForMeleeAttack);
	GetTargetCharacters(PlayingCharacter, CurrentMeleeAttackSlot, PlayingCharacter->Slot, true,
	                    TargetCharacters, MainTargetCharacter);
	OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnBeforeMeleeAttackAbilitiesUsed);
	UsePassiveAbilities(PlayingCharacter, TargetCharacters, EPassiveAbilityUseMoment::BeforeAttack);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnAttacked()
{
	CalculateDamage(PlayingCharacter, TargetCharacters, true, DamageDealt, TotalDamage);

	if (IsValid(CurrentActiveAbility))
	{
		const TArray<ACharacterBase*> AllyCharacters = {PlayingCharacter};
		CurrentActiveAbility->ApplyStatusEffects(AllyCharacters, TargetCharacters, false);
	}

	for (TTuple<ACharacterBase*, FDamageData> InDamageDealt : DamageDealt)
	{
		InDamageDealt.Key->TakeDamageEvent(InDamageDealt.Value.Damage, InDamageDealt.Value.Element);
		PlayMeleeHitEffect(PlayingCharacter, InDamageDealt.Key);
	}
	DisableCurrentActiveAbility();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnAttackAnimationEnded()
{
	PlayingCharacter->ClearMeleeAttackCallback();
	PlayingCharacter->ClearAbilityCallback();
	OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnAfterMeleeAttackAbilitiesUsed);
	UsePassiveAbilities(PlayingCharacter, TargetCharacters, EPassiveAbilityUseMoment::AfterAttack);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnPlayerCharacterSpawned(bool IsSuccess, ACharacterBase* InPlayerCharacter)
{
	if (!IsSuccess)
	{
		return;
	}

	PlayerCharacters.Add(InPlayerCharacter);
	InPlayerCharacter->InitializeEvent();
	InPlayerCharacter->OnDead.AddDynamic(this, &ABattleController::OnPlayerCharacterDead);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnPlayerCharacterDead(ACharacterBase* InCharacter)
{
	Grid->RemoveUnitOnSlot(InCharacter);
	PlayerCharacters.Remove(InCharacter);

	if (BattleState != EBattleState::DeploymentPhase)
	{
		DeadPlayerCharacters.Add(FPlayerCharacterData(InCharacter->GetClass(), InCharacter->StartingStack));

		if (PlayerCharacters.Num() == 0)
		{
			EndBattle(EBattleState::Defeat);
		}
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnRotatedForRetaliate()
{
	MainTargetCharacter->OnCharacterRotated.RemoveDynamic(this, &ABattleController::OnRotatedForRetaliate);

	GetTargetCharacters(MainTargetCharacter, PlayingCharacter->Slot, MainTargetCharacter->Slot,
	                    true, TargetCharacters, PlayingCharacter);

	OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnBeforeRetaliateAttackAbilitiesUsed);
	UsePassiveAbilities(MainTargetCharacter, TargetCharacters, EPassiveAbilityUseMoment::BeforeAttack);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnBeforeRetaliateAttackAbilitiesUsed()
{
	ClearPassiveAbilityCallback();
	MainTargetCharacter->OnAttacked.AddDynamic(this, &ABattleController::OnRetaliateAttack);
	MainTargetCharacter->OnAnimationEnded.AddDynamic(this, &ABattleController::OnRetaliateAnimationEnded);
	MainTargetCharacter->PlayMeleeAttackAnimation();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnRetaliateAttack()
{
	CalculateDamage(MainTargetCharacter, TargetCharacters, true, DamageDealt, TotalDamage);

	for (const TTuple<ACharacterBase*, FDamageData>& InDamageDealt : DamageDealt)
	{
		InDamageDealt.Key->TakeDamageEvent(InDamageDealt.Value.Damage, InDamageDealt.Value.Element);
		PlayMeleeHitEffect(MainTargetCharacter, InDamageDealt.Key);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnRetaliateAnimationEnded()
{
	MainTargetCharacter->ClearMeleeAttackCallback();
	OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnAfterRetaliateAttackAbilitiesUsed);
	UsePassiveAbilities(MainTargetCharacter, TargetCharacters, EPassiveAbilityUseMoment::AfterAttack);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnAfterRetaliateAttackAbilitiesUsed()
{
	ClearPassiveAbilityCallback();
	if (!HasAnyTargetDied(TargetCharacters))
	{
		DoubleMeleeAttack();
	}
	else
	{
		OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnRetaliateKillPassiveAbilitiesUsed);
		UsePassiveAbilities(MainTargetCharacter, TargetCharacters, EPassiveAbilityUseMoment::OnKill);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnRetaliateKillPassiveAbilitiesUsed()
{
	ClearPassiveAbilityCallback();
	TArray<ACharacterBase*> NewTargetCharacters = {PlayingCharacter};
	if (!HasAnyTargetDied(NewTargetCharacters))
	{
		DoubleMeleeAttack();
	}
	else
	{
		OnCharacterTurnEnded();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnRotatedForRangeAttack()
{
	PlayingCharacter->OnCharacterRotated.RemoveDynamic(this, &ABattleController::OnRotatedForRangeAttack);

	GetTargetCharacters(PlayingCharacter, CurrentRangeAttackSlot, CurrentRangeAttackSlot, false,
	                    TargetCharacters, MainTargetCharacter);

	OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnBeforeRangeAttackAbilitiesUsed);
	UsePassiveAbilities(PlayingCharacter, TargetCharacters, EPassiveAbilityUseMoment::BeforeAttack);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnBeforeRangeAttackAbilitiesUsed()
{
	ClearPassiveAbilityCallback();
	RangePlayingCharacter = Cast<ARangeCharacterBase>(PlayingCharacter);
	SRPG_CHECK(RangePlayingCharacter);

	RangePlayingCharacter->OnProjectileHit.AddDynamic(this, &ABattleController::OnProjectileHitHandler);

	if (IsValid(CurrentActiveAbility))
	{
		CurrentActiveAbility->PlayAbilityAnimation();
		UAnimMontage* ActiveMontage = *CurrentActiveAbility->UseAbilityMontage.Find(
			CurrentActiveAbility->AbilityOwner->GetClass());

		if (IsValid(ActiveMontage))
		{
			RangePlayingCharacter->AbilityAttack(MainTargetCharacter, ActiveMontage);
		}
		else
		{
			RangePlayingCharacter->RangeAttack(MainTargetCharacter);
		}
	}
	else
	{
		RangePlayingCharacter->RangeAttack(MainTargetCharacter);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnProjectileHitHandler()
{
	RangePlayingCharacter->OnProjectileHit.RemoveDynamic(this, &ABattleController::OnProjectileHitHandler);

	CalculateDamage(PlayingCharacter, TargetCharacters, false, DamageDealt, TotalDamage);

	if (IsValid(CurrentActiveAbility))
	{
		const TArray<ACharacterBase*> AllyCharacters = {PlayingCharacter};
		CurrentActiveAbility->ApplyStatusEffects(AllyCharacters, TargetCharacters, false);
	}

	for (const TTuple<ACharacterBase*, FDamageData>& InDamageDealt : DamageDealt)
	{
		InDamageDealt.Key->TakeDamageEvent(InDamageDealt.Value.Damage, InDamageDealt.Value.Element);
	}

	DisableCurrentActiveAbility();

	FTimerHandle ProjectileHitTimerHandle; // 타이머의 핸들을 정의하십시오.

	GetWorld()->GetTimerManager().SetTimer(ProjectileHitTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnAfterRangeAttackAbilitiesUsed);
		UsePassiveAbilities(PlayingCharacter, TargetCharacters, EPassiveAbilityUseMoment::AfterAttack);
		GetWorld()->GetTimerManager().ClearTimer(ProjectileHitTimerHandle);
	}), 1.0f, false);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnAfterRangeAttackAbilitiesUsed()
{
	ClearPassiveAbilityCallback();
	if (!HasAnyTargetDied(TargetCharacters))
	{
		DoubleRangeAttack();
	}
	else
	{
		OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnRangeKillPassiveAbilitiesUsed);
		UsePassiveAbilities(PlayingCharacter, TargetCharacters, EPassiveAbilityUseMoment::OnKill);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnRangeKillPassiveAbilitiesUsed()
{
	ClearPassiveAbilityCallback();
	TArray<ACharacterBase*> InTargetCharacters = {MainTargetCharacter};
	if (!HasAnyTargetDied(InTargetCharacters))
	{
		DoubleRangeAttack();
	}
	else
	{
		OnCharacterTurnEnded();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnDOTAppliedHandler(float InDelay)
{
	if (IsValid(AllStatusEffects[DOTIndex]))
	{
		AllStatusEffects[DOTIndex]->OnDOTApplied.RemoveDynamic(this, &ABattleController::OnDOTAppliedHandler);
		FTimerHandle OnDOTAppliedTimerHandle; // 타이머의 핸들을 정의하십시오.

		GetWorld()->GetTimerManager().SetTimer(OnDOTAppliedTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			DOTIndex++;
			ApplyNextDOT(AllStatusEffects);
			GetWorld()->GetTimerManager().ClearTimer(OnDOTAppliedTimerHandle);
		}), InDelay, false);
	}
	else
	{
		DOTIndex++;
		ApplyNextDOT(AllStatusEffects);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnSpellUseStartedHandler()
{
	ChangeBattleState(EBattleState::PlayerIsCastingSpell);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnSpellUseEndedHandler()
{
	CurrentSpell->OnSpellUseEnded.RemoveDynamic(this, &ABattleController::OnSpellUseEndedHandler);
	DisableCurrentSpell();
	Grid->CheckSlotUnderMouse();

	if (BattleState == EBattleState::PlayerIsCastingSpell)
	{
		ChangeBattleState(EBattleState::WaitingForPlayerAction);
	}
	else if (BattleState == EBattleState::Victory || BattleState == EBattleState::Defeat)
	{
		ShowBattleResult();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnTurnStartAbilitiesUsed()
{
	ClearPassiveAbilityCallback();
	ChangeBattleState(PlayingCharacter->bIsAI
		                  ? EBattleState::WaitingForEnemyAction
		                  : EBattleState::WaitingForPlayerAction);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnAllDOTsAppliedHandler()
{
	ClearDOTCallback();
	if (IsValid(PlayingCharacter))
	{
		if (!PlayingCharacter->IsDead())
		{
			OnAllPassiveAbilitiesUsed.AddDynamic(this, &ABattleController::OnTurnStartAbilitiesUsed);
			UsePassiveAbilities(PlayingCharacter, TargetCharacters, EPassiveAbilityUseMoment::OnTurnStart);
		}
		else
		{
			OnCharacterTurnEnded();
		}
	}
	else
	{
		OnCharacterTurnEnded();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
bool ABattleController::HasAnyTargetDied(const TArray<ACharacterBase*>& InTargetCharacters)
{
	for (ACharacterBase* InTargetCharacter : InTargetCharacters)
	{
		if (IsValid(InTargetCharacter))
		{
			if (InTargetCharacter->IsDead())
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::ClearPassiveAbilityCallback()
{
	OnAllPassiveAbilitiesUsed.RemoveAll(this);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::ClearDOTCallback()
{
	OnAllDOTsApplied.RemoveAll(this);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::UsePassiveAbilities(ACharacterBase* InPlayingCharacter,
                                            const TArray<ACharacterBase*>& InHitCharacters,
                                            EPassiveAbilityUseMoment InPassiveAbilityUseMovement)
{
	PassiveAbilityIndex = 0;
	UseNextPassiveAbility(InPlayingCharacter, InHitCharacters, InPassiveAbilityUseMovement);
}

/*ToDo: 기능 구현 확인되지 않음*/
TMap<ACharacterBase*, FDamageData> ABattleController::GetElementReduction(EElement InAttackingElement,
                                                                          const TMap<ACharacterBase*, FDamageData>&
                                                                          InDamageData)
{
	TMap<ACharacterBase*, FDamageData> currentDamageData;
	for (const TTuple<ACharacterBase*, FDamageData>& DamageData : InDamageData)
	{
		ACharacterBase* CurrentTarget = DamageData.Key;

		float currentElementReduction = 0;
		switch (InAttackingElement)
		{
		case EElement::Normal:
			currentElementReduction = CurrentTarget->CurrentNormalReduction;
			break;
		case EElement::Fire:
			currentElementReduction = CurrentTarget->CurrentFireReduction;
			break;
		case EElement::Earth:
			currentElementReduction = CurrentTarget->CurrentEarthReduction;
			break;
		case EElement::Air:
			currentElementReduction = CurrentTarget->CurrentAirReduction;
			break;
		case EElement::Water:
			currentElementReduction = CurrentTarget->CurrentWaterReduction;
			break;
		default: ;
		}

		currentElementReduction = FMath::Clamp(currentElementReduction, -1.0f, 1.0f);

		FDamageData NewDamageData;
		const FDamageData CurrentTargetDamage = *InDamageData.Find(CurrentTarget);

		NewDamageData.Damage = FMath::Max(CurrentTargetDamage.Damage - FMath::TruncToFloat(
			                                  CurrentTargetDamage.Damage * currentElementReduction), 0);
		NewDamageData.MinDamage = FMath::Max(CurrentTargetDamage.MinDamage - FMath::TruncToFloat(
			                                     CurrentTargetDamage.MinDamage * currentElementReduction), 0);
		NewDamageData.MaxDamage = FMath::Max(CurrentTargetDamage.MaxDamage - FMath::TruncToFloat(
			                                     CurrentTargetDamage.MaxDamage * currentElementReduction), 0);
		NewDamageData.DamageModifier = CurrentTargetDamage.DamageModifier - FMath::TruncToFloat(
			CurrentTargetDamage.DamageModifier * currentElementReduction);
		NewDamageData.Element = InAttackingElement;

		currentDamageData.Add(CurrentTarget, NewDamageData);
	}

	return currentDamageData;
}

/*ToDo: 기능 구현 확인되지 않음*/
bool ABattleController::SpawnPlayerCharacter(const FPlayerCharacterData& InPlayerCharacterData, ASlotBase* InSlot,
                                             ACharacterBase*& OutPlayerCharacter)
{
	ACharacterBase* SpawnedCharacter;
	const bool IsSpawn = Grid->SpawnCharacter(InPlayerCharacterData.Character, InSlot->Coordinates, false,
	                                          InPlayerCharacterData.Stack,
	                                          BattleHero->HeroStats, SpawnedCharacter);

	OnPlayerCharacterSpawned(IsSpawn, SpawnedCharacter);
	OutPlayerCharacter = SpawnedCharacter;
	return IsSpawn;
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::IncreaseTurnCount()
{
	CurrentTurn++;

	if (OnNewTurn.IsBound())
	{
		OnNewTurn.Broadcast(CurrentTurn);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::SetReadyCharacters()
{
	PlayedCharacters.Empty();
	IncreaseTurnCount();

	if (CurrentTurn > 1)
	{
		UW_BattleUI->OnNewTurnAnimationEnded.AddDynamic(this, &ABattleController::OnNewTurnAnimationEndedHandler);
		const FText CurrentTurnText = UKismetTextLibrary::Conv_IntToText(CurrentTurn);
		UW_BattleUI->PlayNewTurnAnimation(CurrentTurnText);
	}
	else
	{
		OnCharacterTurnStarted();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::SetStarterCharacters()
{
	for (const ACharacterBase* InPlayerCharacter : PlayerCharacters)
	{
		if (PlayerStarterCharacters.Contains(InPlayerCharacter->GetClass()))
		{
			auto NewStack = *PlayerStarterCharacters.Find(InPlayerCharacter->GetClass()) + InPlayerCharacter->Stack;
			PlayerStarterCharacters.Add(InPlayerCharacter->GetClass(), NewStack);
		}
		else
		{
			PlayerStarterCharacters.Add(InPlayerCharacter->GetClass(), InPlayerCharacter->Stack);
		}
	}

	for (const ACharacterBase* InEnemyCharacter : EnemyCharacters)
	{
		if (EnemyStarterCharacters.Contains(InEnemyCharacter->GetClass()))
		{
			auto NewStack = *EnemyStarterCharacters.Find(InEnemyCharacter->GetClass()) + InEnemyCharacter->Stack;
			EnemyStarterCharacters.Add(InEnemyCharacter->GetClass(), NewStack);
		}
		else
		{
			EnemyStarterCharacters.Add(InEnemyCharacter->GetClass(), InEnemyCharacter->Stack);
		}
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::StartBattle()
{
	if (PlayerCharacters.Num() > 0)
	{
		UW_BattleUI->OnBattleStarted();
		Grid->ResetSlots();
		SetStarterCharacters();
		DeploymentController->SetNonDeployedCharacters();

		FTimerHandle StartBattleTimerHandle; // 타이머의 핸들을 정의하십시오.

		GetWorld()->GetTimerManager().SetTimer(StartBattleTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			UW_BattleUI->ShowBattleQueue();
			SetReadyCharacters();

			if (IsValid(Boss))
			{
				UW_BattleUI->UW_BossHP->InitializeEvent(Boss);
			}
			GetWorld()->GetTimerManager().ClearTimer(StartBattleTimerHandle);
		}), 1.0f, false);
	}
	else
	{
		UW_BattleUI->ShowMessage(FText::FromString(TEXT("캐릭터를 1개 이상 배포하세요.")), true);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::EndBattle(EBattleState InBattleState)
{
	ChangeBattleState(InBattleState);
	UW_BattleUI->HideBattleQueue();
	UW_BattleUI->HideActionButtons();
	UW_BattleUI->HideCharacterDetails();
	UW_BattleUI->HideActiveAbilityList();
	UW_BattleUI->HideControlPanel();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::PlayVictoryAnimations()
{
	TArray<ACharacterBase*> SelectCharacters;
	if (BattleState == EBattleState::Victory)
	{
		SelectCharacters = PlayerCharacters;
	}
	else if (BattleState == EBattleState::Defeat)
	{
		SelectCharacters = EnemyCharacters;
	}

	if (SelectCharacters.IsEmpty())
	{
		return;
	}

	for (ACharacterBase* SelectCharacter : SelectCharacters)
	{
		SelectCharacter->PlayVictoryAnimation();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::ShowBattleResult()
{
	PlayVictoryAnimations();
	PlayBattleOverTheme();

	FTimerHandle ShowBattleTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ShowBattleTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		TMap<TSubclassOf<ACharacterBase>, int32> PlayerCasualties;
		TMap<TSubclassOf<ACharacterBase>, int32> EnemyCasualties;
		int32 TotalExpReward;
		GetCasualties(PlayerCasualties, EnemyCasualties, TotalExpReward);

		SRPG_CHECK(WBP_BattleResultUIClass);
		NewBattleResultUI = CreateWidget<UUW_BattleResultUI>(GetWorld(), WBP_BattleResultUIClass);
		NewBattleResultUI->PlayerCasualties = PlayerCasualties;
		NewBattleResultUI->EnemyCasualties = EnemyCasualties;
		NewBattleResultUI->Result = BattleState;
		NewBattleResultUI->ExpReward = TotalExpReward;
		NewBattleResultUI->Gold = SRPGGameInstance->Gold;
		NewBattleResultUI->Drops = SRPGGameInstance->Drops;
		NewBattleResultUI->AddToViewport();

		if (Grid->OnCursorChanged.IsBound())
		{
			Grid->OnCursorChanged.Broadcast(EBattleCursorType::Default, nullptr, nullptr, nullptr);
		}

		NewBattleResultUI->OnBattleResultClosed.AddDynamic(this, &ABattleController::OnBattleResultClosedHandler);

		GetWorld()->GetTimerManager().ClearTimer(ShowBattleTimerHandle);
	}), 3.0f, false);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnNewTurnAnimationEndedHandler()
{
	UW_BattleUI->OnNewTurnAnimationEnded.RemoveDynamic(this, &ABattleController::OnNewTurnAnimationEndedHandler);
	OnCharacterTurnStarted();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnCharacterTurnStarted()
{
	DamageDealt.Empty();
	TotalDamage = 0;

	GetCharactersBasedOnMovementRange(ReadyToPlayCharacters, WaitingCharacters);
	PlayingCharacter = GetNextReadyCharacter();

	if (!IsValid(PlayingCharacter))
	{
		PlayingCharacter = GetNextWaitingCharacter();

		if (!IsValid(PlayingCharacter))
		{
			SetReadyCharacters();
			return;
		}
	}

	UW_BattleUI->UpdateBattleQueue();

	FTimerHandle TurnStartedTimerHandle; // 타이머의 핸들을 정의하십시오.

	GetWorld()->GetTimerManager().SetTimer(TurnStartedTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		PlayingCharacter->SetDefending(false);

		if (!PlayingCharacter->bIsWaiting)
		{
			OnAllDOTsApplied.AddDynamic(this, &ABattleController::OnAllDOTsAppliedHandler);
			ApplyDOTs();
		}
		else
		{
			OnTurnStartAbilitiesUsed();
		}


		GetWorld()->GetTimerManager().ClearTimer(TurnStartedTimerHandle);
	}), 0.5f, false);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnCharacterTurnEnded()
{
	DisableCurrentActiveAbility();
	PlayingCharacter->SetWaiting(false);
	bIsSecondAttack = false;
	UW_BattleUI->HideActionButtons();
	UW_BattleUI->HideActiveAbilityList();

	if (IsValid(PlayingCharacter))
	{
		if (!PlayingCharacter->IsDead())
		{
			OnAllPassiveAbilitiesUsed.RemoveDynamic(this, &ABattleController::OnTurnEndAbilitiesUsed);
			UsePassiveAbilities(PlayingCharacter, TargetCharacters, EPassiveAbilityUseMoment::OnTurnEnd);
		}
		else
		{
			OnTurnEndAbilitiesUsed();
		}
	}
	else
	{
		OnTurnEndAbilitiesUsed();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnTurnEndAbilitiesUsed()
{
	ClearPassiveAbilityCallback();

	FTimerHandle TurnEndTimerHandle; // 타이머의 핸들을 정의하십시오.
	GetWorld()->GetTimerManager().SetTimer(TurnEndTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if (BattleState == EBattleState::PlayerIsPlaying || BattleState == EBattleState::EnemyIsPlaying)
		{
			PlayedCharacters.Add(PlayingCharacter);
			OnCharacterTurnStarted();
		}
		else if (BattleState == EBattleState::Victory || BattleState == EBattleState::Defeat)
		{
			ShowBattleResult();
		}

		// 코드가 실행되면 타이머가 지워집니다.
		GetWorld()->GetTimerManager().ClearTimer(TurnEndTimerHandle);
	}), 0.5f, false);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnBattleResultClosedHandler(EBattleState InResult)
{
	if (InResult == EBattleState::Victory)
	{
		BattleHero->AddDrops(SRPGGameInstance->Gold, SRPGGameInstance->Drops);
		SRPGGameInstance->ChangeMapAfterBattle(BattleHero, NewBattleResultUI->ExpReward);
	}
	else if (InResult == EBattleState::Defeat)
	{
		UGameplayStatics::OpenLevel(this, MainMenuLevelName);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::ShowHeroDetails()
{
	UW_BattleUI->ShowHeroDetails(BattleHero);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnDefendAction()
{
	PlayingCharacter->SetDefending(true);
	if (PlayingCharacter->bIsAI)
	{
		ChangeBattleState(EBattleState::EnemyIsPlaying);
	}
	else
	{
		PlayerCancelActiveAbility();
		CancelSpell();
		ChangeBattleState(EBattleState::PlayerIsPlaying);
		Grid->ResetSlots();
	}

	OnCharacterTurnEnded();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnWaitAction()
{
	PlayingCharacter->SetWaiting(true);
	if (PlayingCharacter->bIsAI)
	{
		ChangeBattleState(EBattleState::EnemyIsPlaying);
	}
	else
	{
		PlayerCancelActiveAbility();
		CancelSpell();
		ChangeBattleState(EBattleState::PlayerIsPlaying);
		Grid->ResetSlots();
		UW_BattleUI->HideActionButtons();
		UW_BattleUI->HideActiveAbilityList();
	}
	OnCharacterTurnStarted();
}

void ABattleController::OnEnemyCharacterDead(ACharacterBase* InCharacter)
{
	// 그리드와 적 목록에서 죽은 캐릭터를 제거합니다.
	Grid->RemoveUnitOnSlot(InCharacter);
	EnemyCharacters.Remove(InCharacter);

	// 죽은 적 캐릭터 목록에 캐릭터를 추가합니다.
	DeadEnemyCharacters.Add(FPlayerCharacterData(InCharacter->GetClass(), InCharacter->StartingStack));

	// 남은 적이 없는지 확인하고 있다면 전투를 종료하고 돌아갑니다.
	if (EnemyCharacters.Num() == 0)
	{
		EndBattle(EBattleState::Victory);
		return;
	}

	// 캐릭터가 보스라면 남은 모든 적의 스택과 체력을 0으로 설정합니다.
	if (InCharacter->bIsBoss)
	{
		for (ACharacterBase* Enemy : EnemyCharacters)
		{
			if (IsValid(Enemy))
			{
				Enemy->Stack = 0;
				Enemy->SetHealth(0, false);
			}
		}
	}

	// 죽은 캐릭터의 OnDead 이벤트에서 현재 메서드를 제거합니다.
	InCharacter->OnDead.RemoveDynamic(this, &ABattleController::OnEnemyCharacterDead);
}

/*ToDo: 기능 구현 확인되지 않음*/
ACharacterBase* ABattleController::GetNextReadyCharacter()
{
	for (ACharacterBase* ReadyToPlayCharacter : ReadyToPlayCharacters)
	{
		if (!IsValid(ReadyToPlayCharacter))
		{
			continue;
		}

		if (!ReadyToPlayCharacter->IsDead() &&
			!PlayedCharacters.Contains(ReadyToPlayCharacter) &&
			!WaitingCharacters.Contains(ReadyToPlayCharacter))
		{
			return ReadyToPlayCharacter;
		}
	}

	return nullptr;
}

/*ToDo: 기능 구현 확인되지 않음*/
ACharacterBase* ABattleController::GetNextWaitingCharacter()
{
	for (ACharacterBase* WaitingCharacter : WaitingCharacters)
	{
		if (!IsValid(WaitingCharacter))
		{
			continue;
		}

		if (!WaitingCharacter->IsDead())
		{
			return WaitingCharacter;
		}
	}
	return nullptr;
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::GetCharactersBasedOnMovementRange(TArray<ACharacterBase*>& OutReadyToPlayCharacters,
                                                          TArray<ACharacterBase*>& OutWaitingCharacters)
{
	TArray<ACharacterBase*> AllCharacters;
	TArray<ACharacterBase*> CurrentReadyToPlayCharacters;
	TArray<ACharacterBase*> CurrentWaitingCharacters;

	AllCharacters.Append(EnemyCharacters);
	AllCharacters.Append(PlayerCharacters);

	while (AllCharacters.Num() > 0)
	{
		int32 CurrentMovementRange = -1;
		ACharacterBase* CurrentCharacter = nullptr;

		for (ACharacterBase* InCharacter : AllCharacters)
		{
			if (InCharacter->CurrentMovementRange >= CurrentMovementRange)
			{
				CurrentMovementRange = InCharacter->CurrentMovementRange;
				CurrentCharacter = InCharacter;
			}
		}

		if (CurrentCharacter)
		{
			CurrentReadyToPlayCharacters.Add(CurrentCharacter);
			if (CurrentCharacter->bIsWaiting)
			{
				CurrentWaitingCharacters.Insert(CurrentCharacter, 0);
			}

			AllCharacters.Remove(CurrentCharacter);
		}
	}

	OutReadyToPlayCharacters = CurrentReadyToPlayCharacters;
	OutWaitingCharacters = CurrentWaitingCharacters;
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::GetCasualties(TMap<TSubclassOf<ACharacterBase>, int32>& PlayerCasualties,
                                      TMap<TSubclassOf<ACharacterBase>, int32>& EnemyCasualties, int32& TotalExpReward)
{
	TMap<TSubclassOf<ACharacterBase>, int32> CurrentPlayerCasualties;
	TMap<TSubclassOf<ACharacterBase>, int32> CurrentEnemyCasualties;
	int32 CurrentExpReward = 0;

	for (TTuple<TSubclassOf<ACharacterBase>, int32> InPlayerStarterCharacter : PlayerStarterCharacters)
	{
		int32 CurrentStack = 0;
		for (ACharacterBase* InPlayerCharacter : PlayerCharacters)
		{
			if (InPlayerCharacter->GetClass() == InPlayerStarterCharacter.Key)
			{
				CurrentStack = InPlayerCharacter->Stack + CurrentStack;
			}
		}

		int32 StackValue = InPlayerStarterCharacter.Value - CurrentStack;
		if (StackValue != 0)
		{
			CurrentPlayerCasualties.Add(InPlayerStarterCharacter.Key, StackValue);
		}
	}

	for (TTuple<TSubclassOf<ACharacterBase>, int32> InEnemyStarterCharacter : EnemyStarterCharacters)
	{
		int32 CurrentStack = 0;
		for (ACharacterBase* InEnemyCharacter : EnemyCharacters)
		{
			if (InEnemyCharacter->GetClass() == InEnemyStarterCharacter.Key)
			{
				CurrentStack = InEnemyCharacter->Stack + CurrentStack;
			}
		}

		int32 StackValue = InEnemyStarterCharacter.Value - CurrentStack;
		if (StackValue != 0)
		{
			CurrentEnemyCasualties.Add(InEnemyStarterCharacter.Key, StackValue);

			ACharacterBase* EnemyStartCharacterCDO = InEnemyStarterCharacter.Key->GetDefaultObject<ACharacterBase>();
			CurrentExpReward = (StackValue * EnemyStartCharacterCDO->ExpReward) + CurrentExpReward;
		}
	}

	PlayerCasualties = CurrentPlayerCasualties;
	EnemyCasualties = CurrentEnemyCasualties;
	TotalExpReward = CurrentExpReward;
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::PlayerCancelActiveAbility()
{
	if (BattleState == EBattleState::WaitingForPlayerAction && IsValid(CurrentActiveAbility))
	{
		DisableCurrentActiveAbility();
		Grid->CheckSlotUnderMouse();
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::DisableCurrentActiveAbility()
{
	if (IsValid(CurrentActiveAbility))
	{
		CurrentActiveAbility->ActivateActiveAbility(false);
		CurrentActiveAbility->ClearActiveAbilityUseCallbacks();
		CurrentActiveAbility = nullptr;
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::AutoPlay()
{
	bIsAutoPlay = !bIsAutoPlay;

	if (bIsAutoPlay && BattleState == EBattleState::WaitingForPlayerAction)
	{
		Grid->ResetSlots();
		PlayerCancelActiveAbility();
		CancelSpell();
		ChangeBattleState(EBattleState::PlayerIsPlaying);
		AILogic->RunAILogic(PlayingCharacter);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::ShowSpells()
{
	if (BattleHero->bCanUseSpell)
	{
		SRPG_CHECK(WBP_SpellBookUIClass);
		UW_SpellBookUI = CreateWidget<UUW_SpellBookUI>(GetWorld(), WBP_SpellBookUIClass);
		UW_SpellBookUI->BattleHero = BattleHero;
		UW_SpellBookUI->AddToViewport();

		UW_SpellBookUI->OnSpellBookClosed.AddDynamic(this, &ABattleController::OnSpellBookClosedHandler);
		UW_SpellBookUI->OnSpellClicked.AddDynamic(this, &ABattleController::OnSpellClickedHandler);

		UW_BattleUI->UW_ControlPanel->UW_SpellBookButton->ActivateButton(true);
		PlayerCancelActiveAbility();
	}
	else
	{
		UW_BattleUI->ShowMessage(FText::FromString(TEXT("이번 라운드에 주문을 사용할 수 없습니다.")), true);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnSpellBookClosedHandler()
{
	UW_SpellBookUI->RemoveFromParent();
	UW_BattleUI->UW_ControlPanel->UW_SpellBookButton->ActivateButton(false);
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnSpellClickedHandler(ASpellBase* InSpell)
{
	OnSpellClicked(InSpell);
	UW_SpellBookUI->RemoveFromParent();
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnActiveAbilityClicked(AActiveAbilityBase* InActiveAbility)
{
	DisableCurrentActiveAbility();
	CancelSpell();
	CurrentActiveAbility = InActiveAbility;

	CurrentActiveAbility->OnActiveAbilityUseStarted.AddDynamic(
		this, &ABattleController::OnActiveAbilityUseStartedHandler);
	CurrentActiveAbility->OnActiveAbilityUseEnded.AddDynamic(this, &ABattleController::OnActiveAbilityUseEndedHandler);

	if (PlayingCharacter->bIsAI || bIsAutoPlay)
	{
		CurrentActiveAbility->OnActiveAbilityEnabled();
	}
	else
	{
		CurrentActiveAbility->ActivateActiveAbility(true);
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnActiveAbilityUseStartedHandler()
{
	if (PlayingCharacter->bIsAI)
	{
		ChangeBattleState(EBattleState::EnemyIsPlaying);
	}
	else
	{
		ChangeBattleState(EBattleState::PlayerIsPlaying);
		if (!bIsAutoPlay)
		{
			Grid->ResetSlots();
			UW_BattleUI->HideActionButtons();
			UW_BattleUI->HideActiveAbilityList();
			CurrentActiveAbility->ActivateActiveAbility(false);
		}
	}
}

/*ToDo: 기능 구현 확인되지 않음*/
void ABattleController::OnActiveAbilityUseEndedHandler()
{
	DisableCurrentActiveAbility();
	OnCharacterTurnEnded();
}
