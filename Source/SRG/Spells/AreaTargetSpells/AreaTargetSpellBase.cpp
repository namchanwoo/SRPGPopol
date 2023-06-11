// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaTargetSpellBase.h"


// Sets default values
AAreaTargetSpellBase::AAreaTargetSpellBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAreaTargetSpellBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAreaTargetSpellBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

