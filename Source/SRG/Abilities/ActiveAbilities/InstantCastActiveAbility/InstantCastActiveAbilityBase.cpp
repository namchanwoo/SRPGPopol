#include "InstantCastActiveAbilityBase.h"

AInstantCastActiveAbilityBase::AInstantCastActiveAbilityBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AInstantCastActiveAbilityBase::BeginPlay()
{
	Super::BeginPlay();
}

void AInstantCastActiveAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInstantCastActiveAbilityBase::OnActiveAbilityEnabled()
{
	PlayAbilityAnimation();
}


