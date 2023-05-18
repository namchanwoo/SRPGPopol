#include "SRG/Core/RPGSaveGame.h"

URPGSaveGame::URPGSaveGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URPGSaveGame::SaveHero(int32 NewExp, int32 NewGold, int32 NewCurrentMana, const TArray<FPlayerCharacterData>& NewPlayerCharacterList,
                            TSubclassOf<AExploreHeroBase> NewExploreHero, const TMap<TSubclassOf<AQuestBase>, FQuestStepData>& NewActiveQuestData,
                            const TMap<TSubclassOf<AQuestBase>, int32>& NewCompletedQuestData, const TArray<TSubclassOf<AEquipmentBase>>& NewBackPack,
                            const TArray<TSubclassOf<AEquipmentBase>>& NewEquipment)
{
	Exp = NewExp;
	Gold = NewGold;
	CurrentMana = NewCurrentMana;
	PlayerCharacterList = NewPlayerCharacterList;
	ExploreHero = NewExploreHero;
	ActiveQuestData = NewActiveQuestData;
	CompletedQuestData = NewCompletedQuestData;
	BackPack = NewBackPack;
	Equipment = NewEquipment;
}

void URPGSaveGame::LoadHero(int32& OutExp, int32& OutGold, int32& OutCurrentMana, TArray<FPlayerCharacterData>& OutPlayerCharacterList,
                            TSubclassOf<AExploreHeroBase>& OutExploreHero, TMap<TSubclassOf<AQuestBase>, FQuestStepData>& OutActiveQuestData,
                            TMap<TSubclassOf<AQuestBase>, int32>& OutCompletedQuestData, TArray<TSubclassOf<AEquipmentBase>>& OutBackPack,
                            TArray<TSubclassOf<AEquipmentBase>>& OutEquipment)
{
	OutExp = Exp;
	OutGold = Gold;
	OutCurrentMana = CurrentMana;
	OutPlayerCharacterList = PlayerCharacterList;
	OutExploreHero = ExploreHero;
	OutActiveQuestData = ActiveQuestData;
	OutCompletedQuestData = CompletedQuestData;
	OutBackPack = BackPack;
	OutEquipment = Equipment;
}
