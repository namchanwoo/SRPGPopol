#include "BuffBase.h"

#include "SRG/Units/Characters/CharacterBase.h"
#include "SRGCore/SRGLog.h"


ABuffBase::ABuffBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ABuffBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABuffBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABuffBase::RemoveFromCharacter()
{
	SRPG_CHECK(AffectedCharacter);
	AffectedCharacter->RemoveEffect(this,AffectedCharacter->CurrentBuffs);
}
