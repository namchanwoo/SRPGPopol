// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/AbilityBase.h"
#include "PassiveAbilityBase.generated.h"

UCLASS()
class SRG_API APassiveAbilityBase : public AAbilityBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APassiveAbilityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
