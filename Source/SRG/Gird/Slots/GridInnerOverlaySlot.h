
#pragma once

#include "CoreMinimal.h"
#include "SlotBase.h"
#include "GridInnerOverlaySlot.generated.h"

/*
 * AGridInnerOverlaySlot 클래스는 속성과 메서드를 상속하는 ASlotBase 클래스의 자식입니다.
 * "그리드 내부 오버레이 슬롯"으로 알려진 게임의 특정 유형의 슬롯을 나타냅니다.
 * 그리드 내부 오버레이 슬롯은 특히 내부 슬롯을 위해 게임 그리드에 시각적 오버레이를 제공하는 데 사용됩니다.
 * 일반적으로 유닛의 이동 또는 공격 범위와 같은 그리드의 특정 영역을 강조 표시하는 데 사용되지만 내부 슬롯에만 사용됩니다.
 * 기본적으로 그리드 내부 오버레이 슬롯에는 스프라이트가 없지만 기본 슬롯보다 약간 더 높게(Z = 1.1) 위치합니다.
 * 이렇게 하면 오버레이가 기본 슬롯 및 기타 오버레이 위에 나타날 수 있습니다.
 * AGridInnerOverlaySlot 클래스는 주로 게임에서 그리드 내부 오버레이 슬롯의 시각적 표현 및 동작을 처리하는 데 사용됩니다.
 */

UCLASS()
class SRG_API AGridInnerOverlaySlot : public ASlotBase
{
	GENERATED_BODY()

public:
	AGridInnerOverlaySlot();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
