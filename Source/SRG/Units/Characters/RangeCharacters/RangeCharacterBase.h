
#pragma once

#include "CoreMinimal.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "RangeCharacterBase.generated.h"

class AProjectileBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileHit_RangeCharacter);

UCLASS()
class SRG_API ARangeCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	ARangeCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	void RangeAttack(ACharacterBase* InTargetCharacter);
	
	void AbilityAttack(ACharacterBase* InCurrentTargetCharacter, UAnimMontage* MontageToPlay);
	
	void ChangeProjectile(TSubclassOf<AProjectileBase> NewProjectile);

	void ResetProjectile();
	
	FVector GetProjectileSpawnLocation();


protected:
	UFUNCTION()
	void OnProjectileHitHandler();
	
	UFUNCTION()
	void OnRangeAttackHandler();
	
	UFUNCTION()
	void OnRangeAttackAbilityHandler();
	
	
	/*******************************************
	 * Field Members
	 *******************************************/

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Animations")
	TArray<UAnimMontage*> RangeAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Range Character")
	FName ProjectileSpawnSocket;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Range Character")
	TSubclassOf<AProjectileBase> Projectile;

	UPROPERTY(BlueprintReadWrite, Category="Range Character")
	TSubclassOf<AProjectileBase> DefaultProjectile;

	UPROPERTY( BlueprintReadWrite, Category="Range Character")
	AProjectileBase* CurrentProjectile;

	UPROPERTY( BlueprintReadWrite, Category="Range Character")
	ACharacterBase* CurrentTargetCharacter;
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Range Character|Delegate")
	FOnProjectileHit_RangeCharacter OnProjectileHit;
};
