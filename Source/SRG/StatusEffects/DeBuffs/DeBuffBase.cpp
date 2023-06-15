#include "DeBuffBase.h"

#include "SRG/Units/Characters/CharacterBase.h"

ADeBuffBase::ADeBuffBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled=false;
}

void ADeBuffBase::BeginPlay()
{
	Super::BeginPlay();
}

void ADeBuffBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeBuffBase::RemoveFromCharacter()
{
	AffectedCharacter->RemoveEffect(this,AffectedCharacter->CurrentDeBuffs);
}
