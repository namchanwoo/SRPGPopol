#include "SRGGameInstance.h"

#include "BattleController.h"
#include "DeploymentController.h"
#include "RPGSaveGame.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	BattleQuest = FString();
}

void USRGGameInstance::AddInteractions(AInteractionDetector* InteractableBase)
{
	if (!IsValid(RpgSaveGame))
	{
		OnNewGameStarted();
	}

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	CurrentLevelName.Append(TEXT("_"));
	CurrentLevelName.Append(InteractableBase->GetName());
	RpgSaveGame->Interactions.Add(CurrentLevelName);
}

void USRGGameInstance::AddQuest(AQuestBase* InQuest)
{
	if (!IsValid(RpgSaveGame))
	{
		OnNewGameStarted();
	}

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	CurrentLevelName.Append(TEXT("_"));
	CurrentLevelName.Append(InQuest->GetName());
	RpgSaveGame->ActiveQuests.Add(CurrentLevelName);
}

void USRGGameInstance::LoadInteractables()
{
	TArray<AActor*> InteractionDetectorActors;
	UGameplayStatics::GetAllActorsOfClass(this, AInteractionDetector::StaticClass(), InteractionDetectorActors);

	for (AActor* InteractionDetectorActor : InteractionDetectorActors)
	{
		AInteractionDetector* InteractionDetector = Cast<AInteractionDetector>(InteractionDetectorActor);

		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
		CurrentLevelName.Append(TEXT("_"));
		CurrentLevelName.Append(InteractionDetector->GetName());

		if (RpgSaveGame->Interactions.Contains(CurrentLevelName))
		{
			InteractionDetector->Load(bShouldLoadEnemyPawns);
		}
	}
}

void USRGGameInstance::LoadHero()
{
	int32 LoadExp;
	int32 LoadGold;
	int32 LoadCurrentMana;
	TArray<FPlayerCharacterData> LoadPlayerCharacterList;
	TSubclassOf<AExploreHeroBase> LoadExploreHero;
	TMap<TSubclassOf<AQuestBase>, FQuestStepData> LoadActiveQuestData;
	TMap<TSubclassOf<AQuestBase>, int32> LoadCompletedQuestData;
	TArray<TSubclassOf<AEquipmentBase>> LoadBackPack;
	TArray<TSubclassOf<AEquipmentBase>> LoadEquipment;
	RpgSaveGame->LoadHero(LoadExp, LoadGold, LoadCurrentMana, LoadPlayerCharacterList, LoadExploreHero, LoadActiveQuestData,
	                      LoadCompletedQuestData, LoadBackPack, LoadEquipment);

	AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	SRPG_CHECK(ExploreHeroBase);

	ExploreHeroBase->Exp = LoadExp;
	ExploreHeroBase->Gold = LoadGold;
	ExploreHeroBase->CurrentMana = LoadCurrentMana;
	ExploreHeroBase->PlayerCharacterList = LoadPlayerCharacterList;
	ExploreHeroBase->ActiveQuestData = LoadActiveQuestData;
	ExploreHeroBase->CompletedQuestData = LoadCompletedQuestData;
	ExploreHeroBase->BackPack = LoadBackPack;
	ExploreHeroBase->Equipment = LoadEquipment;
	ExploreHeroBase->SetCharacters();
}

void USRGGameInstance::LoadQuests()
{
	TArray<AActor*> QuestBaseActors;
	UGameplayStatics::GetAllActorsOfClass(this, AQuestBase::StaticClass(), QuestBaseActors);

	TArray<AQuestBase*> QuestBaseArray;
	for (int32 i = 0; i < QuestBaseActors.Num(); i++)
	{
		AQuestBase* CastQuestBase = Cast<AQuestBase>(QuestBaseActors[i]);
		if (CastQuestBase)
		{
			QuestBaseArray.Add(CastQuestBase);
		}
	}

	UpdateBattleQuest(QuestBaseArray);

	for (int32 i = 0; i < QuestBaseArray.Num(); i++)
	{
		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
		CurrentLevelName.Append(FString("_"));
		CurrentLevelName.Append(QuestBaseArray[i]->GetName());

		if (RpgSaveGame->ActiveQuests.Contains(CurrentLevelName))
		{
			AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
			SRPG_CHECK(ExploreHeroBase);
			ExploreHeroBase->LoadQuests(QuestBaseArray[i]);
		}
		QuestBaseArray[i]->Load();
	}
}

