#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "EnemyTargetActiveAbilityBase.generated.h"

class AMultiTargetPassiveAbilityBase;

/*
 * AEnemyTargetActiveAbilityBase는 게임에서 적을 대상으로 하는 특정 유형의 활성 능력을 나타냅니다.
 * AActiveAbilityBase 클래스를 상속하고 이를 확장하여 적을 주요 대상으로 하는 능력에 맞게 특별히 조정된 기능을 제공합니다.
 */
UCLASS()
class SRG_API AEnemyTargetActiveAbilityBase : public AActiveAbilityBase
{
	GENERATED_BODY()

public:
	AEnemyTargetActiveAbilityBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual bool IsAvailable(bool IsMeleeAttack) override;
	
	virtual void OnActiveAbilityEnabled() override;
	virtual void OnActiveAbilityDisabled() override;
	virtual void PlayAbilityAnimation() override;
	virtual void ApplyStatusEffects(const TArray<ACharacterBase*>& AllyCharacters,
	                                const TArray<ACharacterBase*>& EnemyCharacters, bool IsBeforeAttack) override;


	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Target Active Ability")
	bool bIsMelee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Target Active Ability")
	TSubclassOf<AMultiTargetPassiveAbilityBase> MultiTargetAbility;

	UPROPERTY(BlueprintReadWrite, Category="Enemy Target Active Ability")
	AMultiTargetPassiveAbilityBase* CurrentMultiTargetAbility;

	UPROPERTY(BlueprintReadWrite, Category="Enemy Target Active Ability")
	AMultiTargetPassiveAbilityBase* DefaultMultiTargetAbility;
};
