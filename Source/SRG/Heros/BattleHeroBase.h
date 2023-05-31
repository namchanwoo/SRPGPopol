#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "SRGCore/SRGEnumStruct.h"
#include "BattleHeroBase.generated.h"

class AEquipmentBase;
class ABattleController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaSet, int32, InMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpellCasted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClicked);

UCLASS()
class SRG_API ABattleHeroBase : public AActor
{
	GENERATED_BODY()

public:
	ABattleHeroBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void InitializeEvent();
	
	void AddDrops(int32 NewGold, const TArray<TSubclassOf<AEquipmentBase>>& NewDrops);

	/*******************************************
	 * Field Members
	 *******************************************/

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	ABattleController* BattleController;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	int32 Gold;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	int32 CurrentMana;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	FHeroStats HeroStats;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	FText Name;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	UTexture2D* HeroImage;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	UTexture2D* HeroDetailsImage;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	int32 Level;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	bool bCanUseSpell;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	TMap<TSubclassOf<ASpellBase>, int32> Spells;

	UPROPERTY(BlueprintReadOnly, Category="Battle Hero")
	TArray<TSubclassOf<AEquipmentBase>> BackPack;

	UPROPERTY(BlueprintReadOnly, Category="Battle Hero")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnManaSet OnManaSet;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnAnimationEnded OnAnimationEnded;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnSpellCasted OnSpellCasted;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnRightClicked OnRightClicked;
	
};
