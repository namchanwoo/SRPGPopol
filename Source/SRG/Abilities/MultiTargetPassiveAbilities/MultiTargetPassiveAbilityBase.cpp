

#include "MultiTargetPassiveAbilityBase.h"



AMultiTargetPassiveAbilityBase::AMultiTargetPassiveAbilityBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMultiTargetPassiveAbilityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMultiTargetPassiveAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

