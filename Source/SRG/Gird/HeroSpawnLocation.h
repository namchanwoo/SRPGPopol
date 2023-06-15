#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroSpawnLocation.generated.h"

class UArrowComponent;

UCLASS()
class SRG_API AHeroSpawnLocation : public AActor
{
	GENERATED_BODY()

public:
	AHeroSpawnLocation();

protected:
	virtual void BeginPlay() override;

public:
	


	/*******************************************
	 * Components
	 *******************************************/
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Hero Spawn Location|Component")
	USceneComponent* DefaultScene;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Hero Spawn Location|Component")
	UStaticMeshComponent* Sphere;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Hero Spawn Location|Component")
	UArrowComponent* Arrow;

	
};
