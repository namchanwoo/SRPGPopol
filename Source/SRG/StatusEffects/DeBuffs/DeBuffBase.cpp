// Fill out your copyright notice in the Description page of Project Settings.


#include "DeBuffBase.h"


// Sets default values
ADeBuffBase::ADeBuffBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADeBuffBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeBuffBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

