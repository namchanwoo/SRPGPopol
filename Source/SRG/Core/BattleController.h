// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleController.generated.h"

class ADeploymentController;
class ACharacterBase;
UCLASS()
class SRG_API ABattleController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABattleController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region   	Field Members

public:
	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	ADeploymentController* DeploymentController;
	
	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	TArray<ACharacterBase*> PlayerCharacters;
#pragma endregion	Field Members
};
