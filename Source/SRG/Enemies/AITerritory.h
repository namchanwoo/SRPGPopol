#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AITerritory.generated.h"


UCLASS()
class SRG_API AAITerritory : public AActor
{
	GENERATED_BODY()

public:
	AAITerritory();

	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

#pragma region   	AI Territory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Territory|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Territory|Component")
	UStaticMeshComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Territory")
	float Radius = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Territory")
	FLinearColor Color;
#pragma endregion	AI Territory
};
