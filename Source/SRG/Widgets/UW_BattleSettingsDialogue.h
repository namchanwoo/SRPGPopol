
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_BattleSettingsDialogue.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContinueClicked_BattleSetting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitClicked);
/**
 * 
 */
UCLASS()
class SRG_API UUW_BattleSettingsDialogue : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Battle Setting Dialogue|Delegate")
	FOnContinueClicked_BattleSetting OnContinueClicked_BattleSetting;
	
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Battle Setting Dialogue|Delegate")
	FOnQuitClicked OnQuitClicked;
};
