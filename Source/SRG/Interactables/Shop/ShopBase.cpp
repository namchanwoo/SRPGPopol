// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopBase.h"


// Sets default values
AShopBase::AShopBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShopBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShopBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShopBase::LoadRecruitShop(const TArray<FPlayerCharacterData>& RecruitCharacterList)
{
}

void AShopBase::LoadEquipmentShip(const TArray<TSubclassOf<AEquipmentBase>>& EquipmentList)
{
}
