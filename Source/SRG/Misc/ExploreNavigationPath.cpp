// Fill out your copyright notice in the Description page of Project Settings.


#include "ExploreNavigationPath.h"


// Sets default values
AExploreNavigationPath::AExploreNavigationPath()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AExploreNavigationPath::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExploreNavigationPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExploreNavigationPath::ShowPath(const FVector& PathStart, const FVector& TargetLocation)
{
}

void AExploreNavigationPath::ClearPath()
{
}

