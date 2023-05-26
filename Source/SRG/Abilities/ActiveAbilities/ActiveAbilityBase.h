// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/AbilityBase.h"
#include "ActiveAbilityBase.generated.h"

UCLASS()
class SRG_API AActiveAbilityBase : public AAbilityBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActiveAbilityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
