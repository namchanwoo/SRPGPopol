﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellBase.h"


// Sets default values
ASpellBase::ASpellBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpellBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpellBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpellBase::OnSpellEnabled()
{
}

void ASpellBase::OnSpellDisabled()
{
}

TMap<ACharacterBase*, FDamageData> ASpellBase::GetSpellDamage(TArray<ACharacterBase*> InTargetCharacters)
{
	return {};
}

void ASpellBase::PlaySpellAnimation()
{
}

void ASpellBase::SetTargetSlot(ASlotBase* InSlot)
{
}

void ASpellBase::GetSpellSlots(TArray<ASlotBase*>& OutSlotsInRange, TArray<ASlotBase*>& OutEmptySlots,
	TArray<ACharacterBase*>& OutAllies, TArray<ACharacterBase*>& OutEnemies)
{
}
