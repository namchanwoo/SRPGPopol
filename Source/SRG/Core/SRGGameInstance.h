#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SRGCore/SRGEnumStruct.h"
#include "SRGGameInstance.generated.h"

class AShopBase;
class AQuestBase;
class ABattleHeroBase;
class URPGSaveGame;
class AObstacleBase;
class ABattleQuestBase;
class AInteractionDetector;
class AEquipmentBase;


/**
 * 
 */
UCLASS()
class SRG_API USRGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USRGGameInstance(const FObjectInitializer& ObjectInitializer);

#pragma region   	Game Instance Event

public:
	void OnNewGameStarted();

	void AddInteractions(AInteractionDetector* InteractableBase);

	void AddQuest(AQuestBase* InQuest);

	void LoadInteractables();

	void LoadHero();

	void LoadQuests();

	void RemoveQuest(AQuestBase* InQuest);

	void SaveBattleHero(ABattleHeroBase* InBattleHero);

	void SaveBattleQuest(ABattleQuestBase* InBattleQuest);

	void SaveExploreHero();

	void ChangeMapForBattle(TArray<FEnemyCharacterData> InEnemyList, TMap<TSubclassOf<AObstacleBase>, int32> InObstacleList, int32 InGold,
	                        ABattleQuestBase* InBattleQuest, TArray<TSubclassOf<AEquipmentBase>> InDrops, const FName& InMapName);


	void ChangeMapAfterBattle(ABattleHeroBase* InBattleHero, int32 InExpReward);

	void ChangeMap(FName InMapName);


	bool GetShouldLoadGame();

	TArray<FPlayerCharacterData> GetSurvivingPlayerCharacters(ABattleHeroBase* InBattleHero);

	void SaveGame(FString InSlotName);

	void AddExp();

	void LoadHeroTransform();

	void LoadGame();

	void LoadThisGame(URPGSaveGame* InRPGSaveGame);

	void LoadShops();

	void UpdateBattleQuest(TArray<AQuestBase*> AllQuests);

	void AddRecruitShop(AShopBase* InShop, FShopRecruitmentList InValue);
	
	void AddEquipmentShop(AShopBase* InShop, FShopEquipmentList InEquipment);

#pragma endregion	Game Instance Event


#pragma region   	Field Members

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	bool bShouldLoadGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	bool bShouldLoadTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	bool bShouldLoadEnemyPawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	bool bShouldAddExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	int32 ExpFromBattle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	FString SlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	FString BattleQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	URPGSaveGame* RpgSaveGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	TArray<FEnemyCharacterData> EnemyCharacterList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	TArray<TSubclassOf<AEquipmentBase>> Drops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	TMap<TSubclassOf<AObstacleBase>, int32> ObstacleList;

#pragma endregion	Field Members
};