void USRGGameInstance::RemoveQuest(AQuestBase* InQuest)
{
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	FString CurrentQuestName = CurrentLevelName.Append(FString("_"));
	CurrentQuestName = CurrentQuestName.Append(InQuest->GetName());
	RpgSaveGame->ActiveQuests.Remove(CurrentQuestName);
}

void USRGGameInstance::SaveBattleHero(ABattleHeroBase* InBattleHero)
{
	if (!IsValid(RpgSaveGame))
	{
		OnNewGameStarted();
	}


	RpgSaveGame->SaveHero(RpgSaveGame->Exp, InBattleHero->Gold, InBattleHero->CurrentMana, GetSurvivingPlayerCharacters(InBattleHero),
	                      RpgSaveGame->ExploreHero, RpgSaveGame->ActiveQuestData, RpgSaveGame->CompletedQuestData,
	                      InBattleHero->BackPack, InBattleHero->Equipment);
}

void USRGGameInstance::SaveBattleQuest(ABattleQuestBase* InBattleQuest)
{
	if (IsValid(InBattleQuest))
	{
		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
		CurrentLevelName.Append(TEXT("_"));
		CurrentLevelName.Append(InBattleQuest->GetName());

		BattleQuest = CurrentLevelName;
	}
}

void USRGGameInstance::SaveExploreHero()
{
	if (!IsValid(RpgSaveGame))
	{
		OnNewGameStarted();
	}

	AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	SRPG_CHECK(ExploreHeroBase);
	RpgSaveGame->SaveHero(ExploreHeroBase->Exp, ExploreHeroBase->Gold, ExploreHeroBase->CurrentMana, ExploreHeroBase->PlayerCharacterList,
	                      ExploreHeroBase->GetClass(), ExploreHeroBase->ActiveQuestData, ExploreHeroBase->CompletedQuestData,
	                      ExploreHeroBase->BackPack, ExploreHeroBase->Equipment);
}

void USRGGameInstance::ChangeMapForBattle(TArray<FEnemyCharacterData> InEnemyList, TMap<TSubclassOf<AObstacleBase>, int32> InObstacleList,
                                          int32 InGold, ABattleQuestBase* InBattleQuest, TArray<TSubclassOf<AEquipmentBase>> InDrops,
                                          const FName& InMapName)
{
	EnemyCharacterList = InEnemyList;
	ObstacleList = InObstacleList;
	Gold = InGold;
	Drops = InDrops;
	bShouldLoadGame = false;
	bShouldLoadTransform = false;

	SaveBattleQuest(InBattleQuest);
	SaveExploreHero();

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	RpgSaveGame->CurrentMap = FName(CurrentLevelName);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(PlayerPawn);
	SRPG_CHECK(ExploreHeroBase);
	RpgSaveGame->HeroTransForm = ExploreHeroBase->GetActorTransform();


	UGameplayStatics::OpenLevel(this, InMapName);
}

void USRGGameInstance::ChangeMapAfterBattle(ABattleHeroBase* InBattleHero, int32 InExpReward)
{
	SaveBattleHero(InBattleHero);
	ExpFromBattle = InExpReward;
	bShouldLoadGame = true;
	bShouldLoadTransform = true;
	bShouldLoadEnemyPawns = false;
	bShouldAddExp = true;
	UGameplayStatics::OpenLevel(this, RpgSaveGame->CurrentMap);
}

void USRGGameInstance::ChangeMap(FName InMapName)
{
	SaveExploreHero();

	bShouldLoadGame = true;
	bShouldLoadTransform = false;
	bShouldLoadEnemyPawns = true;
	bShouldAddExp = false;
	BattleQuest = FString("");
	UGameplayStatics::OpenLevel(this, InMapName);
}

bool USRGGameInstance::GetShouldLoadGame()
{
	return bShouldLoadGame;
}

TArray<FPlayerCharacterData> USRGGameInstance::GetSurvivingPlayerCharacters(ABattleHeroBase* InBattleHero)
{
	TArray<FPlayerCharacterData> CurrentPlayerCharacters;
	for (ACharacterBase* PlayerCharacter : InBattleHero->BattleController->PlayerCharacters)
	{
		FPlayerCharacterData NewPlayerCharacterData = FPlayerCharacterData(PlayerCharacter->GetClass(), PlayerCharacter->Stack);
		CurrentPlayerCharacters.Add(NewPlayerCharacterData);
	}

	CurrentPlayerCharacters.Append(InBattleHero->BattleController->DeploymentController->NonDeployedCharacters);

	return CurrentPlayerCharacters;
}

