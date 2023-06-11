// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveAbilityBase.h"
#include "AreaTargetActiveAbilityBase.generated.h"

UCLASS()
class SRG_API AAreaTargetActiveAbilityBase : public AActiveAbilityBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAreaTargetActiveAbilityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	TMap<ACharacterBase*, FDamageData> GetAbilityDamage(TArray<ACharacterBase*> Array);


public:
	bool bHighlightAllies;
	bool bHighlightEnemies;
	UPROPERTY()
	ASlotBase* TargetSlot;
};
