// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveAbilityBase.h"


// Sets default values
AActiveAbilityBase::AActiveAbilityBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AActiveAbilityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActiveAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

