#include "SRG/Libraries/SRPGFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "SRG/Core/SRPGGameInstance.h"
#include "SRG/Equipment/EquipmentBase.h"
#include "SRG/Interactables/InteractionDetector.h"
#include "SRGCore/Utilities/ColorNames.h"
#include "SRGCore/Utilities/SRGLog.h"


void USRPGFunctionLibrary::DisableActor(AActor* Actor, bool IsDisabled)
{
	// 액터가 nullptr인지 확인하고 참이면 경고를 기록합니다.
	if (!Actor)
	{
		SRPG_LOG_WARNING(TEXT("Actor is null!"));
		return;
	}

	// IsDisabled에 따라 액터의 가시성, 콜리전, 틱 상태를 설정합니다.
	Actor->SetActorHiddenInGame(IsDisabled);
	Actor->SetActorEnableCollision(!IsDisabled);
	Actor->SetActorTickEnabled(!IsDisabled);

	// 액터에서 UChildActorComponent 찾기 시도
	UChildActorComponent* ChildActorComponent = Actor->FindComponentByClass<UChildActorComponent>();
	if (ChildActorComponent)
	{
		// 구성 요소의 하위 액터를 가져옵니다.
		AActor* ChildActor = ChildActorComponent->GetChildActor();
		if (ChildActor)
		{
			// 하위 액터를 AInteractionDetector로 캐스팅하고 IsDisabled를 기반으로 게임에서 숨깁니다.
			AInteractionDetector* InteractionDetector = Cast<AInteractionDetector>(ChildActor);
			if (InteractionDetector)
			{
				InteractionDetector->SetActorHiddenInGame(IsDisabled);
			}
		}
	}
}

int32 USRPGFunctionLibrary::GetExp(int32 Level, float ExpExponent)
{
	if (Level < 0)
	{
		SRPG_LOG_WARNING(TEXT("레벨이 0보다 작음!"));
		return 0;
	}

	return UKismetMathLibrary::FTrunc((ExpExponent * FMath::Square(Level)) - (ExpExponent * Level));
}

int32 USRPGFunctionLibrary::GetHeroStat(int32 Level, int32 MaxLevel, int32 InitialStat, int32 FirstLevelStat,
                                        int32 LastLevelStat)
{
	if (Level < 0 || MaxLevel < 0 || InitialStat < 0 || FirstLevelStat < 0 || LastLevelStat < 0)
	{
		SRPG_LOG_WARNING(TEXT("하나 이상의 입력 매개변수가 0보다 작습니다!"));
		return 0;
	}

	const float RangeValue = UKismetMathLibrary::MapRangeClamped(Level, 1.0f, MaxLevel, FirstLevelStat, LastLevelStat);
	return InitialStat + FMath::TruncToInt(RangeValue);
}

FHeroStats USRPGFunctionLibrary::CalculateHeroStats(int32 Level, int32 MaxLevel, FHeroStats InitialStats,
                                                    FHeroStats FirstLevelStats,
                                                    FHeroStats LastLevelStats)
{
	// 결과 HeroStats 구조체 정의
	FHeroStats Result;

	// 주어진 레벨에 대한 영웅의 공격 통계를 계산합니다.
	Result.Attack = GetHeroStat(Level, MaxLevel, InitialStats.Attack,
	                            FirstLevelStats.Attack, LastLevelStats.Attack);
	SRPG_LOG(Log, TEXT("레벨 %d의 계산된 공격 통계는 %f입니다."), Level, Result.Attack);

	// 주어진 레벨에 대한 영웅의 방어 통계를 계산합니다.
	Result.Defense = GetHeroStat(Level, MaxLevel, InitialStats.Defense,
	                             FirstLevelStats.Defense, LastLevelStats.Defense);
	SRPG_LOG(Log, TEXT("%d 레벨의 계산된 방어 능력치는 %f입니다."), Level, Result.Defense);

	// 주어진 레벨에 대한 영웅의 파워 통계를 계산합니다.
	Result.Power = GetHeroStat(Level, MaxLevel, InitialStats.Power,
	                           FirstLevelStats.Power, LastLevelStats.Power);
	SRPG_LOG(Log, TEXT("레벨 %d의 계산된 전력 통계는 %f입니다."), Level, Result.Power);

	// 주어진 레벨에 대한 영웅의 지식 통계를 계산합니다.
	Result.Knowledge = GetHeroStat(Level, MaxLevel, InitialStats.Knowledge,
	                               FirstLevelStats.Knowledge, LastLevelStats.Knowledge);
	SRPG_LOG(Log, TEXT("%d 레벨의 계산된 지식 통계는 %f입니다."), Level, Result.Knowledge);

	// 결과 HeroStats 구조체를 반환합니다.
	return Result;
}

int32 USRPGFunctionLibrary::GetHeroLevel(int32 Exp, float ExpExponent, int32 MaxLevel)
{
	// 불필요한 계산을 피하기 위해 MaxLevel이 0보다 큰지 확인
	if (MaxLevel <= 0)
	{
		SRPG_LOG_ERROR(TEXT("MaxLevel은 GetHeroLevel에서 0보다 커야 합니다."));
		return -1;
	}

	// 수식의 제곱근 부분을 계산합니다.
	const float sqrtPart = UKismetMathLibrary::Sqrt((ExpExponent * ExpExponent) + (ExpExponent * 4 * Exp));

	//공식의 분자를 계산
	const float numerator = ExpExponent + sqrtPart;

	// 수식의 분모 계산
	const float denominator = ExpExponent * 2;

	// 수식을 사용하여 로컬 값을 계산합니다.
	const float LocalLevelValue = FMath::FloorToInt(numerator / denominator);

	// 계산된 수준과 최대 수준 사이의 최소값 반환
	const int32 finalLevel = FMath::Min(FMath::TruncToInt(LocalLevelValue), MaxLevel);

	SRPG_LOG(Log, TEXT("계산된 영웅 레벨은 %d입니다."), finalLevel);
	return finalLevel;
}

