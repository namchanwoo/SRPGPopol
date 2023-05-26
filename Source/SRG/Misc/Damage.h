#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/SRGEnumStruct.h"
#include "Damage.generated.h"

UCLASS()
class SRG_API ADamage : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	int32 Stack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	int32 Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	int32 Heal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	EElement Element;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	bool bIsAI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	bool bIsResurrect;
};
