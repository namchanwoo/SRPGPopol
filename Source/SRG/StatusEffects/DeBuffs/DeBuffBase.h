#pragma once

#include "CoreMinimal.h"
#include "SRG/StatusEffects/StatusEffectBase.h"
#include "DeBuffBase.generated.h"

/**
 * @class ADeBuffBase
 * @brief SRG_API ADeBuffBase 클래스는 AStatusEffectBase 클래스를 상속받는 클래스입니다.
 * 이 클래스는 상태 효과 중 "디버프"를 관리하는 클래스입니다. 디버프는 캐릭터에게 일시적인 능력치 감소나 특정 제약을 부과하는 상태 효과를 말합니다.
 * 디버프는 일반적으로 부정적인 효과를 가지며, 이는 적 캐릭터를 약화시키거나 제한하여 전투에서 우위를 얻는데 사용됩니다.
 * ADeBuffBase 클래스는 AStatusEffectBase를 확장하여 디버프의 고유한 속성과 동작을 추가합니다.
 */
UCLASS()
class SRG_API ADeBuffBase : public AStatusEffectBase
{
	GENERATED_BODY()

public:
	ADeBuffBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void RemoveFromCharacter() override;
};