void USRPGFunctionLibrary::GetHeroStats(AExploreHeroBase* ExploreHero, int32& OutLevel,
                                        FHeroStats& OutHeroStats)
{
	OutLevel = GetHeroLevel(ExploreHero->Exp, ExploreHero->ExpExponent, ExploreHero->MaxLevel);

	OutHeroStats = CalculateHeroStats(OutLevel, ExploreHero->MaxLevel,
	                                  ExploreHero->InitialStats, ExploreHero->FirstLevelStats,
	                                  ExploreHero->LastLevelStats);
}

FHeroStats USRPGFunctionLibrary::GetEquipmentStats(UObject* WorldContextObject,
                                                   const TArray<TSubclassOf<AEquipmentBase>>& Equipments)
{
	// WorldContextObject가 유효한지 확인하십시오.
	if (!IsValid(WorldContextObject))
	{
		SRPG_LOG_ERROR(TEXT("GetEquipmentStats에 잘못된 WorldContextObject가 있습니다."));
		return FHeroStats(); // 기본 구성된 HeroStats 반환
	}

	// Equipments 배열이 비어 있는지 확인하십시오.
	if (Equipments.Num() <= 0)
	{
		SRPG_LOG_WARNING(TEXT("GetEquipmentStats의 빈 장비 배열입니다."));
		return FHeroStats(); // 기본 구성된 HeroStats 반환
	}

	FHeroStats CurrentEquipmentStats;

	for (TSubclassOf<AEquipmentBase> Equipment : Equipments)
	{
		if (Equipment != nullptr)
		{
			if (AEquipmentBase* EquipmentObject = Equipment->GetDefaultObject<AEquipmentBase>())
			{
				CurrentEquipmentStats = EquipmentObject->HeroStats + CurrentEquipmentStats;
			}
			else
			{
				SRPG_LOG_ERROR(TEXT("GetEquipmentStats에서 장비의 기본 개체를 가져올 수 없습니다."));
			}
		}
		else
		{
			SRPG_LOG_WARNING(TEXT("GetEquipmentStats의 장비 배열에 있는 Null 장비 하위 클래스입니다."));
		}
	}

	return CurrentEquipmentStats;
}

FHeroStats USRPGFunctionLibrary::GetEquipmentStatsFromHero(UObject* WorldContextObject, AExploreHeroBase* ExploreHero)
{
	return GetEquipmentStats(WorldContextObject, ExploreHero->Equipment);
}

FHeroStats USRPGFunctionLibrary::GetHeroStatsWithEquipment(UObject* WorldContextObject, AExploreHeroBase* ExploreHero)
{
	// WorldContextObject가 유효한지 확인하십시오.
	if (!IsValid(WorldContextObject))
	{
		SRPG_LOG_ERROR(TEXT("GetHeroStatsWithEquipment에 잘못된 WorldContextObject가 있습니다."));
		return FHeroStats(); // return a default constructed HeroStats
	}

	// ExploreHero가 유효한지 확인
	if (!IsValid(ExploreHero))
	{
		SRPG_LOG_ERROR(TEXT("GetHeroStatsWithEquipment에 잘못된 ExploreHero가 있습니다."));
		return FHeroStats(); // return a default constructed HeroStats
	}

	int32 Level;
	FHeroStats LevelHeroStats;

	// 영웅 통계 얻기
	GetHeroStats(ExploreHero, Level, LevelHeroStats);

	// 장비 통계 가져오기
	const FHeroStats EquipmentHeroStats = GetEquipmentStatsFromHero(WorldContextObject, ExploreHero);
	if (EquipmentHeroStats == FHeroStats())
	{
		SRPG_LOG_WARNING(TEXT("GetHeroStatsWithEquipment의 빈 장비 통계."));
	}

	return EquipmentHeroStats + LevelHeroStats;
}

void USRPGFunctionLibrary::GetElementColorAndImage(UObject* WorldContextObject, EElement InElement,
                                                   FLinearColor& ElementColor,
                                                   UTexture2D*& ElementImage, UTexture2D*& ElementResistImage)
{
	USRPGGameInstance* SRPGGameInstance = Cast<
		USRPGGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	SRPG_CHECK(SRPGGameInstance);

	switch (InElement)
	{
	case EElement::Normal:
		ElementColor = SRPG_COLOR_BLACK;
		ElementImage = nullptr;
		break;

	case EElement::Fire:
		ElementColor = SRPG_COLOR_ORANGE;
		ElementImage = SRPGGameInstance->Element_Fire;
		ElementResistImage = SRPGGameInstance->ElementResist_Fire;
		break;

	case EElement::Earth:
		ElementColor = SRPG_COLOR_GREEN;
		ElementImage = SRPGGameInstance->Element_Earth;
		ElementResistImage = SRPGGameInstance->ElementResist_Earth;
		break;

	case EElement::Air:
		ElementColor = SRPG_COLOR_SKY_BLUE;
		ElementImage = SRPGGameInstance->Element_Air;
		ElementResistImage = SRPGGameInstance->ElementResist_Air;
		break;

	case EElement::Water:
		ElementColor = SRPG_COLOR_BLUE;
		ElementImage = SRPGGameInstance->Element_Water;
		ElementResistImage = SRPGGameInstance->ElementResist_Water;
		break;

	default: break;
	}
}
