// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "EnemyTargetActiveAbilityBase.generated.h"

UCLASS()
class SRG_API AEnemyTargetActiveAbilityBase : public AActiveAbilityBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyTargetActiveAbilityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
