#include "SRGGameInstance.h"
#include "BattleController.h"
#include "DeploymentController.h"
#include "RPGSaveGame.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "SRG/Heros/BattleHeroBase.h"
#include "SRG/Interactables/InteractionDetector.h"
#include "SRG/Interactables/Shop/ShopBase.h"
#include "SRG/Quests/BattleQuests/BattleQuestBase.h"
#include "SRG/Units/CharacterBase.h"
#include "SRGCore/SRGLog.h"

USRGGameInstance::USRGGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USRGGameInstance::OnNewGameStarted()
{
	RpgSaveGame = Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass()));
	bShouldLoadGame = false;
	bShouldLoadTransform = false;
	bShouldLoadEnemyPawns = true;
	bShouldAddExp = false;
	BattleQuest.Empty();
}

bool USRGGameInstance::GetShouldLoadGame()
{
	return bShouldLoadGame;
}

void USRGGameInstance::LoadGame()
{
	// 저장된 게임이 유효한지 확인
	if (!IsValid(RpgSaveGame))
	{
		// 저장된 게임이 유효하지 않은 경우 로드하지 않고 리턴
		return;
	}
	// 영웅 로드
	LoadHero();

	// 상호작용 가능 항목 로드
	LoadInteractables();

	// 퀘스트 로드
	LoadQuests();

	// 상점 로드
	LoadShops();

	// 영웅의 변환 로드
	LoadHeroTransform();

	// 경험치 추가
	AddExp();

	// 로드 플래그 설정
	bShouldLoadGame = false;
	bShouldLoadTransform = false;
	bShouldLoadEnemyPawns = true;
	bShouldAddExp = false;
}

void USRGGameInstance::LoadThisGame(URPGSaveGame* InRPGSaveGame)
{
	RpgSaveGame = InRPGSaveGame;

	bShouldLoadGame = true;

	bShouldLoadTransform = true;

	bShouldLoadEnemyPawns = true;

	bShouldAddExp = false;

	BattleQuest = FString();

	UGameplayStatics::OpenLevel(this, RpgSaveGame->CurrentMap);
}

void USRGGameInstance::SaveGame(FString InSlotName)
{
	SlotName = InSlotName;
	
	EnsureSaveGameInitialized();

	SaveExploreHero();

	const AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	SRPG_CHECK(ExploreHeroBase);
	
	RpgSaveGame->HeroTransForm = ExploreHeroBase->GetActorTransform();
	RpgSaveGame->SaveDate = FDateTime::Now();

	RpgSaveGame->CurrentMap = FName(UGameplayStatics::GetCurrentLevelName(this));
	UGameplayStatics::SaveGameToSlot(RpgSaveGame, InSlotName, 0);
}

void USRGGameInstance::LoadInteractables()
{
	TArray<AActor*> InteractionDetectorActors;
	UGameplayStatics::GetAllActorsOfClass(this, AInteractionDetector::StaticClass(), InteractionDetectorActors);

	for (AActor* InteractionDetectorActor : InteractionDetectorActors)
	{
		AInteractionDetector* InteractionDetector = Cast<AInteractionDetector>(InteractionDetectorActor);
		FString CurrentInteractableName = GenerateUniqueIdentifier(InteractionDetector);
		if (RpgSaveGame->Interactions.Contains(CurrentInteractableName))
		{
			InteractionDetector->Load(bShouldLoadEnemyPawns);
		}
	}
}

void USRGGameInstance::LoadHero()
{
	AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	SRPG_CHECK(ExploreHeroBase);
	
	TSubclassOf<AExploreHeroBase> LoadExploreHero;
	
	RpgSaveGame->LoadHero(ExploreHeroBase->Exp, ExploreHeroBase->Gold, ExploreHeroBase->CurrentMana,
	                      ExploreHeroBase->PlayerCharacterList,
	                      LoadExploreHero,
	                      ExploreHeroBase->ActiveQuestData,
	                      ExploreHeroBase->CompletedQuestData,
	                      ExploreHeroBase->BackPack,
	                      ExploreHeroBase->Equipment);

	ExploreHeroBase->SetCharacters();
}

