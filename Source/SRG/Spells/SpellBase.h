#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "SpellBase.generated.h"

class ASlotBase;
class ACharacterBase;
class ABattleController;
class ABattleHeroBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpellUseEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpellUseStarted);



UCLASS()
class SRG_API ASpellBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpellBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnSpellEnabled();
	
	void OnSpellDisabled();

	TMap<ACharacterBase*, FDamageData> GetSpellDamage(TArray<ACharacterBase*> InTargetCharacters);
	
	void PlaySpellAnimation();
	
	void SetTargetSlot(ASlotBase* InSlot);
	
	void GetSpellSlots(TArray<ASlotBase*>& OutSlotsInRange,TArray<ASlotBase*>& OutEmptySlots,
		TArray<ACharacterBase*>& OutAllies,TArray<ACharacterBase*>& OutEnemies);

	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spell")
	int32 ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spell")
	EElement Element;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spell")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spell")
	UTexture2D* CursorIcon;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Spell|Delegate")
	FOnSpellUseEnded OnSpellUseEnded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Spell|Delegate")
	FOnSpellUseStarted OnSpellUseStarted;
	ABattleController* BattleController;
	ABattleHeroBase* Hero;
};
