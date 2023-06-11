#include "BattleHeroBase.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "SRG/Core/BattleController.h"
#include "SRG/Equipment/EquipmentBase.h"
#include "SRG/Libraries/SRPGFunctionLibrary.h"
#include "SRG/Spells/SpellBase.h"
#include "SRGCore/Libraries/SRPGConstants.h"
#include "SRGCore/Utilities/SRGLog.h"


ABattleHeroBase::ABattleHeroBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);


	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh_GEN_VARIABLE"));
	SkeletalMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SkeletalMesh->SetupAttachment(DefaultSceneRoot);


	// Create the Box Component and attach it to RootComponent
	RightClickDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("RightClickDetector_GEN_VARIABLE"));
	RightClickDetector->SetupAttachment(RootComponent);

	// Set the AreaClass
	RightClickDetector->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());

	RightClickDetector->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightClickDetector->BodyInstance.bNotifyRigidBodyCollision = true;
	RightClickDetector->BodyInstance.SetCollisionProfileName("Custom");

	FCollisionResponseContainer responseContainer;
	responseContainer.SetAllChannels(ECR_Overlap);
	RightClickDetector->BodyInstance.SetResponseToChannels(responseContainer);
	RightClickDetector->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	RightClickDetector->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	RightClickDetector->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	RightClickDetector->SetRelativeScale3D(FVector(1.f, 1.f, 2.75f));


	if (UAnimInstance* AnimInstance = SkeletalMesh ? SkeletalMesh->GetAnimInstance() : nullptr)
	{
		AnimInstance->OnPlayMontageNotifyBegin.Clear();
		AnimInstance->OnMontageEnded.Clear();
	}
}

void ABattleHeroBase::BeginPlay()
{
	Super::BeginPlay();

	RightClickDetector->OnClicked.AddDynamic(this, &ABattleHeroBase::OnClickedHandler_RightClickDetector);
}

void ABattleHeroBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABattleHeroBase::InitializeEvent()
{
	FHeroStats EquipmentHeroStats = USRPGFunctionLibrary::GetEquipmentStats(this, Equipment);
	HeroStats = HeroStats + EquipmentHeroStats;
	MaxMana = HeroStats.Knowledge * 10;

	SetMana(CurrentMana);
	SetSpells();

	BattleController->OnNewTurn.AddDynamic(this, &ABattleHeroBase::OnNewTurnHandler);
}

void ABattleHeroBase::SetMana(int32 NewMana)
{
	CurrentMana = NewMana;

	if (OnManaSet.IsBound())
		OnManaSet.Broadcast(CurrentMana);
}

void ABattleHeroBase::SetSpells()
{
	TArray<TSubclassOf<ASpellBase>> availableSpells;

	for (TTuple<TSubclassOf<ASpellBase>, int> SpellPair : Spells)
	{
		if (SpellPair.Value <= Level)
		{
			availableSpells.AddUnique(SpellPair.Key);
		}
	}

	for (TSubclassOf<AEquipmentBase> EquipmentClass : Equipment)
	{
		AEquipmentBase* EquipmentCDO = EquipmentClass->GetDefaultObject<AEquipmentBase>();

		for (TSubclassOf<ASpellBase> InSpell : EquipmentCDO->Spells)
		{
			availableSpells.AddUnique(InSpell);
		}
	}

	for (TSubclassOf<ASpellBase> AvailableSpellClass : availableSpells)
	{
		UWorld* World = GetWorld();
		SRPG_CHECK(World);

		FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector(99999.0f));
		if (ASpellBase* NewSpell = World->SpawnActorDeferred<ASpellBase>(AvailableSpellClass, SpawnTransform))
		{
			NewSpell->BattleController = BattleController;
			NewSpell->Hero = this;
			UGameplayStatics::FinishSpawningActor(NewSpell, SpawnTransform);
			CurrentSpells.Add(NewSpell);
		}
	}
}

void ABattleHeroBase::OnSpellUsed(int32 InManaCost)
{
	bCanUseSpell = false;
	SetMana(CurrentMana - InManaCost);
}

void ABattleHeroBase::AddDrops(int32 NewGold, const TArray<TSubclassOf<AEquipmentBase>>& NewDrops)
{
	Gold = NewGold + Gold;
	BackPack.Append(NewDrops);
}

void ABattleHeroBase::PlayAnimationMontage(UAnimMontage* MontageToPlay)
{
	if (IsValid(MontageToPlay))
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
		if (IsValid(AnimInstance))
		{
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ABattleHeroBase::OnPlayMontageNotifyBeginHandler);
			AnimInstance->OnMontageEnded.AddDynamic(this, &ABattleHeroBase::OnMontageEndedHandler);
			AnimInstance->Montage_Play(MontageToPlay);
		}
	}
}

void ABattleHeroBase::ClearSpellCallback()
{
	OnAnimationEnded.RemoveAll(this);
	OnSpellCasted.RemoveAll(this);
}

FVector ABattleHeroBase::GetProjectileSpawnLocation()
{
	return SkeletalMesh->GetSocketLocation(ProjectileSpawnSocket);
}

void ABattleHeroBase::OnNewTurnHandler(int32 NewTurn)
{
	bCanUseSpell = true;
}

void ABattleHeroBase::OnClickedHandler_RightClickDetector(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::RightMouseButton)
	{
		if (OnRightClicked.IsBound())
		{
			OnRightClicked.Broadcast();
		}
	}
}

void ABattleHeroBase::OnPlayMontageNotifyBeginHandler(FName NotifyName,
                                                      const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == USRPGConstants::UseSpell)
	{
		if (OnSpellCasted.IsBound())
		{
			OnSpellCasted.Broadcast();
		}
	}
}

void ABattleHeroBase::OnMontageEndedHandler(UAnimMontage* Montage, bool bInterrupted)
{
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ABattleHeroBase::OnPlayMontageNotifyBeginHandler);
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABattleHeroBase::OnMontageEndedHandler);
	}

	// Rest of your code...
	if (OnAnimationEnded.IsBound())
	{
		OnAnimationEnded.Broadcast();
	}
}
