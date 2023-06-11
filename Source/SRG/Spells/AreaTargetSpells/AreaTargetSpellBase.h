// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SRG/Spells/SpellBase.h"
#include "AreaTargetSpellBase.generated.h"

UCLASS()
class SRG_API AAreaTargetSpellBase : public ASpellBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAreaTargetSpellBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	bool bHighlightEnemies;
	UPROPERTY()
	bool bHighlightAllies;
	UPROPERTY()
	int32 Range;
	UPROPERTY()
	ASlotBase* TargetSlot;
};
