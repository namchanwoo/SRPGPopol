#pragma once

#include "CoreMinimal.h"
#include "SRG/Quests/QuestBase.h"
#include "BattleQuestBase.generated.h"

class AEnemyExplorePawnBase;
/**
 * 
 */
UCLASS()
class SRG_API ABattleQuestBase : public AQuestBase
{
	GENERATED_BODY()

public:
	ABattleQuestBase();

	virtual void BeginPlay() override;

	void SetQuestActors(AEnemyExplorePawnBase* Actor,bool IsActive);

#pragma region   	Battle Quest Event

public:

	virtual void Activate(int32 CurrentStep) override;

	virtual void Completed() override;
	
	virtual int32 GetSteps() override;


#pragma endregion	Battle Quest Event

#pragma region   	Field Members

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest|Battle")
	bool bHideActors = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest|Battle")
	TArray<AEnemyExplorePawnBase*> EnemyPawn;

#pragma endregion	Field Members
};
