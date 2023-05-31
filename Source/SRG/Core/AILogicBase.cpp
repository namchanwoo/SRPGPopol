// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogicBase.h"


// Sets default values
AAILogicBase::AAILogicBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAILogicBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAILogicBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAILogicBase::InitializeEvent(ABattleController* InBattleController, AGrid* InGrid)
{
}

void AAILogicBase::RunAILogic(ACharacterBase* CharacterBase)
{
}

