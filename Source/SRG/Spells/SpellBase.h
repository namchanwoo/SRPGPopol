#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpellUseEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpellUseStarted);

UCLASS()
class SRG_API ASpellBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpellBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void OnSpellEnabled();
	void OnSpellDisabled();

	/*******************************************
	 * Field Members
	 *******************************************/
public:
	int32 ManaCost;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Spell|Delegate")
	FOnSpellUseEnded OnSpellUseEnded;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Spell|Delegate")
	FOnSpellUseStarted OnSpellUseStarted;
};
