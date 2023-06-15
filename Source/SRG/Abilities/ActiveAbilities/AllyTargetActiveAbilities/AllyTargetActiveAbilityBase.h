#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "AllyTargetActiveAbilityBase.generated.h"

/*
 * 'AAllyTargetActiveAbilityBase' 클래스는 그리드 기반 SRPG(Strategy Role-Playing Game)에서 아군을 대상으로 하는 활성 능력을 나타냅니다.
 * 'AActiveAbilityBase' 클래스를 확장하여 아군 중에서 대상을 선택하고, 능력을 활성화 및 비활성화하고, 범위 내 슬롯을 모으는 추가 기능을 제공합니다.
 *  이 기능에는 AI 제어 엔티티에 대한 대상 슬롯 선택 메커니즘도 포함됩니다.
 */
UCLASS()
class SRG_API AAllyTargetActiveAbilityBase : public AActiveAbilityBase
{
	GENERATED_BODY()

public:
	AAllyTargetActiveAbilityBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


	virtual void OnActiveAbilityEnabled() override;
	virtual void OnActiveAbilityDisabled() override;
	virtual void SetTargetSlot(ASlotBase* InSlot) override;
	virtual void SetTargetSlotForAI_Implementation() override;
	virtual void ResetTargetSlot() override;

	virtual void GetAbilitySlots_Implementation(TArray<ASlotBase*>& OutSlotsInRange,
	                                            TArray<ASlotBase*>& OutEmptySlotsInRange,
	                                            TArray<ACharacterBase*>& OutAlliesInRange,
	                                            TArray<ACharacterBase*>& OutEnemiesInRange) override;
	

	virtual bool IsAvailable(bool IsMeleeAttack) override;

public:
	UPROPERTY(BlueprintReadWrite, Category="Ally Target Active Ability")
	ASlotBase* TargetSlot;
};
