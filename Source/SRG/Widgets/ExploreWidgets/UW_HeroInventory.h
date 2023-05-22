#pragma once

#include "CoreMinimal.h"
#include "UW_ExploreWidget.h"
#include "UW_HeroInventory.generated.h"



/**
 * 
 */
UCLASS()
class SRG_API UUW_HeroInventory : public UUW_ExploreWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Hero Inventory|Delegate")
	FOnOkClicked OnOkClicked;
};
