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
enum class EBattleState : uint8
{
	Initialization,
	DeploymentPhase,
	WaitingForPlayerAction,
	PlayerIsCastingSpell,
	PlayerIsPlaying,
	WaitingForEnemyAction,
	EnemyIsPlaying,
	Victory,
	Defeat,
	Over
};


UENUM(BlueprintType)
enum class EBattleCursorType : uint8
{
	Default,
	Walk,
	Fly,
	MeleeAttack,
	RangeAttack,
	BrokenRangeAttack,
	Spell,
	Wait
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
enum class EElement : uint8
{
	Normal,
	Fire,
	Earth,
	Air,
	Water
};


UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Walk,
	Fly,
	Immobilie
};

UENUM(BlueprintType)
enum class EPassiveAbilityUseMoment : uint8
{
	OnTurnStart,
	OnMoved,
	BeforeAttack,
	AfterAttack,
	OnKill,
	OnTurnEnd
};

UENUM(BlueprintType)
enum class EEquipment : uint8
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
struct FCharacterStats
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	int32 MaxHealth = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	int32 MaxMana = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	int32 Attack = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	int32 Defense = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	int32 MinDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	int32 MaxDamage = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	int32 Ammo = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	int32 MovementRange = 4;

	// Default constructor
	FCharacterStats() : MaxHealth(5), MaxMana(10), Attack(1), Defense(0),
	                    MinDamage(1), MaxDamage(2), Ammo(5), MovementRange(4)
	{
	}

	// Constructor with parameters
	FCharacterStats(int32 InMaxHealth, int32 InMaxMana, int32 InAttack, int32 InDefense,
	                int32 InMinDamage, int32 InMaxDamage, int32 InAmmo, int32 InMovementRange)
		: MaxHealth(InMaxHealth)
		  , MaxMana(InMaxMana)
		  , Attack(InAttack)
		  , Defense(InDefense)
		  , MinDamage(InMinDamage)
		  , MaxDamage(InMaxDamage)
		  , Ammo(InAmmo)
		  , MovementRange(InMovementRange)
	{
	}

	// Copy constructor
	FCharacterStats(const FCharacterStats& Other) : MaxHealth(Other.MaxHealth)
	                                                , MaxMana(Other.MaxMana)
	                                                , Attack(Other.Attack)
	                                                , Defense(Other.Defense)
	                                                , MinDamage(Other.MinDamage)
	                                                , MaxDamage(Other.MaxDamage)
	                                                , Ammo(Other.Ammo)
	                                                , MovementRange(Other.MovementRange)
	{
	}

	// Assignment operator
	FCharacterStats& operator=(const FCharacterStats& Other)
	{
		if (this != &Other)
		{
			MaxHealth = Other.MaxHealth;
			MaxMana = Other.MaxMana;
			Attack = Other.Attack;
			Defense = Other.Defense;
			MinDamage = Other.MinDamage;
			MaxDamage = Other.MaxDamage;
			Ammo = Other.Ammo;
			MovementRange = Other.MovementRange;
		}
		return *this;
	}

	// Addition operator
	FCharacterStats operator+(const FCharacterStats& Other) const
	{
		FCharacterStats Sum;
		Sum.MaxHealth = MaxHealth + Other.MaxHealth;
		Sum.MaxMana = MaxMana + Other.MaxMana;
		Sum.Attack = Attack + Other.Attack;
		Sum.Defense = Defense + Other.Defense;
		Sum.MinDamage = MinDamage + Other.MinDamage;
		Sum.MaxDamage = MaxDamage + Other.MaxDamage;
		Sum.Ammo = Ammo + Other.Ammo;
		Sum.MovementRange = MovementRange + Other.MovementRange;
		return Sum;
	}


	FCharacterStats operator+(const FHeroStats& Other) const
	{
		FCharacterStats Sum;
		Sum.Attack = Attack + Other.Attack;
		Sum.Defense = Defense + Other.Defense;
		return Sum;
	}

	// Subtraction operator
	FCharacterStats operator-(const FCharacterStats& Other) const
	{
		FCharacterStats Difference;
		Difference.MaxHealth = MaxHealth - Other.MaxHealth;
		Difference.MaxMana = MaxMana - Other.MaxMana;
		Difference.Attack = Attack - Other.Attack;
		Difference.Defense = Defense - Other.Defense;
		Difference.MinDamage = MinDamage - Other.MinDamage;
		Difference.MaxDamage = MaxDamage - Other.MaxDamage;
		Difference.Ammo = Ammo - Other.Ammo;
		Difference.MovementRange = MovementRange - Other.MovementRange;
		return Difference;
	}

	FCharacterStats Negative()
	{
		// 모든 필드가 기본적으로 0이라고 가정합니다.
		FCharacterStats ZeroStats = FCharacterStats(0, 0, 0, 0, 0, 0, 0, 0);
		return ZeroStats - *this;
	}
};



USTRUCT(BlueprintType)
struct FCharacterStatsBuff
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats Buff")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats Buff")
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats Buff")
	float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats Buff")
	float MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats Buff")
	float Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats Buff")
	float MovementRange;
};


USTRUCT(BlueprintType)
struct FElementReductions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Element Reductions")
	float Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Element Reductions")
	float Fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Element Reductions")
	float Earth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Element Reductions")
	float Air;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Element Reductions")
	float Water;

	FElementReductions(): Normal(0), Fire(0), Earth(0), Air(0), Water(0)
	{
	}

	// Subtraction operator
	FElementReductions operator-(const FElementReductions& Other) const
	{
		FElementReductions Difference;
		Difference.Normal = Normal - Other.Normal;
		Difference.Fire = Fire - Other.Fire;
		Difference.Earth = Earth - Other.Earth;
		Difference.Air = Air - Other.Air;
		Difference.Water = Water - Other.Water;
		return Difference;
	}

	FElementReductions Negative()
	{
		// 모든 필드가 기본적으로 0이라고 가정합니다.
		FElementReductions ZeroElementReductions;
		return ZeroElementReductions - *this;
	}
};


USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage Data")
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage Data")
	int32 MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage Data")
	int32 MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage Data")
	float DamageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage Data")
	EElement Element = EElement::Normal;
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
