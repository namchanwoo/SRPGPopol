// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveAbilityBase.h"
#include "AllyTargetActiveAbilityBase.generated.h"

UCLASS()
class SRG_API AAllyTargetActiveAbilityBase : public AActiveAbilityBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAllyTargetActiveAbilityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	ASlotBase* TargetSlot;
};