void USRGGameInstance::SaveGame(FString InSlotName)
{
	if (!IsValid(RpgSaveGame))
	{
		OnNewGameStarted();
	}

	SaveExploreHero();

	AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	SRPG_CHECK(ExploreHeroBase);
	RpgSaveGame->HeroTransForm = ExploreHeroBase->GetActorTransform();
	RpgSaveGame->SaveDate = FDateTime::Now();

	RpgSaveGame->CurrentMap = FName(UGameplayStatics::GetCurrentLevelName(this));
	UGameplayStatics::SaveGameToSlot(RpgSaveGame, InSlotName, 0);
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

void USRGGameInstance::LoadHeroTransform()
{
	if (bShouldLoadTransform)
	{
		AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
		SRPG_CHECK(ExploreHeroBase);

		ExploreHeroBase->SpringArm->bEnableCameraLag = false;
		ExploreHeroBase->SetActorLocationAndRotation(RpgSaveGame->HeroTransForm.GetLocation(),
		                                             RpgSaveGame->HeroTransForm.Rotator());

		FTimerHandle WaitTimerHandle;
		constexpr float WaitTime = 0.5f;
		GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			ExploreHeroBase->SpringArm->bEnableCameraLag = true;
			GetWorld()->GetTimerManager().ClearTimer(WaitTimerHandle);
		}), WaitTime, false); // 반복하려면 false를 true로 변경
	}
}

void USRGGameInstance::LoadGame()
{
	if (!IsValid(RpgSaveGame))
	{
		return;
	}

	LoadHero();
	LoadInteractables();
	LoadQuests();
	LoadShops();
	LoadHeroTransform();
	AddExp();

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

void USRGGameInstance::LoadShops()
{
	TArray<AActor*> ShopBaseActors;
	UGameplayStatics::GetAllActorsOfClass(this, AShopBase::StaticClass(), ShopBaseActors);

	for (AActor* ShopBaseActor : ShopBaseActors)
	{
		AShopBase* ShopBase = Cast<AShopBase>(ShopBaseActor);
		SRPG_CHECK(ShopBase);

		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
		FString CurrentShopName = CurrentLevelName.Append(FString("_"));
		CurrentShopName = CurrentShopName.Append(ShopBaseActor->GetName());

		if (RpgSaveGame->RecruitShop.Contains(CurrentShopName))
		{
			ShopBase->LoadRecruitShop(RpgSaveGame->RecruitShop.Find(CurrentShopName)->RecruitmentList);
		}

		if (RpgSaveGame->EquipmentShop.Contains(CurrentShopName))
		{
			ShopBase->LoadEquipmentShip(RpgSaveGame->EquipmentShop.Find(CurrentShopName)->Equipment);
		}
	}
}

void USRGGameInstance::UpdateBattleQuest(TArray<AQuestBase*> AllQuests)
{
	if (!BattleQuest.IsEmpty())
	{
		for (AQuestBase* Quest : AllQuests)
		{
			FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
			CurrentLevelName.Append(FString("_"));
			CurrentLevelName.Append(Quest->GetName());

			if (CurrentLevelName == BattleQuest)
			{
				BattleQuest = FString();
				AExploreHeroBase* ExploreHeroBase = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
				SRPG_CHECK(ExploreHeroBase);
				ExploreHeroBase->UpdateQuest(Quest, 1, false, false);
			}
		}
	}
}

void USRGGameInstance::AddRecruitShop(AShopBase* InShop, FShopRecruitmentList InValue)
{
	if (!IsValid(RpgSaveGame))
	{
		OnNewGameStarted();
	}

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	FString ShopName = CurrentLevelName.Append(FString("_"));
	ShopName = ShopName.Append(InShop->GetName());

	RpgSaveGame->RecruitShop.Add(ShopName, InValue);
}

void USRGGameInstance::AddEquipmentShop(AShopBase* InShop, FShopEquipmentList InEquipment)
{
	if (!IsValid(RpgSaveGame))
	{
		OnNewGameStarted();
	}

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	FString ShopName = CurrentLevelName.Append(FString("_"));
	ShopName = ShopName.Append(InShop->GetName());

	RpgSaveGame->EquipmentShop.Add(ShopName, InEquipment);
}
