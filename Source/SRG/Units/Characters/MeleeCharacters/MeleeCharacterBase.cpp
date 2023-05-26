#include "MeleeCharacterBase.h"


AMeleeCharacterBase::AMeleeCharacterBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMeleeCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
