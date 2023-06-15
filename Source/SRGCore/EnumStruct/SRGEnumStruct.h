#pragma once

#include "CoreMinimal.h"
#include "SRGEnumStruct.generated.h"

class AEquipmentBase;
class ACharacterBase;
class ASlotBase;
class UPaperSprite;

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	NoSlot,
	DefaultSlot,
	MovableSlot,
	ActiveSlot,
	EnemySlot,
	AllySlot,
	MovementOverlaySlot,
	SpellSlot
};

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
	/** 전투가 초기화되고 있습니다. */
	Initialization UMETA(DisplayName = "Initialization"),
	/** 플레이어가 캐릭터를 배치하는 배치 단계입니다. */
	DeploymentPhase UMETA(DisplayName = "Deployment Phase"),
	/** 플레이어가 액션을 실행하기를 기다립니다. */
	WaitingForPlayerAction UMETA(DisplayName = "Waiting for Player Action"),
	/** 플레이어는 현재 주문을 시전하고 있습니다. */
	PlayerIsCastingSpell UMETA(DisplayName = "Player is Casting Spell"),
	/** 플레이어가 적극적으로 자신의 턴을 플레이하고 있습니다. */
	PlayerIsPlaying UMETA(DisplayName = "Player is Playing"),
	/** 적의 행동을 기다리고 있습니다. */
	WaitingForEnemyAction UMETA(DisplayName = "Waiting for Enemy Action"),
	/** 적은 적극적으로 자신의 턴을 플레이하고 있습니다. */
	EnemyIsPlaying UMETA(DisplayName = "Enemy is Playing"),
	/** 플레이어가 승리를 달성했습니다. */
	Victory UMETA(DisplayName = "Victory"),
	/** 플레이어가 패배했습니다. */
	Defeat UMETA(DisplayName = "Defeat"),
	/** 전투가 끝났습니다. */
	Over UMETA(DisplayName = "Over")
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
enum class EPassiveAbilityTarget : uint8
{
	Self,
	CharactersInRange,
	HitCharacters
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

	FCharacterStatsBuff(): Attack(0), Defense(0), MinDamage(0), MaxDamage(0), Ammo(0), MovementRange(0)
	{
	}

	FCharacterStatsBuff(float InAttack, float InDefense, float InMinDamage,
	                    float InMaxDamage, float InAmmo, float InMovementRange)
		: Attack(InAttack), Defense(InDefense), MinDamage(InMinDamage), MaxDamage(InMaxDamage), Ammo(InAmmo),
		  MovementRange(InMovementRange)
	{
	}
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
struct FListOfSlots
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="List Of Slots")
	TArray<ASlotBase*> OverlaySlots;

	FListOfSlots()
	{
	}

	FListOfSlots(const TArray<ASlotBase*>& NewOverlaySlots)
	{
		OverlaySlots = NewOverlaySlots;
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


USTRUCT(BlueprintType)
struct FSlotSprites
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Sprites")
	UPaperSprite* DefaultSlotSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Sprites")
	UPaperSprite* MovableSlotSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Sprites")
	UPaperSprite* ActiveSlotSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Sprites")
	UPaperSprite* EnemySlotSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Sprites")
	UPaperSprite* AllySlotSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Sprites")
	UPaperSprite* MovementOverlaySlotSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Sprites")
	UPaperSprite* SpellSlotSprite;
};

USTRUCT(BlueprintType)
struct FSlotSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Setting")
	FVector SlotSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Setting")
	FVector SlotSpriteSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Setting")
	FSlotSprites SlotSprites1_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Setting")
	FSlotSprites SlotSprites2_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Setting")
	FSlotSprites SlotSprites3_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Setting")
	FSlotSprites SlotSprites4_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Setting")
	FSlotSprites SlotSprites5_5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Setting")
	FSlotSprites SlotSprites6_6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Setting")
	FSlotSprites SlotSprites7_7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot Setting")
	FSlotSprites SlotSprites8_8;

	FSlotSetting(): SlotSprites1_1(), SlotSprites2_2(), SlotSprites3_3(), SlotSprites4_4(),
	                SlotSprites5_5(), SlotSprites6_6(), SlotSprites7_7(), SlotSprites8_8()
	{
		SlotSize = FVector(60.0f, 60.0f, 2.0f);
	}
};
