#pragma once

#include "CoreMinimal.h"
#include "SRGEnumStruct.generated.h"

class AEquipmentBase;
class ACharacterBase;

UENUM(BlueprintType)
enum class EExploreCursorType : uint8
{
	Default,
	Inventory,
	Enemy
};


UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
	Unavailable,
	New,
	Active,
	ReadyToDeliver,
	Completed
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

	bool operator==(const FHeroStats& Other) const
	{
		return Attack == Other.Attack
			&& Defense == Other.Defense
			&& Power == Other.Power
			&& Knowledge == Other.Knowledge;
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

	FPlayerCharacterData()
	{
	}

	FPlayerCharacterData(TSubclassOf<ACharacterBase> NewCharacter, int32 NewStack) : Character(NewCharacter),
		Stack(NewStack)
	{
	}
};


USTRUCT(BlueprintType)
struct FQuestStepData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Step Data")
	int32 CurrentStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Step Data")
	int32 TotalSteps;

	FQuestStepData(): CurrentStep(0), TotalSteps(0)
	{
	}

	FQuestStepData(int32 NewCurrentStep, int32 NewTotalSteps) : CurrentStep(NewCurrentStep), TotalSteps(NewTotalSteps)
	{
	}
};

USTRUCT(BlueprintType)
struct FQuestSounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Sounds")
	USoundBase* NewQuestSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Sounds")
	USoundBase* QuestUpdateSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Sounds")
	USoundBase* QuestCompletedSound;
};

USTRUCT(BlueprintType)
struct FQuestRewards
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestRewards")
	TArray<FPlayerCharacterData> Characters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestRewards")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestRewards")
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuestRewards")
	int32 Exp;
};

USTRUCT(BlueprintType)
struct FEnemyCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Character Data")
	TSubclassOf<ACharacterBase> Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Character Data")
	int32 MinStack = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Character Data")
	int32 MaxStack = 1;
};


USTRUCT(BlueprintType)
struct FShopRecruitmentList
{
	GENERATED_BODY()

	/*모집 목록*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shop Recruitment List")
	TArray<FPlayerCharacterData> RecruitmentList;
};

USTRUCT(BlueprintType)
struct FShopEquipmentList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shop Equipment List")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;
};
