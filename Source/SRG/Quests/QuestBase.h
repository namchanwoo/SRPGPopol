#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "QuestBase.generated.h"

/*
 * AQuestBase는 게임에서 기본 퀘스트를 나타내는 액터 클래스입니다.
 * 퀘스트 상태를 관리하고, 퀘스트를 로드하고, 퀘스트 진행을 처리하는 기능을 제공합니다.
 */
UCLASS()
class SRG_API AQuestBase : public AActor
{
	GENERATED_BODY()

public:
	AQuestBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 퀘스트를 로드하고 상태를 확인하고 영웅의 퀘스트 데이터를 기반으로 적절한 기능을 호출합니다.
	 */
	void Load();

public:
	/**
 	 * @brief 지정된 단계에서 퀘스트를 활성화합니다.
 	 * @param CurrentStep 퀘스트를 활성화하는 단계
 	 */
	virtual void Activate(int32 CurrentStep);

	/**
	 * @brief 퀘스트 완료로 표시
	 */
	virtual void Completed();

	/**
	 * @brief 주어진 영웅에 대한 퀘스트가 활성화되어 있는지 확인합니다.
	 * @param ExploreHeroBase 퀘스트 진행 상황을 확인할 영웅
	 * @return 퀘스트가 활성화되어 있으면 true, 그렇지 않으면 false
	 */
	bool IsActiveQuest(AExploreHeroBase* ExploreHeroBase);

	/**
	 * @brief 주어진 영웅의 퀘스트 완료 여부 확인
	 * @param ExploreHeroBase 퀘스트 진행 상황을 확인할 영웅
	 * @return 퀘스트가 완료되면 true, 그렇지 않으면 false
	 */
	bool IsCompletedQuest(AExploreHeroBase* ExploreHeroBase);

	/**
	 * @brief 퀘스트 상태에 따라 적절한 함수를 호출하여 활성 퀘스트를 처리합니다.
	 * @param ExploreHeroBase 활성 퀘스트를 처리할 영웅
	 */
	void HandleActiveQuest(AExploreHeroBase* ExploreHeroBase);

	/**
	 * @brief 퀘스트의 단계 수를 검색합니다.
	 * @return 퀘스트의 단계 수
	 */
	virtual int32 GetSteps();

	/**
	 * @brief 플레이어에 대한 AExploreHeroBase 인스턴스를 검색합니다.
	 * @return AExploreHeroBase 인스턴스에 대한 포인터
	 */
	AExploreHeroBase* GetExploreHeroBase();

	

	/*******************************************
	 * Map Operations
	 *******************************************/

	/*---	      	    Component    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest|Component")
	USceneComponent* DefaultSceneRoot;

	/*---	      	    Quest    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	bool bShowNotification = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FText NewQuestText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FText QuestUnavailableText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FText QuestInProgressText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FText QuestReadyToDeliverText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FText QuestAlreadyCompletedText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FQuestSounds QuestSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FQuestRewards QuestRewards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	TArray<TSubclassOf<AQuestBase>> RequiredQuests;
};
