#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestNPCBase.generated.h"

class UUW_InfoDialogue;
class AQuestBase;
class AQuestLogic;
class AInteractionDetector;

UCLASS()
class SRG_API AQuestNPCBase : public AActor
{
	GENERATED_BODY()

public:
	AQuestNPCBase();

protected:
	virtual void BeginPlay() override;

#pragma region   	On Delegate Event

public:
	UFUNCTION()
	void OnInteracted_QuestNPC(int32 InButtonIndex);

	UFUNCTION()
	void OnLoaded_Handler(bool bBShouldLoadEnemyPawns);

	UFUNCTION()
	void OnQuestUnavailable_Handle(AQuestBase* InQuest);

	UFUNCTION()
	void OnNewQuestReceived_Handle(AQuestBase* InQuest);

	UFUNCTION()
	void OnQuestInProgress_Handle(AQuestBase* InQuest);

	UFUNCTION()
	void OnQuestReadyToDeliver_Handle(AQuestBase* InQuest);


	UFUNCTION()
	void OnQuestAlreadyCompleted_Handle(AQuestBase* InQuest);

	UFUNCTION()
	void OnQuestUnavailableConfirmed();

	UFUNCTION()
	void OnNewQuestReceivedConfirmed();

	UFUNCTION()
	void OnQuestInProgressConfirmed();

	UFUNCTION()
	void OnQuestDeliveredConfirmed();

	UFUNCTION()
	void OnQuestAlreadyCompletedConfirmed();

#pragma endregion	On Delegate Event


#pragma region   	Field Members

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Npc|Component")
	UChildActorComponent* InteractionActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Npc|Component")
	UChildActorComponent* QuestActor;

	UPROPERTY(BlueprintReadWrite, Category="Quest Npc|Setting")
	TSubclassOf<UUW_InfoDialogue> InfoDialogueClass;

	UPROPERTY(BlueprintReadWrite, Category="Quest Npc")
	AInteractionDetector* InteractionDetector;

	UPROPERTY(BlueprintReadWrite, Category="Quest Npc")
	AQuestLogic* QuestLogic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Npc")
	TArray<AQuestBase*> Quests;

private:
	UPROPERTY()
	UUW_InfoDialogue* InfoDialogueWidget;

	UPROPERTY()
	AQuestBase* ProcessingQuest;
#pragma endregion	Field Members
};
