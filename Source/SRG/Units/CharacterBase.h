#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "CharacterBase.generated.h"

UCLASS()
class SRG_API ACharacterBase : public AUnitBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region   	Field Members

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CharacterBase")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CharacterBase")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CharacterBase")
	int32 Stack = 1;
#pragma endregion	Field Members
};
