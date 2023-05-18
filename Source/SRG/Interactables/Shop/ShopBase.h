// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/SRGEnumStruct.h"
#include "ShopBase.generated.h"

UCLASS()
class SRG_API AShopBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShopBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void LoadRecruitShop(const TArray<FPlayerCharacterData>& RecruitCharacterList);
	void LoadEquipmentShip(const TArray<TSubclassOf<AEquipmentBase>>& EquipmentList);
};
