
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SRPGTypes.generated.h"


/**
 * 
 */
UCLASS()
class SRGCORE_API USRPGTypes : public UObject
{
	GENERATED_BODY()
public:
	static FORCEINLINE ETraceTypeQuery ConvertToTraceTypeVisibility()
	{
		return UEngineTypes::ConvertToTraceType(ECC_Visibility);
	}

	static FORCEINLINE ETraceTypeQuery ConvertToTraceTypeCamera()
	{
		return UEngineTypes::ConvertToTraceType(ECC_Camera);
	}
	// 다른 함수들...
};
