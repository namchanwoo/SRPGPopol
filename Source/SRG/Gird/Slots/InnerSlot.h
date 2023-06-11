
#pragma once

#include "CoreMinimal.h"
#include "Slot.h"
#include "InnerSlot.generated.h"

/*
 * AInnerSlot 클래스는 속성과 메서드를 상속하는 ASlot 클래스의 자식입니다.
 * "내부 슬롯"으로 알려진 게임의 특정 유형의 슬롯을 나타냅니다.
 * 내부 슬롯은 고유한 속성과 동작으로 구별됩니다.
 * 예를 들어 기본적으로 스프라이트가 없고 상자 구성 요소가 약간 더 낮게 위치하며(Z = -1.0) 내부 슬롯으로 표시됩니다(bIsInnerSlot = true).
 * 또한 스프라이트 크기가 0으로 설정되어 보이지 않게 됩니다.
 * AInnerSlot 클래스는 주로 게임의 내부 슬롯과 사용자 상호 작용을 처리하는 데 사용됩니다.
 */
UCLASS()
class SRG_API AInnerSlot : public ASlot
{
	GENERATED_BODY()

public:
	AInnerSlot();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void MouseOverSlot() override;
};
