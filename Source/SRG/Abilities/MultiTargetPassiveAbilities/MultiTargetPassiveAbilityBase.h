
#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/AbilityBase.h"
#include "MultiTargetPassiveAbilityBase.generated.h"

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
};