void USRGGameInstance::LoadQuest(AQuestBase* Quest)
{
	const FString CurrentLevelName = GenerateUniqueIdentifier(Quest);

	if (RpgSaveGame->ActiveQuests.Contains(CurrentLevelName))
	{
		AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
		SRPG_CHECK(ExploreHeroBase);
		ExploreHeroBase->LoadQuests(Quest);
	}
}

void USRGGameInstance::LoadQuests()
{
	TArray<AActor*> QuestBaseActors;
	UGameplayStatics::GetAllActorsOfClass(this, AQuestBase::StaticClass(), QuestBaseActors);

	TArray<AQuestBase*> QuestBaseArray;
	for (AActor* QuestBaseActor : QuestBaseActors)
	{
		if (AQuestBase* CastQuestBase = Cast<AQuestBase>(QuestBaseActor))
		{
			QuestBaseArray.Add(CastQuestBase);
		}
	}

	UpdateBattleQuest(QuestBaseArray);

	for (AQuestBase* Quest : QuestBaseArray)
	{
		LoadQuest(Quest);
		Quest->Load();
	}
}

void USRGGameInstance::LoadShops()
{
	// 레벨의 모든 ShopBase 액터 가져오기
	TArray<AActor*> ShopBaseActors;
	UGameplayStatics::GetAllActorsOfClass(this, AShopBase::StaticClass(), ShopBaseActors);

	// 각 ShopBase 액터에 대해 반복
	for (AActor* ShopBaseActor : ShopBaseActors)
	{
		// 액터를 AShopBase에 캐스팅
		AShopBase* ShopBase = Cast<AShopBase>(ShopBaseActor);

		// 캐스트가 성공했는지 확인
		SRPG_CHECK(ShopBase);

		// 현재 레벨과 액터 이름을 기반으로 고유한 상점 이름 구성
		FString CurrentShopName = GenerateUniqueIdentifier(ShopBaseActor);

		// 가능한 경우 현재 매장에 대한 채용 매장 데이터 로드
		if (RpgSaveGame->RecruitShop.Contains(CurrentShopName))
		{
			ShopBase->LoadRecruitShop(RpgSaveGame->RecruitShop.Find(CurrentShopName)->RecruitmentList);
		}

		// 사용 가능한 경우 현재 매장에 대한 장비 매장 데이터 로드
		if (RpgSaveGame->EquipmentShop.Contains(CurrentShopName))
		{
			ShopBase->LoadEquipmentShip(RpgSaveGame->EquipmentShop.Find(CurrentShopName)->Equipment);
		}
	}
}

void USRGGameInstance::LoadHeroTransform()
{
	if (bShouldLoadTransform)
	{
		AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
		SRPG_CHECK(ExploreHeroBase);

		// 카메라 지연을 비활성화하고 배우의 위치와 회전을 설정합니다.
		ExploreHeroBase->SpringArm->bEnableCameraLag = false;
		ExploreHeroBase->SetActorLocationAndRotation(RpgSaveGame->HeroTransForm.GetLocation(),
		                                             RpgSaveGame->HeroTransForm.Rotator());

		// 짧은 지연 후 카메라 지연 활성화
		constexpr float WaitTime = 0.5f;
		FTimerHandle WaitTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, [ExploreHeroBase]()
		{
			// 역참조하기 전에 포인터가 유효한지 확인하십시오.
			if (IsValid(ExploreHeroBase))
			{
				ExploreHeroBase->SpringArm->bEnableCameraLag = true;
			}
		}, WaitTime, false);
	}
}

void USRGGameInstance::SaveExploreHero()
{
	EnsureSaveGameInitialized();

	const AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	SRPG_CHECK(ExploreHeroBase);

	RpgSaveGame->SaveHero(ExploreHeroBase->Exp, ExploreHeroBase->Gold, ExploreHeroBase->CurrentMana,
	                      ExploreHeroBase->PlayerCharacterList,
	                      ExploreHeroBase->GetClass(), ExploreHeroBase->ActiveQuestData,
	                      ExploreHeroBase->CompletedQuestData,
	                      ExploreHeroBase->BackPack, ExploreHeroBase->Equipment);
}

