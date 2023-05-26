#include "UnitBase.h"


AUnitBase::AUnitBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();
}

void AUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnitBase::InitializeEvent()
{
	bInitialized = true;
}

void AUnitBase::SetUnitAnchorSlot(ASlotBase* InSlot)
{
	Slot = InSlot;
}

void AUnitBase::Highlight(bool IsOn)
{
}

void AUnitBase::OnMovedToSlot()
{
}

void AUnitBase::OnRemovedFromSlot()
{
}
