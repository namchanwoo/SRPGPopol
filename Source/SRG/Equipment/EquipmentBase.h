// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/SRGEnumStruct.h"
#include "EquipmentBase.generated.h"

UCLASS()
class SRG_API AEquipmentBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEquipmentBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


#pragma region   	Field Members

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	int32 Cost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	EEquipmenet Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	FHeroStats HeroStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	UTexture2D* Icon;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	// TArray<TSubclassOf<SpellBase>> Spells;

#pragma endregion	Field Members
};
