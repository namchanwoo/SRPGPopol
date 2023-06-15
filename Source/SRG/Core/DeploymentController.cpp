// Fill out your copyright notice in the Description page of Project Settings.


#include "DeploymentController.h"


// Sets default values
ADeploymentController::ADeploymentController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADeploymentController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeploymentController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeploymentController::InitializeEvent()
{
}

void ADeploymentController::SetNonDeployedCharacters()
{
}

void ADeploymentController::OnDragEnded()
{
	
}

