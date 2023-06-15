#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "AreaTargetActiveAbilityBase.generated.h"

/*
 * AAreaTargetActiveAbilityBase 클래스는 AActiveAbilityBase의 하위 클래스입니다.
 * 이 클래스는 특히 단일 유닛이 아닌 지역을 대상으로 하는 게임의 능력을 처리합니다.
 * 능력이 활성화 또는 비활성화될 때 추가 동작을 정의하고 게임 그리드의 특정 슬롯을 대상으로 하며 능력의 효과 영역을 결정하여 기본 활성 능력을 확장합니다.
 * 이 클래스는 또한 능력을 사용해야 하는지 여부와 능력을 대상으로 지정할 위치를 결정하는 AI 동작을 지원합니다.
 */
UCLASS()
class SRG_API AAreaTargetActiveAbilityBase : public AActiveAbilityBase
{
	GENERATED_BODY()

public:
	AAreaTargetActiveAbilityBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


	virtual void OnActiveAbilityEnabled() override;
	virtual void OnActiveAbilityDisabled() override;
	virtual void SetTargetSlot(ASlotBase* InSlot) override;
	virtual void SetTargetSlotForAI_Implementation() override;
	virtual void ResetTargetSlot() override;

	virtual bool ShouldAIUse_Implementation(const TArray<ASlotBase*>& SlotsInRange,
	                                        const TArray<ASlotBase*>& EmptySlotsInRange,
	                                        const TArray<ACharacterBase*>& AlliesInRange,
	                                        const TArray<ACharacterBase*>& EnemiesInRange,
	                                        ACharacterBase* WeakestEnemy) override;

	virtual void GetAbilitySlots_Implementation(TArray<ASlotBase*>& OutSlotsInRange,
	                                            TArray<ASlotBase*>& OutEmptySlotsInRange,
	                                            TArray<ACharacterBase*>& OutAlliesInRange,
	                                            TArray<ACharacterBase*>& OutEnemiesInRange) override;

	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Area Target Active Ability")
	bool bHighlightAllies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Area Target Active Ability")
	bool bHighlightEnemies;

	UPROPERTY(BlueprintReadWrite, Category="Area Target Active Ability")
	ASlotBase* TargetSlot;
};
