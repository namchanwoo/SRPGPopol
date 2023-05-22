#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "InteractionDetector.generated.h"

class UUW_InteractionUI;
class UWidgetComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, int32, InButtonIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionAppeared);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionDisappeared);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoaded, bool, bShouldLoadEnemyPawns);

UCLASS()
class SRG_API AInteractionDetector : public AActor
{
	GENERATED_BODY()

public:
	AInteractionDetector();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;


#pragma region   	Interaction Event

public:
	void SetExploreHero(AExploreHeroBase* InExploreHero);

	void EnableInteraction();

	void DisableInteraction();

	void Load(bool IsShouldLoadEnemyPawns);
#pragma endregion	Interaction Event


#pragma region   	Widget Event

public:
	void ShowInteractionUI();

	void HideInteractionUI();

#pragma endregion	Widget Event


#pragma region   	Delegate On Function

public:
	UFUNCTION()
	void OnInteractClicked_InteractionUI(int32 ButtonIndex);

#pragma endregion	Delegate On Function


#pragma region   	Field Members

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction Detector|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction Detector|Component")
	USphereComponent* InteractionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction Detector|Component")
	UWidgetComponent* InteractionWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Detector")
	bool bCanInteract = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Detector")
	bool bIsForceInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Detector")
	float InteractionRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Detector")
	FVector InteractionUILocation;

	UPROPERTY(BlueprintReadWrite, Category="Interaction Detector")
	AExploreHeroBase* ExploreHero;

	UPROPERTY(BlueprintReadWrite, Category="Interaction Detector")
	UUW_InteractionUI* InteractionUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Detector")
	TArray<FText> InteractionTexts;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction Detector|Delegate")
	FOnInteracted OnInteracted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction Detector|Delegate")
	FOnInteractionAppeared OnInteractionAppeared;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction Detector|Delegate")
	FOnInteractionDisappeared OnInteractionDisappeared;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction Detector|Delegate")
	FOnLoaded OnLoaded;

#pragma endregion	Field Members
};
