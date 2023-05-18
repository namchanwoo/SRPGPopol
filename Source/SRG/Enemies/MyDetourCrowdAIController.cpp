// Fill out your copyright notice in the Description page of Project Settings.


#include "SRG/Enemies/MyDetourCrowdAIController.h"

#include "Navigation/CrowdFollowingComponent.h"


AMyDetourCrowdAIController::AMyDetourCrowdAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}
