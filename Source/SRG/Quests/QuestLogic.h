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

/**
 * AQuestLogic 클래스는 게임 내의 퀘스트 상태를 확인하고 해당 퀘스트에 대한 상태 변경을 관리합니다.
 * 이 클래스는 현재 진행 중인 퀘스트를 추적하며, 퀘스트 상태에 따른 이벤트를 트리거합니다.
 * 또한, 퀘스트 상태 변경 시 실행되는 델리게이트를 제공합니다.
 */
UCLASS()
class SRG_API AQuestLogic : public AActor
{
	GENERATED_BODY()

public:
	AQuestLogic();

protected:
	virtual void BeginPlay() override;

public:
	/**
 	* @brief Quest 상태를 체크하고 알맞은 delegate를 broadcast 합니다.
 	* @param InExploreHero : Quest 상태를 체크할 ExploreHeroBase 포인터
 	* @param InQuests : 체크할 Quest 객체들의 배열
 	*/
	void CheckQuestStatus(AExploreHeroBase* InExploreHero, const TArray<AQuestBase*>& InQuests);

	/**
	 * @brief 현재 활성화된 퀘스트를 검색합니다.
	 * @return 발견된 경우 활성 AQuestBase 개체에 대한 포인터, 그렇지 않으면 목록의 마지막 퀘스트입니다.
	 */
	AQuestBase* GetCurrentQuest();


	/*******************************************
	 * Field Members
	 *******************************************/

	/*---	      	    Component    	      	---*/
public:
	// 기본적인 구성 요소인 SceneComponent. 다른 구성 요소들의 상위 요소로 사용됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest Logic|Component")
	USceneComponent* DefaultSceneRoot;

	// 퀘스트 상태를 체크할 대상인 탐험용 영웅 객체를 참조합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest Logic")
	AExploreHeroBase* ExploreHero;

	// 이 퀘스트 로직에서 관리하는 퀘스트들의 리스트입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest Logic")
	TArray<AQuestBase*> Quests;

	/*---	      	    Delegate    	      	---*/
public:
	// 새로운 퀘스트를 받았을 때 호출되는 델리게이트입니다.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Quest Logic|Delegate")
	FOnNewQuestReceived OnNewQuestReceived;

	// 진행 중인 퀘스트가 있을 때 호출되는 델리게이트입니다.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Quest Logic|Delegate")
	FOnQuestInProgress OnQuestInProgress;

	// 퀘스트를 완료하여 전달할 준비가 되었을 때 호출되는 델리게이트입니다.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Quest Logic|Delegate")
	FOnQuestReadyToDeliver OnQuestReadyToDeliver;

	// 이미 완료된 퀘스트에 대해 호출되는 델리게이트입니다.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Quest Logic|Delegate")
	FOnQuestAlreadyCompleted OnQuestAlreadyCompleted;

	// 사용할 수 없는 퀘스트에 대해 호출되는 델리게이트입니다.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Quest Logic|Delegate")
	FOnQuestUnavailable OnQuestUnavailable;
};
