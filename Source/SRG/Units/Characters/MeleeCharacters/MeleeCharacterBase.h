#pragma once

#include "CoreMinimal.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "MeleeCharacterBase.generated.h"

/*
 * AMeleeCharacterBase 클래스는 근접 전투에 참여하는 캐릭터를 나타냅니다.
 * 이 클래스는 기본 캐릭터 클래스(ACharacterBase)의 파생물이며 일부 기능을 재정의하여 근접 전투에 대한 특정 동작을 제공합니다.
 * 
 * 모든 프레임에서 호출되는 Tick 함수는 공격 기회를 확인하거나 캐릭터의 움직임을 관리하는 것과 같은 근접전 관련 로직을 처리하는 데 사용할 수 있습니다.
 * 
 * BeginPlay 함수는 일반적으로 초기화에 사용됩니다.
 * 근접 캐릭터의 경우 여기에는 무기 또는 방어 통계 설정 또는 세계에서 캐릭터의 위치 지정이 포함될 수 있습니다.
 * 이 클래스는 UCLASS 매크로로 표시되어 있어 언리얼 엔진 에디터에서 청사진으로 사용할 수 있습니다.
 */
UCLASS()
class SRG_API AMeleeCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AMeleeCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
