#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "SRG/Widgets/ExploreWidgets/UW_EnemyInfoDialogue.h"
#include "SRGCore/SRGEnumStruct.h"
#include "EnemyExplorePawnBase.generated.h"

class UBlendSpace1D;
class UTimelineComponent;
class AAITerritory;
class UBoxComponent;
class AExplorePlayerController;
class AExploreHeroBase;
class USRGGameInstance;
class AInteractionDetector;
class AAIController;
class AObstacleBase;


UCLASS()
class SRG_API AEnemyExplorePawnBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyExplorePawnBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region   	Enemy Explore Pawn Event

public:
	void EnableAI();

	void StopAI();

	void StartBattle();

	void GetAITerritory(FVector& OutOrigin, float& OutRadius);
#pragma endregion	Enemy Explore Pawn Event


#pragma region   	Logic Function

protected:
	bool GetRandomReachablePointInRadius(UWorld* World, const FVector& Origin, float Radius, FNavLocation& RandomLocation);

	void MoveToLocation(AAIController* AIController, const FVector& Destination, float AcceptanceRadius, bool bStopOnOverlap);


#pragma endregion	Logic Function


#pragma region   	On Delegate Function

public:
	UFUNCTION()
	void OnInteracted_Handle(int32 InButtonIndex);

	UFUNCTION()
	void OnLoaded_Handle(bool bShouldLoadEnemyPawns);

	UFUNCTION()
	void OnBeginCursorOver_RightClickDetector(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnEndCursorOver_RightClickDetector(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnClicked_RightClickDetector(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnContinueClicked_Handler();

protected:
	UFUNCTION()
	void EnableAIUpdateCallbackFunction();

	UFUNCTION()
	void EnableAIFinishedCallbackFunction();

	
	void OnMoveCompletedCallback(FAIRequestID RequestID, const FPathFollowingResult& Result);

#pragma endregion	On Delegate Function


#pragma region   	Field Members

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EnemyExplorePawnBase|Component")
	UChildActorComponent* InteractionActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EnemyExplorePawnBase|Component")
	UBoxComponent* RightClickDetector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyExplorePawnBase|Setting")
	TSubclassOf<UUW_EnemyInfoDialogue> WBP_EnemyInfoDialogueClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	bool bRespawnAfterLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	FName BattleMapName;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	AExplorePlayerController* ExplorePlayerController;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	AExploreHeroBase* ExploreHero;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	USRGGameInstance* GameInstance;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	AInteractionDetector* InteractionDetector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	AAITerritory* AITerritory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyExplorePawnBase|Setting")
	UBlendSpace1D* IdleWalkBlendSpace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	TArray<TSubclassOf<AEquipmentBase>> Drops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	TArray<FEnemyCharacterData> EnemyCharacterList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyExplorePawnBase")
	TMap<TSubclassOf<AObstacleBase>, int32> ObstacleList;

protected:
	FTimeline EnableAITimeLine;

	UPROPERTY()
	UUW_EnemyInfoDialogue* EnemyInfoDialogue;


#pragma endregion	Field Members
};
