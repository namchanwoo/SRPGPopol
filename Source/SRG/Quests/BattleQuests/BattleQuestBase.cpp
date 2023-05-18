#include "SRG/Quests/BattleQuests/BattleQuestBase.h"

#include "SRG/Enemies/EnemyExplorePawnBase.h"
#include "SRG/Interactables/InteractionDetector.h"
#include "SRG/Libraries/SRGFunctionLibrary.h"

ABattleQuestBase::ABattleQuestBase()
{
}

void ABattleQuestBase::BeginPlay()
{
	// Super::BeginPlay();

	for (AEnemyExplorePawnBase* Pawn : EnemyPawn)
	{
		SetQuestActors(Pawn, false);
	}
}

void ABattleQuestBase::SetQuestActors(AEnemyExplorePawnBase* Actor, bool IsActive)
{
	if (!IsValid(Actor))
	{
		return;
	}

	if (bHideActors)
	{
		USRGFunctionLibrary::DisableActor(Actor, !IsActive);
	}

	UChildActorComponent* ChildActorComponent = Actor->FindComponentByClass<UChildActorComponent>();
	if (!(ChildActorComponent && ChildActorComponent->GetChildActor()))
	{
		return;
	}

	AInteractionDetector* InteractionDetector = Cast<AInteractionDetector>(ChildActorComponent->GetChildActor());
	if (InteractionDetector)
	{
		if (IsActive)
		{
			InteractionDetector->EnableInteraction();
		}
		else
		{
			InteractionDetector->DisableInteraction();
		}
	}
}

void ABattleQuestBase::Activate(int32 CurrentStep)
{
	if (!EnemyPawn.IsValidIndex(CurrentStep))
	{
		return;
	}
	SetQuestActors(EnemyPawn[CurrentStep], true);

	if (!bHideActors)
	{
		return;
	}

	for (int32 i = 0; i < EnemyPawn.Num(); i++)
	{
		if (!IsValid(EnemyPawn[i]))
			continue;

		if (i < CurrentStep)
		{
			USRGFunctionLibrary::DisableActor(EnemyPawn[i], false);
		}
	}
}

void ABattleQuestBase::Completed()
{
	if (!bHideActors)
	{
		return;
	}

	for (int32 i = 0; i < EnemyPawn.Num(); i++)
	{
		if (!IsValid(EnemyPawn[i]))
			continue;

		USRGFunctionLibrary::DisableActor(EnemyPawn[i], false);
	}
}

int32 ABattleQuestBase::GetSteps()
{
	return EnemyPawn.Num();
}
