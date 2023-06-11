#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "RPGSaveGame.generated.h"

class AQuestBase;
class AExploreHeroBase;
class AEquipmentBase;

/**
 * 
 */
UCLASS()
class SRG_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	URPGSaveGame(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="SRG Game Instance")
	void SaveHero(int32 NewExp, int32 NewGold, int32 NewCurrentMana, const TArray<FPlayerCharacterData>& NewPlayerCharacterList,
	              TSubclassOf<AExploreHeroBase> NewExploreHero, const TMap<TSubclassOf<AQuestBase>, FQuestStepData>& NewActiveQuestData,
	              const TMap<TSubclassOf<AQuestBase>, int32>& NewCompletedQuestData,
	              const TArray<TSubclassOf<AEquipmentBase>>& NewBackPack, const TArray<TSubclassOf<AEquipmentBase>>& NewEquipment);

	UFUNCTION(BlueprintCallable, Category="SRG Game Instance")
	void LoadHero(int32& OutExp, int32& OutGold, int32& OutCurrentMana, TArray<FPlayerCharacterData>& OutPlayerCharacterList,
	              TSubclassOf<AExploreHeroBase>& OutExploreHero, TMap<TSubclassOf<AQuestBase>, FQuestStepData>& OutActiveQuestData,
	              TMap<TSubclassOf<AQuestBase>, int32>& OutCompletedQuestData,
	              TArray<TSubclassOf<AEquipmentBase>>& OutBackPack, TArray<TSubclassOf<AEquipmentBase>>& OutEquipment);

#pragma region   	Field Members

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	FName CurrentMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	int32 Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	int32 CurrentMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	FDateTime SaveDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	FTransform HeroTransForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TSubclassOf<AExploreHeroBase> ExploreHero;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TArray<FPlayerCharacterData> PlayerCharacterList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TArray<FString> RemovedBattles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TArray<FString> Interactions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TArray<FString> ActiveQuests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TArray<TSubclassOf<AEquipmentBase>> BackPack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TMap<FString, FShopRecruitmentList> RecruitShop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TMap<FString, FShopEquipmentList> EquipmentShop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TMap<TSubclassOf<AQuestBase>, FQuestStepData> ActiveQuestData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RPG Save")
	TMap<TSubclassOf<AQuestBase>, int32> CompletedQuestData;

#pragma endregion	Field Members
};
