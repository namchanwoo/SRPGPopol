// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleController.h"
#include "GameFramework/Actor.h"
#include "AILogicBase.generated.h"

UCLASS()
class SRG_API AAILogicBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAILogicBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	void InitializeEvent(ABattleController* InBattleController, AGrid* InGrid);
	
	void RunAILogic(ACharacterBase* CharacterBase);
};
