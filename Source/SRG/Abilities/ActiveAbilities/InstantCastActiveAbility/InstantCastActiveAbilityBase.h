
#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "InstantCastActiveAbilityBase.generated.h"

/*
 * AInstantCastActiveAbilityBase는 게임에서 즉시 시전 유형의 능력을 나타내는 클래스입니다.
 * 이 클래스는 AActiveAbilityBase의 파생 클래스로 모든 속성과 동작을 상속합니다. 
 * 활성화와 효과 실행 사이에 지연 없이 게임에서 즉시 활성화되는 능력의 특징을 지정합니다. 
 * 활성 능력의 특화로 기본 기능을 확장하여 즉각적인 능력 행동을 지원합니다.
 */
UCLASS()
class SRG_API AInstantCastActiveAbilityBase : public AActiveAbilityBase
{
	GENERATED_BODY()

public:
	AInstantCastActiveAbilityBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnActiveAbilityEnabled() override;
	
};
