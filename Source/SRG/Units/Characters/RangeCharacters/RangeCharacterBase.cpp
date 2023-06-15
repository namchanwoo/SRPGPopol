#include "RangeCharacterBase.h"

#include "SRG/Actors/Projectiles/ProjectileBase.h"
#include "SRGCore/Libraries/SRPGUtilityLibrary.h"
#include "SRGCore/Utilities/SRGLog.h"


ARangeCharacterBase::ARangeCharacterBase()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다. 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = true;
}

void ARangeCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARangeCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARangeCharacterBase::RangeAttack(ACharacterBase* InTargetCharacter)
{
	CurrentTargetCharacter = InTargetCharacter;
	OnAttacked.AddDynamic(this, &ARangeCharacterBase::OnRangeAttackHandler);
	PlayAnimationMontage(USRPGUtilityLibrary::GetRandomElement<UAnimMontage*>(RangeAttackMontages));
}

void ARangeCharacterBase::AbilityAttack(ACharacterBase* InCurrentTargetCharacter, UAnimMontage* MontageToPlay)
{
	CurrentTargetCharacter = InCurrentTargetCharacter;

	OnAbilityUsed.AddDynamic(this, &ARangeCharacterBase::OnRangeAttackAbilityHandler);

	PlayAnimationMontage(MontageToPlay);
}

void ARangeCharacterBase::ChangeProjectile(TSubclassOf<AProjectileBase> NewProjectile)
{
	DefaultProjectile = Projectile;
	Projectile = NewProjectile;
}

void ARangeCharacterBase::ResetProjectile()
{
	Projectile = DefaultProjectile;
}

FVector ARangeCharacterBase::GetProjectileSpawnLocation()
{
	return SkeletalMesh->GetSocketLocation(ProjectileSpawnSocket);
}

void ARangeCharacterBase::OnProjectileHitHandler()
{
	if (OnProjectileHit.IsBound())
		OnProjectileHit.Broadcast();
}

void ARangeCharacterBase::OnRangeAttackHandler()
{
	OnAttacked.RemoveDynamic(this, &ARangeCharacterBase::OnRangeAttackHandler);
	ApplyStats(FCharacterStats(0, 0, 0, 0, 0, 0, -1, 0));

	if (IsValid(Projectile))
	{
		UWorld* World = GetWorld();
		SRPG_CHECK(World);

		const FActorSpawnParameters SpawnParams;
		const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, GetProjectileSpawnLocation());
		CurrentProjectile = World->SpawnActor<AProjectileBase>(Projectile, SpawnTransform, SpawnParams);
		
		CurrentProjectile->OnProjectileHit.AddDynamic(this,&ARangeCharacterBase::OnProjectileHitHandler);
		CurrentProjectile->MoveProjectileToTarget(CurrentTargetCharacter->ProjectileHitLocation->GetComponentLocation());
	}
	else
	{
		if (OnProjectileHit.IsBound())
			OnProjectileHit.Broadcast();
	}
}

void ARangeCharacterBase::OnRangeAttackAbilityHandler()
{
	OnAbilityUsed.RemoveDynamic(this, &ARangeCharacterBase::OnRangeAttackAbilityHandler);
	OnRangeAttackHandler();
}
