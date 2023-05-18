#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestLogic.generated.h"


class AExploreHeroBase;
class AQuestBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewQuestReceived, AQuestBase*, InQuest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestInProgress, AQuestBase*, InQuest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestReadyToDeliver, AQuestBase*, InQuest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAlreadyCompleted, AQuestBase*, InQuest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUnavailable, AQuestBase*, InQuest);

UCLASS()
class SRG_API AQuestLogic : public AActor
{
	GENERATED_BODY()

public:
	AQuestLogic();

protected:
	virtual void BeginPlay() override;

public:
	void CheckQuestStatus(AExploreHeroBase* InExploreHero, const TArray<AQuestBase*>& InQuests);

	AQuestBase* GetCurrentQuest();


#pragma region   	Field Members

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuestLogic|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuestLogic")
	AExploreHeroBase* ExploreHero;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuestLogic")
	TArray<AQuestBase*> Quests;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="QuestLogic|Delegate")
	FOnNewQuestReceived OnNewQuestReceived;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="QuestLogic|Delegate")
	FOnQuestInProgress OnQuestInProgress;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="QuestLogic|Delegate")
	FOnQuestReadyToDeliver OnQuestReadyToDeliver;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="QuestLogic|Delegate")
	FOnQuestAlreadyCompleted OnQuestAlreadyCompleted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="QuestLogic|Delegate")
	FOnQuestUnavailable OnQuestUnavailable;

#pragma endregion	Field Members
};