void USRGGameInstance::SaveBattleHero(ABattleHeroBase* InBattleHero)
{
	EnsureSaveGameInitialized();

	RpgSaveGame->SaveHero(RpgSaveGame->Exp, InBattleHero->Gold, InBattleHero->CurrentMana,
	                      GetSurvivingPlayerCharacters(InBattleHero),
	                      RpgSaveGame->ExploreHero, RpgSaveGame->ActiveQuestData, RpgSaveGame->CompletedQuestData,
	                      InBattleHero->BackPack, InBattleHero->Equipment);
}

TArray<FPlayerCharacterData> USRGGameInstance::GetSurvivingPlayerCharacters(ABattleHeroBase* InBattleHero)
{
	TArray<FPlayerCharacterData> CurrentPlayerCharacters;

	for (ACharacterBase* PlayerCharacter : InBattleHero->BattleController->PlayerCharacters)
	{
		CurrentPlayerCharacters.Add(GeneratePlayerCharacterData(PlayerCharacter));
	}

	CurrentPlayerCharacters.Append(InBattleHero->BattleController->DeploymentController->NonDeployedCharacters);

	return CurrentPlayerCharacters;
}

void USRGGameInstance::AddExp()
{
	if (bShouldAddExp)
	{
		AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
		SRPG_CHECK(ExploreHeroBase);

		ExploreHeroBase->AddExp(ExpFromBattle);
		ExpFromBattle = 0;
	}
}

void USRGGameInstance::AddQuest(AQuestBase* InQuest)
{
	// 저장 게임 인스턴스가 초기화되었는지 확인하십시오.
	EnsureSaveGameInitialized();

	// 퀘스트의 고유 식별자 생성
	const FString CurrentQuestName = GenerateUniqueIdentifier(InQuest);

	// 저장 게임 인스턴스의 활성 퀘스트에 퀘스트를 추가합니다.
	RpgSaveGame->ActiveQuests.Add(CurrentQuestName);
}

void USRGGameInstance::RemoveQuest(AQuestBase* InQuest)
{
	// 퀘스트의 고유 식별자 생성
	const FString CurrentQuestName = GenerateUniqueIdentifier(InQuest);

	// 활성 퀘스트에서 퀘스트 제거
	RpgSaveGame->ActiveQuests.Remove(CurrentQuestName);
}

void USRGGameInstance::UpdateBattleQuest(const TArray<AQuestBase*>& AllQuests)
{
	// BattleQuest가 비어 있지 않은지 확인하십시오.
	if (!BattleQuest.IsEmpty())
	{
		// 현재 레벨 이름 얻기
		const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);

		// 모든 퀘스트 반복
		for (AQuestBase* Quest : AllQuests)
		{
			// 현재 레벨 이름과 퀘스트 이름을 추가하여 퀘스트 레벨 이름 구성
			FString QuestLevelName = CurrentLevelName + "_" + Quest->GetName();

			// 구성된 퀘스트 레벨 이름이 BattleQuest와 일치하는지 확인
			if (QuestLevelName == BattleQuest)
			{
				// BattleQuest 클리어
				BattleQuest.Empty();

				// ExploreHeroBase 폰 가져오기
				AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
				SRPG_CHECK(ExploreHeroBase);

				// 퀘스트 업데이트
				ExploreHeroBase->UpdateQuest(Quest, 1, false, false);
			}
		}
	}
}

void USRGGameInstance::SaveBattleQuest(ABattleQuestBase* InBattleQuest)
{
	if (IsValid(InBattleQuest))
	{
		BattleQuest = GenerateUniqueIdentifier(InBattleQuest);
	}
}

void USRGGameInstance::AddInteractions(AInteractionDetector* InteractableBase)
{
	EnsureSaveGameInitialized();

	const FString InteractableIdentifier = GenerateUniqueIdentifier(InteractableBase);
	RpgSaveGame->Interactions.Add(InteractableIdentifier);
}

