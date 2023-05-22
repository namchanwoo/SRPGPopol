#include "PassiveEnemyExplorePawnBase.h"


APassiveEnemyExplorePawnBase::APassiveEnemyExplorePawnBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APassiveEnemyExplorePawnBase::BeginPlay()
{
	Super::BeginPlay();
}

void APassiveEnemyExplorePawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
