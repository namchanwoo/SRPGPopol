// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExploreNavigationPath.generated.h"

UCLASS()
class SRG_API AExploreNavigationPath : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExploreNavigationPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="ExploreNavigationPath")
	void ShowPath(const FVector& PathStart, const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, Category="ExploreNavigationPath")
	void ClearPath();
};
