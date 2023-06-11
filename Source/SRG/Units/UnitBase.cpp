#include "UnitBase.h"

#include "Kismet/KismetMathLibrary.h"


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

bool AUnitBase::IsEvenSizedUnit()
{
	//ToDo::결과를 봐야함
	double remainder;
	int32 modValue = UKismetMathLibrary::FMod(UnitSize, 2.0f, remainder);
	return FMath::IsNearlyEqual(remainder, 0.0f);
}
