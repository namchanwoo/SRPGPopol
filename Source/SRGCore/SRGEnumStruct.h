#pragma once

#include "CoreMinimal.h"
#include "SRGEnumStruct.generated.h"

class ACharacterBase;

UENUM(BlueprintType)
enum class EExploreCursorType : uint8
{
	Default,
	Inventory,
	Enemy
};

UENUM(BlueprintType)
enum class EEquipmenet : uint8
{
	None,
	Weapon,
	Shield,
	Armor,
	Helmet,
	Shoes,
	Necklace,
	Misc
};


USTRUCT(BlueprintType)
struct FHeroStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Stats")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Stats")
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Stats")
	float Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Stats")
	float Knowledge;

	FHeroStats(): Attack(0), Defense(0), Power(0), Knowledge(0)
	{
	}

	FHeroStats(float InValue)
		: Attack(InValue), Defense(InValue), Power(InValue), Knowledge(InValue)
	{
	}

	FHeroStats(float InAttack, float InDefense, float InPower, float InKnowledge)
		: Attack(InAttack), Defense(InDefense), Power(InPower), Knowledge(InKnowledge)
	{
	}

	FHeroStats operator+(const FHeroStats& Other) const
	{
		return FHeroStats(
			Attack + Other.Attack,
			Defense + Other.Defense,
			Power + Other.Power,
			Knowledge + Other.Knowledge
		);
	}

	FHeroStats operator-(const FHeroStats& Other) const
	{
		return FHeroStats(
			Attack - Other.Attack,
			Defense - Other.Defense,
			Power - Other.Power,
			Knowledge - Other.Knowledge
		);
	}

	FHeroStats& operator+=(const FHeroStats& Other)
	{
		Attack += Other.Attack;
		Defense += Other.Defense;
		Power += Other.Power;
		Knowledge += Other.Knowledge;
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FPlayerCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Character Data")
	TSubclassOf<ACharacterBase> Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Character Data")
	int32 Stack = 1;
};
