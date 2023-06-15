#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/AbilityBase.h"
#include "MultiTargetPassiveAbilityBase.generated.h"

class ASlotBase;

/**
 * 이 클래스는 여러 대상에게 영향을 미치는 패시브 능력을 관리하는 데 사용됩니다.
 * 패시브 능력은 활성화되지 않아도 항상 효과가 적용되는 능력을 의미합니다.
 * 이러한 능력은 캐릭터가 특정 조건에 충족하거나 특정 이벤트가 발생할 때 자동으로 발동됩니다.
 * 또한, 여러 대상을 대상으로 할 수 있으며, 이는 'bIsMelee' 변수를 통해 근접 공격인지 여부를 결정할 수 있습니다.
 * AMultiTargetPassiveAbilityBase 클래스는 AAbilityBase를 확장하여 다중 대상 패시브 능력의 고유한 특성과 동작을 추가합니다.
 */
UCLASS()
class SRG_API AMultiTargetPassiveAbilityBase : public AAbilityBase
{
	GENERATED_BODY()

public:
	AMultiTargetPassiveAbilityBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Multi Target Passive Ability")
	TArray<ACharacterBase*> GetEnemiesInAbilityRange(ASlotBase* InAttackSlot, ASlotBase* InStandingSlot);
	TArray<ACharacterBase*> GetEnemiesInAbilityRange_Implementation(ASlotBase* InAttackSlot, ASlotBase* InStandingSlot);

	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Multi Target Passive Ability")
	bool bIsMelee = true;
};
