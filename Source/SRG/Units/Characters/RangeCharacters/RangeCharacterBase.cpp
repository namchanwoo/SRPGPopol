// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeCharacterBase.h"


// Sets default values
ARangeCharacterBase::ARangeCharacterBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARangeCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARangeCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARangeCharacterBase::RangeAttack(ACharacterBase* InTargetCharacter)
{
}

void ARangeCharacterBase::AbilityAttack(ACharacterBase* InCurrentTargetCharacter, UAnimMontage* MontageToPlay)
{
}

