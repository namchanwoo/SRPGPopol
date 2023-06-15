#include "AbilityBase.h"

#include "SRG/Units/Characters/CharacterBase.h"
#include "SRGCore/Utilities/SRGLog.h"


AAbilityBase::AAbilityBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
}

void AAbilityBase::BeginPlay()
{
	Super::BeginPlay();

	SRPG_IF_CHECK(AbilityOwner, TEXT("AbilityOwner가 유효하지 않습니다."));

	AbilityOwner->OnDead.AddDynamic(this, &AAbilityBase::OnCharacterDies);
}

void AAbilityBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (AbilityOwner)
		AbilityOwner->OnDead.RemoveDynamic(this, &AAbilityBase::OnCharacterDies);
	
}

void AAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbilityBase::OnAbilityUsed_Implementation(const TArray<ASlotBase*>& InSlotsInRange,
                                                const TArray<ASlotBase*>& InEmptySlotsInRange, const TArray<ACharacterBase*>& InAlliesInRange,
                                                const TArray<ACharacterBase*>& InEnemiesInRange)
{
}



void AAbilityBase::OnCharacterDies(ACharacterBase* InCharacter)
{
	Destroy();
}
