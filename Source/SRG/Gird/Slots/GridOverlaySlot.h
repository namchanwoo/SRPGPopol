
#pragma once

#include "CoreMinimal.h"
#include "SlotBase.h"
#include "GridOverlaySlot.generated.h"

/*
 * AGridOverlaySlot 클래스는 속성과 메서드를 상속하는 ASlotBase 클래스의 자식입니다.
 * "그리드 오버레이 슬롯"으로 알려진 게임의 특정 유형의 슬롯을 나타냅니다.
 * 그리드 오버레이 슬롯은 게임 그리드에 시각적 오버레이를 제공하는 데 사용됩니다.
 * 일반적으로 유닛의 이동 또는 공격 범위와 같은 그리드의 특정 영역을 강조 표시하는 데 사용됩니다.
 * 기본적으로 그리드 오버레이 슬롯에는 스프라이트가 없지만 스프라이트 색상은 흰색으로 설정되고 기본 슬롯보다 약간 높게(Z = 1.0) 배치됩니다.
 * 이렇게 하면 오버레이가 베이스 슬롯 위에 나타날 수 있습니다.
 * AGridOverlaySlot 클래스는 주로 게임에서 그리드 오버레이 슬롯의 시각적 표현 및 동작을 처리하는 데 사용됩니다.
 */
UCLASS()
class SRG_API AGridOverlaySlot : public ASlotBase
{
	GENERATED_BODY()

public:
	AGridOverlaySlot();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
