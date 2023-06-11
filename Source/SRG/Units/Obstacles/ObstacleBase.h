
#pragma once

#include "CoreMinimal.h"
#include "SRG/Units/UnitBase.h"
#include "ObstacleBase.generated.h"

/**
 * 
 */
UCLASS()
class SRG_API AObstacleBase : public AUnitBase
{
	GENERATED_BODY()
public:

	bool bRandomRotation;
};
