#include "DeBuffBase.h"

#include "SRG/Units/Characters/CharacterBase.h"

ADeBuffBase::ADeBuffBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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
