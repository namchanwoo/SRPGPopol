#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AILogicBase.generated.h"

class ASlotBase;
class AGrid;
class ACharacterBase;
class ABattleController;
class AActiveAbilityBase;

UCLASS()
class SRG_API AAILogicBase : public AActor
{
	GENERATED_BODY()

public:
	AAILogicBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void InitializeEvent(ABattleController* InBattleController, AGrid* InGrid);

	void RunAILogic(ACharacterBase* InCharacter);


	void CalculateMovableSlots(ACharacterBase* InCharacter, bool& IsMeleeAttack,
	                           TArray<ASlotBase*>& OutEnemySlots, TArray<ASlotBase*>& OutAllySlots,
	                           ASlotBase*& OutClosetSlotToMove);

	ACharacterBase* GetWeakestEnemy(const TArray<ASlotBase*>& InEnemySlots);

	AActiveAbilityBase* GetActiveAbility();

	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Logic")
	AGrid* Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Logic")
	ABattleController* BattleController;

	UPROPERTY(BlueprintReadWrite, Category="AI Logic")
	ASlotBase* ClosestSlotToMove;

	UPROPERTY(BlueprintReadWrite, Category="AI Logic")
	AActiveAbilityBase* CurrentAvailableAbility;
	
	UPROPERTY(BlueprintReadWrite, Category="AI Logic")
	ACharacterBase* CurrentPlayingCharacter;

	UPROPERTY(BlueprintReadWrite, Category="AI Logic")
	ACharacterBase* WeakestEnemy;
};
