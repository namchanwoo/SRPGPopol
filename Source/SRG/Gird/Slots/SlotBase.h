// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRG/Abilities/Auras/AuraBase.h"
#include "SlotBase.generated.h"

class AUnitBase;
class ACharacterBase;


UCLASS()
class SRG_API ASlotBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlotBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyAuraToContainedUnit();

	void AddAura(AAuraBase* InAura, ACharacterBase* InAuraOwner);

	UFUNCTION(BlueprintPure, Category="Slot Event")
	bool IsEnemySlot(ACharacterBase* InCharacter, ACharacterBase*& EnemyCharacter);
	
	void OnSpellDisabled();

	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(BlueprintReadWrite, Category="Slot")
	FVector2D Coordinates;

	UPROPERTY(BlueprintReadWrite, Category="Slot")
	AUnitBase* ContainedUnit;
	
	UPROPERTY(BlueprintReadOnly, Category="Slot")
	TArray<ASlotBase*> Path;
};
