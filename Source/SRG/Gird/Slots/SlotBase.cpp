// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotBase.h"


// Sets default values
ASlotBase::ASlotBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASlotBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlotBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlotBase::ApplyAuraToContainedUnit()
{
}

void ASlotBase::AddAura(AAuraBase* InAura, ACharacterBase* InAuraOwner)
{
}

bool ASlotBase::IsEnemySlot(ACharacterBase* InCharacter, ACharacterBase*& EnemyCharacter)
{
	return false;
}

void ASlotBase::OnSpellDisabled()
{
}

