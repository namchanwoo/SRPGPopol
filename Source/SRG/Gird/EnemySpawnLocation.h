#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnLocation.generated.h"

class UBoxComponent;
class UArrowComponent;
class ASlotBase;

UCLASS()
class SRG_API AEnemySpawnLocation : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawnLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:



	/*******************************************
	 * Field Members
	 *************** ****************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemySpawnLocation")
	ASlotBase* Slot;

	/*******************************************
 	* Components
 	*******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Spawn Location|Component")
	USceneComponent* DefaultScene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Spawn Location|Component")
	UStaticMeshComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Spawn Location|Component")
	UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Spawn Location|Component")
	UBoxComponent* Box;
};
