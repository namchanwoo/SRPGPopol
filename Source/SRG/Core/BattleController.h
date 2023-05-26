#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/SRGEnumStruct.h"
#include "BattleController.generated.h"

class AGrid;
class ADeploymentController;
class ACharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleStateChanged, EBattleState, NewBattleState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewTurn, int32, NewTurn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllPassiveAbilitiesUsed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllDOTsApplied);

UCLASS()
class SRG_API ABattleController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABattleController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	AGrid* GetGrid() { return Grid; };


	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	ADeploymentController* DeploymentController;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	TArray<ACharacterBase*> PlayerCharacters;

	UPROPERTY(BlueprintReadWrite, Category="Battle Controller")
	AGrid* Grid;

	/*---	      	    Delegate Event    	      	---*/
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Controller|Delegate")
	FOnBattleStateChanged OnBattleStateChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Controller|Delegate")
	FOnNewTurn OnNewTurn;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Controller|Delegate")
	FOnAllPassiveAbilitiesUsed OnAllPassiveAbilitiesUsed;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Controller|Delegate")
	FOnAllDOTsApplied OnAllDOTsApplied;
};
