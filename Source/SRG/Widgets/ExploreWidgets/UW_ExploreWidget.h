
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ExploreWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContinueClicked);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOkClicked);
/**
 * 
 */
UCLASS()
class SRG_API UUW_ExploreWidget : public UUserWidget
{
	GENERATED_BODY()
};
