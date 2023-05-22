// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SRG/Quests/QuestBase.h"
#include "InteractionQuestBase.generated.h"

UCLASS()
class SRG_API AInteractionQuestBase : public AQuestBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractionQuestBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TArray<AActor*> InteractableActors;
};
