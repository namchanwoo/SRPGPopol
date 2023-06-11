
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "DeploymentController.generated.h"

class ABattleController;
class AGrid;

UCLASS()
class SRG_API ADeploymentController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADeploymentController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void InitializeEvent();
	
	void SetNonDeployedCharacters();

	UPROPERTY()
	ABattleController* BattleController;
	
	UPROPERTY()
	AGrid* Grid;

	UPROPERTY()
	TArray<FPlayerCharacterData> NonDeployedCharacters;
};