void USRGGameInstance::AddRecruitShop(AShopBase* InShop, FShopRecruitmentList InValue)
{
	SRPG_LOG_FUNCTION; //로그 함수 호출

	EnsureSaveGameInitialized();

	const FString ShopName = GenerateUniqueIdentifier(InShop);

	// 게임을 저장하기 위해 상점을 추가
	RpgSaveGame->RecruitShop.Add(ShopName, InValue);

	// 신규 점포 추가 기록
	SRPG_LOG(Log, TEXT("Added shop '%s' to recruitment list."), *ShopName);
}

void USRGGameInstance::AddEquipmentShop(AShopBase* InShop, FShopEquipmentList InEquipment)
{
	SRPG_LOG_FUNCTION; // 로그 함수 호출

	EnsureSaveGameInitialized();

	// 상점의 고유 식별자를 얻습니다.
	const FString ShopName = GenerateUniqueIdentifier(InShop);

	// 게임을 저장하기 위해 상점을 추가
	RpgSaveGame->EquipmentShop.Add(ShopName, InEquipment);

	// 신규 점포 추가 기록
	SRPG_LOG(Log, TEXT("Added '%s' to equipment shop."), *ShopName);
}

void USRGGameInstance::ChangeMap(FName InMapName)
{
	SaveExploreHero();

	bShouldLoadGame = true;
	bShouldLoadEnemyPawns = true;

	bShouldLoadTransform = false;
	bShouldAddExp = false;

	BattleQuest.Empty();

	UGameplayStatics::OpenLevel(this, InMapName);
}

void USRGGameInstance::ChangeMapForBattle(const TArray<FEnemyCharacterData>& InEnemyList,
                                          const TMap<TSubclassOf<AObstacleBase>, int32>& InObstacleList,
                                          int32 InGold, ABattleQuestBase* InBattleQuest,
                                          const TArray<TSubclassOf<AEquipmentBase>>& InDrops,
                                          const FName& InMapName)
{
	// 수신 데이터 저장
	EnemyCharacterList = InEnemyList;
	ObstacleList = InObstacleList;
	Gold = InGold;
	Drops = InDrops;

	// 로드 플래그 재설정
	bShouldLoadGame = false;
	bShouldLoadTransform = false;

	// 전투 퀘스트 및 영웅 상태 저장
	SaveBattleQuest(InBattleQuest);
	SaveExploreHero();

	// 세이브 게임에서 현재 맵 업데이트
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	RpgSaveGame->CurrentMap = FName(CurrentLevelName);

	// 플레이어 폰과 변형 가져오기
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	const AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(PlayerPawn);
	SRPG_CHECK(ExploreHeroBase);
	RpgSaveGame->HeroTransForm = ExploreHeroBase->GetActorTransform();

	// 지정된 지도 열기
	UGameplayStatics::OpenLevel(this, InMapName);
}

void USRGGameInstance::ChangeMapAfterBattle(ABattleHeroBase* InBattleHero, int32 InExpReward)
{
	SaveBattleHero(InBattleHero);

	ExpFromBattle = InExpReward;

	bShouldLoadGame = true;
	bShouldLoadTransform = true;
	bShouldAddExp = true;
	bShouldLoadEnemyPawns = false;

	UGameplayStatics::OpenLevel(this, RpgSaveGame->CurrentMap);
}

void USRGGameInstance::EnsureSaveGameInitialized()
{
	if (!IsValid(RpgSaveGame))
	{
		OnNewGameStarted();
	}
}

FString USRGGameInstance::GenerateUniqueIdentifier(AActor* Actor)
{
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	CurrentLevelName.Append(TEXT("_"));
	CurrentLevelName.Append(Actor->GetName());
	return CurrentLevelName;
}

FPlayerCharacterData USRGGameInstance::GeneratePlayerCharacterData(ACharacterBase* PlayerCharacter)
{
	return FPlayerCharacterData(PlayerCharacter->GetClass(), PlayerCharacter->Stack);
}
