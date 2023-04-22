#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExplorePlayerController.generated.h"

class UUW_ExploreCursor;
class UInputMappingContext;
// class UInputConfig;

/**
 * 
 */
UCLASS()
class SRG_API AExplorePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AExplorePlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;


#pragma region   	----------------------- Field Members -----------------------

	/*---	      	    Input    	      	---*/
public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ExplorePlayerController|Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ExplorePlayerController|Input", meta=(AllowPrivateAccess = "true"))
	// UInputConfig* DefaultInputConfig;

	/*---	      	    Reference    	      	---*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExplorePlayerController|Reference", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UUW_ExploreCursor> ExploreCursorClass;
	
	UPROPERTY(BlueprintReadWrite, Category="ExplorePlayerController|Reference", meta=(AllowPrivateAccess = "true"))
	UUW_ExploreCursor* ExploreCursor;

#pragma endregion	----------------------- Field Members -----------------------
};
