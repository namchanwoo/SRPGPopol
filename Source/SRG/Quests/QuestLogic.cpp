#include "QuestLogic.h"
#include "QuestBase.h"
#include "SRGCore/Utilities/SRGLog.h"
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
	if (InExploreHero == nullptr)
	{
		SRPG_LOG_ERROR(TEXT("InExploreHero는 null입니다."));
		return;
	}

	if (InQuests.Num() == 0)
	{
		SRPG_LOG_ERROR(TEXT("InQuests가 비어 있습니다."));
		return;
	}

	ExploreHero = InExploreHero;
	Quests = InQuests;

	AQuestBase* CurrentQuest = GetCurrentQuest();

	if (CurrentQuest == nullptr)
	{
		SRPG_LOG_ERROR(TEXT("CurrentQuest는 null입니다."));
		return;
	}

	const EQuestStatus QuestStatus = ExploreHero->GetQuestStatus(CurrentQuest->GetClass());

	// 델리게이트 바인딩 및 방송을 위한 람다함수 생성
	auto BroadcastIfBound = [](auto& Delegate, auto* Quest)
	{
		if (Delegate.IsBound())
			Delegate.Broadcast(Quest);
	};

	switch (QuestStatus)
	{
	case EQuestStatus::Unavailable:
		BroadcastIfBound(OnQuestUnavailable, CurrentQuest);
		break;
	case EQuestStatus::New:
		BroadcastIfBound(OnNewQuestReceived, CurrentQuest);
		break;
	case EQuestStatus::Active:
		BroadcastIfBound(OnQuestInProgress, CurrentQuest);
		break;
	case EQuestStatus::ReadyToDeliver:
		BroadcastIfBound(OnQuestReadyToDeliver, CurrentQuest);
		break;
	case EQuestStatus::Completed:
		BroadcastIfBound(OnQuestAlreadyCompleted, CurrentQuest);
		break;
	default:
		SRPG_LOG_ERROR(TEXT("알 수 없는 퀘스트 상태."));
		break;
	}
}


AQuestBase* AQuestLogic::GetCurrentQuest()
{
	// 퀘스트가 있는지 확인
	if (Quests.Num() == 0)
	{
		return nullptr; // 퀘스트가 없으면 null 반환
	}

	// ExploreHero가 아직 완료하지 않은 퀘스트를 찾습니다.
	for (AQuestBase* Quest : Quests)
	{
		if (!ExploreHero->CompletedQuestData.Contains(Quest->GetClass()))
		{
			return Quest; // 완료하지 않은 첫 번째 퀘스트 반환
		}
	}

	// 모든 퀘스트가 완료되면 목록의 마지막 퀘스트를 반환합니다.
	return Quests.Last();
}
