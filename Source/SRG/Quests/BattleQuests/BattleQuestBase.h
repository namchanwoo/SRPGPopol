#pragma once

#include "CoreMinimal.h"
#include "SRG/Quests/QuestBase.h"
#include "BattleQuestBase.generated.h"

class AEnemyExplorePawnBase;

/**
 * ABattleQuestBase는 전투 퀘스트를 관리하는 AQuestBase의 서브클래스입니다.
 * 이 클래스는 퀘스트에 참여하는 적 캐릭터 (EnemyPawn)의 가시성과 상호작용을 제어합니다.
 * 각 적은 퀘스트에서 한 단계를 나타냅니다.
 */
UCLASS()
class SRG_API ABattleQuestBase : public AQuestBase
{
	GENERATED_BODY()

public:
	ABattleQuestBase();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * @brief 퀘스트의 현재 단계를 활성화합니다. 퀘스트의 현재 단계에 해당하는 적 캐릭터를 활성화합니다. 
	 * 또한, bHideActors가 true인 경우 현재 단계 이전의 모든 적 캐릭터를 비활성화합니다.
	 * @param CurrentStep 활성화할 퀘스트의 현재 단계
	 */
	virtual void Activate(int32 CurrentStep) override;


	/**
	 * @brief 퀘스트가 완료되었을 때 호출되는 함수입니다.
	 * bHideActors가 true인 경우 퀘스트에 연관된 모든 적 캐릭터를 비활성화합니다.
	 */
	virtual void Completed() override;

	/**
	 * @brief 이 함수는 퀘스트를 완료하는 데 필요한 단계의 수를 반환합니다.
	 * @return 퀘스트를 완료하는 데 필요한 적 캐릭터의 수
	 */
	virtual int32 GetSteps() override;

public:
	/**
 	* @brief 퀘스트 액터들의 상태를 설정합니다. 퀘스트가 활성화되었을 경우, 적 캐릭터를 활성화하고 상호작용을 가능하게 합니다. 
 	* 퀘스트가 비활성화 되었을 경우, 적 캐릭터를 비활성화하고 상호작용을 불가능하게 합니다. 
 	* @param Actor 퀘스트에 참여하는 적 캐릭터
 	* @param IsActive 퀘스트의 활성화 여부
 	*/
	void SetQuestActors(AEnemyExplorePawnBase* Actor, bool IsActive);


	/*******************************************
	 * Field Members
	 *******************************************/

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest|Battle Quest")
	bool bHideActors = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest|Battle Quest")
	TArray<AEnemyExplorePawnBase*> EnemyPawn;
};
