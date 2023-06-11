#include "SRG/Quests/QuestBase.h"

#include "Kismet/GameplayStatics.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "SRGCore/Utilities/SRGLog.h"

AQuestBase::AQuestBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	QuestUnavailableText = FText::FromString(TEXT("Quest is unavailable at the moment."));
	NewQuestText = FText::FromString(TEXT("New Quest received!"));
	QuestInProgressText = FText::FromString(TEXT("Quest is in progress."));
	QuestReadyToDeliverText = FText::FromString(TEXT("Well done!"));
	QuestAlreadyCompletedText = FText::FromString(TEXT("You have already completed this quest."));
}

void AQuestBase::BeginPlay()
{
	Super::BeginPlay();
}

void AQuestBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuestBase::Load()
{
	AExploreHeroBase* ExploreHeroBase = GetExploreHeroBase();
	SRPG_CHECK(ExploreHeroBase);

	if (IsActiveQuest(ExploreHeroBase))
	{
		HandleActiveQuest(ExploreHeroBase);
		return;
	}

	if (IsCompletedQuest(ExploreHeroBase))
	{
		Completed();
	}
}

void AQuestBase::Activate(int32 CurrentStep)
{
}

void AQuestBase::Completed()
{
}

bool AQuestBase::IsActiveQuest(AExploreHeroBase* ExploreHeroBase)
{
	return ExploreHeroBase->ActiveQuestData.Contains(this->GetClass());
}

bool AQuestBase::IsCompletedQuest(AExploreHeroBase* ExploreHeroBase)
{
	return ExploreHeroBase->CompletedQuestData.Contains(this->GetClass());
}

void AQuestBase::HandleActiveQuest(AExploreHeroBase* ExploreHeroBase)
{
	const EQuestStatus QuestStatus = ExploreHeroBase->GetQuestStatus(this->GetClass());

	if (QuestStatus == EQuestStatus::Active)
	{
		const int32 ActivateStep = ExploreHeroBase->ActiveQuestData.Find(this->GetClass())->CurrentStep;
		Activate(ActivateStep);
	}
	else if (QuestStatus == EQuestStatus::ReadyToDeliver)
	{
		Completed();
	}
}

int32 AQuestBase::GetSteps()
{
	return 1;
}

AExploreHeroBase* AQuestBase::GetExploreHeroBase()
{
	return Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
}
