#include "SRG/Libraries/SRGFunctionLibrary.h"

#include "Kismet/KismetMathLibrary.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "SRG/Equipment/EquipmentBase.h"
#include "SRG/Interactables/InteractionDetector.h"


void USRGFunctionLibrary::DisableActor(AActor* Actor, bool IsDisabled)
{
	Actor->SetActorHiddenInGame(IsDisabled);
	Actor->SetActorEnableCollision(!IsDisabled);
	Actor->SetActorTickEnabled(!IsDisabled);

	UChildActorComponent* ChildActorComponent = Actor->FindComponentByClass<UChildActorComponent>();
	if (ChildActorComponent && ChildActorComponent->GetChildActor())
	{
		AInteractionDetector* InteractionDetector = Cast<AInteractionDetector>(ChildActorComponent->GetChildActor());
		if (InteractionDetector)
		{
			InteractionDetector->SetActorHiddenInGame(IsDisabled);
		}
	}
}

int32 USRGFunctionLibrary::GetHeroStat(int32 Level, int32 MaxLevel, int32 InitialStat, int32 FirstLevelStat, int32 LastLevelStat)
{
	const float RangeValue = UKismetMathLibrary::MapRangeClamped(Level, 1.0f, MaxLevel, FirstLevelStat, LastLevelStat);
	return InitialStat + FMath::TruncToInt(RangeValue);
}

FHeroStats USRGFunctionLibrary::CalculateHeroStats(int32 Level, int32 MaxLevel, FHeroStats InitialStats, FHeroStats FirstLevelStats,
                                                   FHeroStats LastLevelStats)
{
	FHeroStats Result;

	Result.Attack = GetHeroStat(Level, MaxLevel, InitialStats.Attack,
	                            FirstLevelStats.Attack, LastLevelStats.Attack);

	Result.Defense = GetHeroStat(Level, MaxLevel, InitialStats.Defense,
	                             FirstLevelStats.Defense, LastLevelStats.Defense);

	Result.Power = GetHeroStat(Level, MaxLevel, InitialStats.Power,
	                           FirstLevelStats.Power, LastLevelStats.Power);

	Result.Knowledge = GetHeroStat(Level, MaxLevel, InitialStats.Knowledge,
	                               FirstLevelStats.Knowledge, LastLevelStats.Knowledge);

	return Result;
}


int32 USRGFunctionLibrary::GetHeroLevel(UObject* WorldContextObject, int32 Exp, float ExpExponent, int32 MaxLevel)
{
	// 수식의 제곱근 부분을 계산합니다.
	float sqrtPart = UKismetMathLibrary::Sqrt((ExpExponent * ExpExponent) + (ExpExponent * 4 * Exp));

	// 공식의 분자를 계산
	float numerator = ExpExponent + sqrtPart;

	// 수식의 분모 계산
	float denominator = ExpExponent * 2;

	// 수식을 사용하여 로컬 값을 계산합니다.
	float LocalLevelValue = FMath::FloorToInt(numerator / denominator);

	// 계산된 레벨과 최대 레벨 사이의 최소값 반환
	return FMath::Min(FMath::TruncToInt(LocalLevelValue), MaxLevel);
}


void USRGFunctionLibrary::GetHeroStats(UObject* WorldContextObject, AExploreHeroBase* ExploreHero, int32& OutLevel, FHeroStats& OutHeroStats)
{
	OutLevel = GetHeroLevel(WorldContextObject, ExploreHero->Exp, ExploreHero->ExpExponent, ExploreHero->MaxLevel);

	OutHeroStats = CalculateHeroStats(OutLevel, ExploreHero->MaxLevel,
	                                  ExploreHero->InitialStats, ExploreHero->FirstLevelStats, ExploreHero->LastLevelStats);
}

FHeroStats USRGFunctionLibrary::GetEquipmentStats(UObject* WorldContextObject, const TArray<TSubclassOf<AEquipmentBase>>& Equipments)
{
	FHeroStats CurrentEquipmentStats;

	for (TSubclassOf<AEquipmentBase> Equipment : Equipments)
	{
		AEquipmentBase* EquipmentObejct = Equipment->GetDefaultObject<AEquipmentBase>();
		CurrentEquipmentStats = EquipmentObejct->HeroStats + CurrentEquipmentStats;
	}

	return CurrentEquipmentStats;
}

FHeroStats USRGFunctionLibrary::GetEquipmentStatsFromHero(UObject* WorldContextObject, AExploreHeroBase* ExploreHero)
{
	return GetEquipmentStats(WorldContextObject, ExploreHero->Equipment);
}

FHeroStats USRGFunctionLibrary::GetHeroStatsWithEquipment(UObject* WorldContextObject, AExploreHeroBase* ExploreHero)
{
	int32 Level;
	FHeroStats LevelHeroStats;
	GetHeroStats(WorldContextObject, ExploreHero, Level, LevelHeroStats);
	FHeroStats EquipmentHeroStats = GetEquipmentStatsFromHero(WorldContextObject, ExploreHero);
	return EquipmentHeroStats + LevelHeroStats;
}
