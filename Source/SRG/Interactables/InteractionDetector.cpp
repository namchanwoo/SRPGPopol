// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionDetector.h"


// Sets default values
AInteractionDetector::AInteractionDetector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteractionDetector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractionDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractionDetector::SetExploreHero(AExploreHeroBase* ExploreHeroBase)
{
}

void AInteractionDetector::ShowInteractionUI()
{
}

void AInteractionDetector::OnInteractButtonClicked(int ButtonIndex)
{
}

void AInteractionDetector::HideInteractionUI()
{
}

