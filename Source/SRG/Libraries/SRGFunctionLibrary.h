#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SRGCore/SRGEnumStruct.h"
#include "SRGFunctionLibrary.generated.h"

class AExploreHeroBase;
/**
 * 
 */
UCLASS()
class SRG_API USRGFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="SRG Function Library")
	static void DisableActor(AActor* Actor,bool IsDisabled);
	
	
	UFUNCTION(BlueprintCallable, Category="SRG Function Library")
	static int32 GetHeroStat(int32 Level, int32 MaxLevel, int32 InitialStat, int32 FirstLevelStat, int32 LastLevelStat);
	
	UFUNCTION(BlueprintCallable, Category="SRG Function Library")
	static FHeroStats CalculateHeroStats(int32 Level, int32 MaxLevel, FHeroStats InitialStats, FHeroStats FirstLevelStats, FHeroStats LastLevelStats);

	/*Exp, Exponent 및 MaxLevel을 기준으로 레벨 계산 및 반환*/
	UFUNCTION(BlueprintCallable, Category="SRG Function Library", meta=(WorldContext="WorldContextObject"))
	static int32 GetHeroLevel(UObject* WorldContextObject, int32 Exp, float ExpExponent, int32 MaxLevel);

	UFUNCTION(BlueprintCallable, Category="SRG Function Library", meta=(WorldContext="WorldContextObject"))
	static void GetHeroStats(UObject* WorldContextObject, AExploreHeroBase* ExploreHero, int32& OutLevel, FHeroStats& OutHeroStats);

	UFUNCTION(BlueprintCallable, Category="SRG Function Library", meta=(WorldContext="WorldContextObject"))
	static FHeroStats GetEquipmentStats(UObject* WorldContextObject, const TArray<TSubclassOf<AEquipmentBase>>& Equipments);
	
	UFUNCTION(BlueprintCallable, Category="SRG Function Library", meta=(WorldContext="WorldContextObject"))
	static FHeroStats GetEquipmentStatsFromHero(UObject* WorldContextObject, AExploreHeroBase* ExploreHero);
	
	UFUNCTION(BlueprintCallable, Category="SRG Function Library", meta=(WorldContext="WorldContextObject"))
	static FHeroStats GetHeroStatsWithEquipment(UObject* WorldContextObject, AExploreHeroBase* ExploreHero);
};
