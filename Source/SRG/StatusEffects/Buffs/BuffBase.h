
#pragma once

#include "CoreMinimal.h"
#include "SRG/StatusEffects/StatusEffectBase.h"
#include "BuffBase.generated.h"

UCLASS()
class SRG_API ABuffBase : public AStatusEffectBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuffBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	



public:

};
