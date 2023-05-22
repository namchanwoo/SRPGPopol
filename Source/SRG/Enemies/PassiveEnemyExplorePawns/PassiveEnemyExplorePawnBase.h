#pragma once

#include "CoreMinimal.h"
#include "SRG/Enemies/EnemyExplorePawnBase.h"
#include "PassiveEnemyExplorePawnBase.generated.h"

/**
 * @brief 수동 적 탐색 폰의 기본 클래스입니다.
 * 이 클래스는 게임에서 수동 적 탐색 폰의 기본 클래스 역할을 합니다.
 * 이는 AEnemyExplorePawnBase에서 상속되며 수동적 적에 특정한 추가 기능을 제공합니다.
 * 수동적인 적은 공격적이지 않으며 도발하지 않는 한 전투에 참여하지 않습니다.
 */
UCLASS()
class SRG_API APassiveEnemyExplorePawnBase : public AEnemyExplorePawnBase
{
	GENERATED_BODY()

public:
	APassiveEnemyExplorePawnBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
