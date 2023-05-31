

#include "BattleHeroBase.h"



ABattleHeroBase::ABattleHeroBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


void ABattleHeroBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABattleHeroBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABattleHeroBase::InitializeEvent()
{
}

void ABattleHeroBase::AddDrops(int32 NewGold, const TArray<TSubclassOf<AEquipmentBase>>& NewDrops)
{
}

