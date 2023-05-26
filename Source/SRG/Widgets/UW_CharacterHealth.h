
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CharacterHealth.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class SRG_API UUW_CharacterHealth : public UUserWidget
{
	GENERATED_BODY()
public:
	
	void InitializeEvent(ACharacterBase* NewCharacterBase);
};
