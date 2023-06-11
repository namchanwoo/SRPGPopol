#include "SRG/Quests/BattleQuests/BattleQuestBase.h"

#include "SRG/Enemies/EnemyExplorePawnBase.h"
#include "SRG/Interactables/InteractionDetector.h"
#include "SRG/Libraries/SRPGFunctionLibrary.h"

ABattleQuestBase::ABattleQuestBase()
{
}

void ABattleQuestBase::BeginPlay()
{
	Super::BeginPlay();

	for (AEnemyExplorePawnBase* Pawn : EnemyPawn)
	{
		SetQuestActors(Pawn, false);
	}
}

void ABattleQuestBase::Activate(int32 CurrentStep)
{
	// 현재 단계가 EnemyPawn 배열의 유효한 인덱스인지 확인
	if (!EnemyPawn.IsValidIndex(CurrentStep))
	{
		return;
	}

	// 현재 단계에 해당하는 적 캐릭터를 활성화
	SetQuestActors(EnemyPawn[CurrentStep], true);

	// bHideActors가 false인 경우 함수 종료
	if (!bHideActors)
	{
		return;
	}

	// 현재 단계 이전의 모든 적 캐릭터를 비활성화
	for (int32 i = 0; i < CurrentStep; i++)
	{
		if (IsValid(EnemyPawn[i]))
		{
			USRPGFunctionLibrary::DisableActor(EnemyPawn[i], false);
		}
	}
}

void ABattleQuestBase::Completed()
{
	// bHideActors가 false인 경우 함수 종료
	if (!bHideActors)
	{
		return;
	}

	// 퀘스트에 연관된 모든 적 캐릭터를 비활성화
	for (AEnemyExplorePawnBase* pawn : EnemyPawn)
	{
		if (IsValid(pawn))
		{
			USRPGFunctionLibrary::DisableActor(pawn, false);
		}
	}
}

int32 ABattleQuestBase::GetSteps()
{
	return EnemyPawn.Num();
}

void ABattleQuestBase::SetQuestActors(AEnemyExplorePawnBase* Actor, bool IsActive)
{
	// Actor 유효성 확인
	if (!IsValid(Actor))
	{
		return;
	}

	// IsActive와 bHideActors에 따른 Actor 가시성 처리
	if (bHideActors)
	{
		USRPGFunctionLibrary::DisableActor(Actor, !IsActive);
	}

	// ChildActorComponent를 검색하고 유효성 확인
	const UChildActorComponent* ChildActorComponent = Actor->FindComponentByClass<UChildActorComponent>();
	if (!ChildActorComponent || !ChildActorComponent->GetChildActor())
	{
		return;
	}

	// ChildActor에 InteractionDetector가 존재하면, IsActive에 따라 상호작용을 처리
	if (AInteractionDetector* InteractionDetector = Cast<AInteractionDetector>(ChildActorComponent->GetChildActor()))
	{
		IsActive ? InteractionDetector->EnableInteraction() : InteractionDetector->DisableInteraction();
	}
}