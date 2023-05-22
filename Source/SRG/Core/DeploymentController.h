﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/SRGEnumStruct.h"
#include "DeploymentController.generated.h"

UCLASS()
class SRG_API ADeploymentController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADeploymentController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region   	Field Members

public:
	UPROPERTY(BlueprintReadWrite, Category="DeploymentController")
	TArray<FPlayerCharacterData> NonDeployedCharacters;

#pragma endregion	Field Members
};