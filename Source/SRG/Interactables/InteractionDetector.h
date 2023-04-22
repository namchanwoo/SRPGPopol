// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "InteractionDetector.generated.h"

class USphereComponent;
UCLASS()
class SRG_API AInteractionDetector : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractionDetector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetExploreHero(AExploreHeroBase* ExploreHeroBase);
	
	void ShowInteractionUI();
	
	void OnInteractButtonClicked(int ButtonIndex);
	
	void HideInteractionUI();


#pragma region   	Field Members
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction Detector")
	bool bCanInteract = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction Detector")
	bool bIsForceInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction Detector")
	USphereComponent* InteractionSphere;

#pragma endregion	Field Members
};
