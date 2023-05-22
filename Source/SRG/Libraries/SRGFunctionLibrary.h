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
	/**
 	 * Is Disabled 부울에 따라 액터를 비활성화하거나 활성화합니다. 비활성화되면 액터가 숨겨지고 충돌이 꺼지며 틱되지 않습니다.
 	 */
	UFUNCTION(BlueprintCallable, Category="SRG Function Library")
	static void DisableActor(AActor* Actor, bool IsDisabled);

	/**
	 * 지수 공식을 사용하여 주어진 레벨에 필요한 경험치를 반환합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="SRG Function Library")
	int32 GetExp(int32 Level, float ExpExponent);

	/**
	 * 초기 통계, 첫 번째 수준의 통계, 최대 수준의 통계가 주어지면 주어진 수준에서 영웅의 단일 통계를 계산합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="SRG Function Library")
	static int32 GetHeroStat(int32 Level, int32 MaxLevel, int32 InitialStat, int32 FirstLevelStat, int32 LastLevelStat);

	/**
	 * 경험, 초기 통계, 첫 번째 수준의 통계 및 최대 수준의 통계를 기반으로 영웅의 통계 및 레벨을 검색합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="SRG Function Library", meta=(WorldContext="WorldContextObject"))
	static void GetHeroStats(AExploreHeroBase* ExploreHero, int32& OutLevel,
	                         FHeroStats& OutHeroStats);

	/**
	 * 초기 통계, 첫 번째 수준의 통계 및 최대 수준의 통계가 주어지면 주어진 수준에서 영웅의 통계를 계산합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="SRG Function Library")
	static FHeroStats CalculateHeroStats(int32 Level, int32 MaxLevel, FHeroStats InitialStats,
	                                     FHeroStats FirstLevelStats, FHeroStats LastLevelStats);

	/**
	 * 영웅의 경험, 경험 지수 및 최대 레벨을 기반으로 영웅의 레벨을 계산합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="SRG Function Library", meta=(WorldContext="WorldContextObject"))
	static int32 GetHeroLevel(int32 Exp, float ExpExponent, int32 MaxLevel);

	/**
	 * 장비 항목 세트에서 제공하는 총 통계를 검색합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="SRG Function Library", meta=(WorldContext="WorldContextObject"))
	static FHeroStats GetEquipmentStats(UObject* WorldContextObject,
	                                    const TArray<TSubclassOf<AEquipmentBase>>& Equipments);

	/**
	 * 영웅이 장착한 아이템이 제공하는 총 통계를 검색합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="SRG Function Library", meta=(WorldContext="WorldContextObject"))
	static FHeroStats GetEquipmentStatsFromHero(UObject* WorldContextObject, AExploreHeroBase* ExploreHero);

	/**
	 * 영웅의 통계와 장비한 아이템이 제공하는 통계를 검색합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="SRG Function Library", meta=(WorldContext="WorldContextObject"))
	static FHeroStats GetHeroStatsWithEquipment(UObject* WorldContextObject, AExploreHeroBase* ExploreHero);
};
