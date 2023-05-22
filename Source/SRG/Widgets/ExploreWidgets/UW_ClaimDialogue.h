
#pragma once

#include "CoreMinimal.h"
#include "UW_ExploreWidget.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "UW_ClaimDialogue.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClaimClicked);

/**
 * 구현해야함
 */
UCLASS()
class SRG_API UUW_ClaimDialogue : public UUW_ExploreWidget
{
	GENERATED_BODY()
public:
	/*---	      	     ClaimDialogue   	      	---*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ClaimDialogue")
	AExploreHeroBase* ExploreHero;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ClaimDialogue")
	FText DialogueTitle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ClaimDialogue")
	FText YesButtonText;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ClaimDialogue")
	TArray<FPlayerCharacterData> Characters;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ClaimDialogue")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ClaimDialogue")
	int32 Gold;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ClaimDialogue")
	int32 Exp;

	UPROPERTY(BlueprintCallable,BlueprintAssignable,Category="ClaimDialogue|Delegate")
	FOnClaimClicked OnClaimClicked;
};
