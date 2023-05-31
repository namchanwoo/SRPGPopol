// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "RangeCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileHit);

UCLASS()
class SRG_API ARangeCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARangeCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void RangeAttack(ACharacterBase* InTargetCharacter);
	void AbilityAttack(ACharacterBase* InCurrentTargetCharacter, UAnimMontage* MontageToPlay);

	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Range Character|Delegate")
	FOnProjectileHit OnProjectileHit;
};
