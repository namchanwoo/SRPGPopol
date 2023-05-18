#include "QuestLogic.h"

#include "QuestBase.h"
#include "SRG/Characters/ExploreHeroBase.h"


AQuestLogic::AQuestLogic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
}

void AQuestLogic::BeginPlay()
{
	Super::BeginPlay();
}

void AQuestLogic::CheckQuestStatus(AExploreHeroBase* InExploreHero, const TArray<AQuestBase*>& InQuests)
{
	ExploreHero = InExploreHero;
	Quests = InQuests;

	AQuestBase* CurrentQuest = GetCurrentQuest();
	EQuestStatus QuestStatus = ExploreHero->GetQuestStatus(CurrentQuest->GetClass());

	if (QuestStatus == EQuestStatus::Unavailable)
	{
		if (OnQuestUnavailable.IsBound())
			OnQuestUnavailable.Broadcast(CurrentQuest);
	}
	else if (QuestStatus == EQuestStatus::New)
	{
		if (OnNewQuestReceived.IsBound())
			OnNewQuestReceived.Broadcast(CurrentQuest);
	}
	else if (QuestStatus == EQuestStatus::Active)
	{
		if (OnQuestInProgress.IsBound())
			OnQuestInProgress.Broadcast(CurrentQuest);
	}
	else if (QuestStatus == EQuestStatus::ReadyToDeliver)
	{
		if (OnQuestReadyToDeliver.IsBound())
			OnQuestReadyToDeliver.Broadcast(CurrentQuest);
	}
	else if (QuestStatus == EQuestStatus::Completed)
	{
		if (OnQuestAlreadyCompleted.IsBound())
			OnQuestAlreadyCompleted.Broadcast(CurrentQuest);
	}
	
}

AQuestBase* AQuestLogic::GetCurrentQuest()
{
	AQuestBase* CurrentQuest = Quests[Quests.Num() - 1];

	for (AQuestBase* Quest : Quests)
	{
		if (!ExploreHero->CompletedQuestData.Contains(Quest->GetClass()))
		{
			CurrentQuest = Quest;
			break;
		}
	}

	return CurrentQuest;
}
