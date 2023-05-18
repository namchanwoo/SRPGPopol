// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleHeroBase.generated.h"

class AEquipmentBase;
class ABattleController;
UCLASS()
class SRG_API ABattleHeroBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABattleHeroBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region   	Field Members
	UPROPERTY(BlueprintReadOnly, Category="Battle Hero")
	ABattleController* BattleController;

	UPROPERTY(BlueprintReadOnly, Category="Battle Hero")
	int32 Gold;

	UPROPERTY(BlueprintReadOnly, Category="Battle Hero")
	int32 CurrentMana;

	UPROPERTY(BlueprintReadOnly, Category="Battle Hero")
	TArray<TSubclassOf<AEquipmentBase>> BackPack;

	UPROPERTY(BlueprintReadOnly, Category="Battle Hero")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;


#pragma endregion	Field Members
};
