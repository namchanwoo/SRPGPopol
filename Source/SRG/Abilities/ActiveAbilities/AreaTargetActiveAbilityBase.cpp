// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaTargetActiveAbilityBase.h"


// Sets default values
AAreaTargetActiveAbilityBase::AAreaTargetActiveAbilityBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAreaTargetActiveAbilityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAreaTargetActiveAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TMap<ACharacterBase*, FDamageData> AAreaTargetActiveAbilityBase::GetAbilityDamage(TArray<ACharacterBase*> Array)
{
	return {};
}

