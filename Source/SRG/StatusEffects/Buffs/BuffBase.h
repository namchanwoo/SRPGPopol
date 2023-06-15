#pragma once

#include "CoreMinimal.h"
#include "SRG/StatusEffects/StatusEffectBase.h"
#include "BuffBase.generated.h"

/**
 * SRG_API ABuffBase 클래스는 AStatusEffectBase 클래스를 상속받는 클래스입니다.
 * 이 클래스는 상태 효과 중 "버프"를 관리하는 클래스입니다. 버프는 캐릭터의 일시적인 능력치 상승이나 특별한 이점을 주는 상태 효과를 말합니다. 
 * 따라서 버프는 일반적으로 긍정적인 효과를 가지며, 주로 캐릭터가 전투에서 우위를 얻을 수 있도록 돕습니다.
 * ABuffBase 클래스는 AStatusEffectBase를 확장하여 버프의 고유한 속성과 동작을 추가합니다.
 */

UCLASS()
class SRG_API ABuffBase : public AStatusEffectBase
{
	GENERATED_BODY()

public:
	ABuffBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void RemoveFromCharacter() override;

	/*******************************************
	 * Field Members
	 *******************************************/
public:

};
