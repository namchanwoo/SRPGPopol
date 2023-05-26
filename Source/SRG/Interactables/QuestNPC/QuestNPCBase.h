#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestNPCBase.generated.h"

class UUW_InfoDialogue;
class AQuestBase;
class AQuestLogic;
class AInteractionDetector;

/*
 * AQuestNPCBase는 게임에서 퀘스트를 담당하는 NPC(Non-Player Character)를 나타내는 클래스입니다.
 * 이 클래스는 대화 위젯 생성, 퀘스트 가용성 및 상태 변경 처리를 포함하여 NPC의 퀘스트 관련 동작을 관리합니다.
 */
UCLASS()
class SRG_API AQuestNPCBase : public AActor
{
	GENERATED_BODY()

public:
	AQuestNPCBase();
	
	void InitializeCustomComponents();
	
	void InitializeInfoDialogueClass();

protected:
	
	virtual void BeginPlay() override;

	void SetupInteractionDetector();
	
	void SetupQuestLogic();

	/**
 	* @brief 대화 위젯을 만들고 설정합니다.
 	* @param Title 대화 위젯의 제목입니다.
 	* @param YesButtonText '예' 버튼의 텍스트입니다.
 	* @param DialogueText 대화의 본문입니다.
 	* @param Delegate '확인' 버튼을 클릭했을 때 호출되는 함수.
 	*/
	UUW_InfoDialogue* CreateAndSetupDialogueWidget(FString Title, FString YesButtonText, FString DialogueText);

	/*******************************************
	 * Interaction Handlers
	 *******************************************/
public:
	/**
 	* @brief 이 함수는 NPC 상호작용이 감지될 때 호출됩니다. 감지기에서 상호 작용을 활성화하고 퀘스트 상태를 확인합니다.
 	* @param InButtonIndex 상호 작용을 트리거한 버튼의 인덱스입니다.
 	*/
	UFUNCTION()
	void OnInteracted_QuestNPC(int32 InButtonIndex);

	/**
 	* @brief 이 기능은 특정 게임 조건이 로드될 때 트리거됩니다. InteractionDetector에서 상호 작용을 활성화합니다.
 	* @param bBShouldLoadEnemyPawns 적 폰을 로드해야 하는지 여부를 나타내는 부울입니다.
 	*/
	UFUNCTION()
	void OnLoaded_Handler(bool bBShouldLoadEnemyPawns);

	/*******************************************
	* Quest State Handlers
	*******************************************/
public:
	UFUNCTION()
	void OnQuestUnavailable_Handle(AQuestBase* InQuest);

	UFUNCTION()
	void OnNewQuestReceived_Handle(AQuestBase* InQuest);

	UFUNCTION()
	void OnQuestInProgress_Handle(AQuestBase* InQuest);

	UFUNCTION()
	void OnQuestReadyToDeliver_Handle(AQuestBase* InQuest);

	UFUNCTION()
	void OnQuestAlreadyCompleted_Handle(AQuestBase* InQuest);


	/*******************************************
	 * Quest Confirmation Handlers
	 *******************************************/
public:
	UFUNCTION()
	void OnQuestUnavailableConfirmed();

	UFUNCTION()
	void OnNewQuestReceivedConfirmed();

	UFUNCTION()
	void OnQuestInProgressConfirmed();

	UFUNCTION()
	void OnQuestDeliveredConfirmed();

	UFUNCTION()
	void OnQuestAlreadyCompletedConfirmed();


	/*******************************************
	 * Field Members
	 *******************************************/

	/*---	      	    Setting    	      	---*/
public:
	// 대화 상자 클래스의 참조. 이 클래스는 NPC와 대화할 때 사용됩니다.
	UPROPERTY(BlueprintReadWrite, Category="Quest Npc|Setting")
	TSubclassOf<UUW_InfoDialogue> WBP_InfoDialogueClass;

	/*---	      	    Component    	      	---*/
public:
	// 기본 SceneComponent. 이것은 이 액터의 RootComponent로 사용됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Npc|Component")
	USceneComponent* DefaultSceneRoot;

	// 상호작용 관련 정보를 처리하는 데 사용되는 자식 액터 컴포넌트.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Npc|Component")
	UChildActorComponent* InteractionActor;

	// 퀘스트 관련 정보를 처리하는 데 사용되는 자식 액터 컴포넌트.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Npc|Component")
	UChildActorComponent* QuestActor;

	/*---	      	    Quest NPC    	      	---*/
public:
	// 플레이어와 NPC 간의 상호작용을 감지하는 인스턴스.
	UPROPERTY(BlueprintReadWrite, Category="Quest Npc")
	AInteractionDetector* InteractionDetector;

	// 퀘스트 상태와 진행 상황을 관리하는 인스턴스.
	UPROPERTY(BlueprintReadWrite, Category="Quest Npc")
	AQuestLogic* QuestLogic;

	// NPC가 제공할 수 있는 모든 퀘스트의 배열.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest Npc")
	TArray<AQuestBase*> Quests;

private:
	// 현재 화면에 표시되는 대화 상자 위젯 인스턴스.
	UPROPERTY()
	UUW_InfoDialogue* QuestUnavailableDialogue;

	UPROPERTY()
	UUW_InfoDialogue* NewQuestReceivedDialogue;

	UPROPERTY()
	UUW_InfoDialogue* QuestInProgressDialogue;

	UPROPERTY()
	UUW_InfoDialogue* QuestReadyToDeliverDialogue;

	UPROPERTY()
	UUW_InfoDialogue* QuestAlreadyCompletedDialogue;
	

	// 현재 처리 중인 퀘스트 인스턴스.
	UPROPERTY()
	AQuestBase* ProcessingQuest;
};
