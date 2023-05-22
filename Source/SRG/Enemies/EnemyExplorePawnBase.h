#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "SRG/Widgets/ExploreWidgets/UW_EnemyInfoDialogue.h"
#include "SRGCore/SRGEnumStruct.h"
#include "EnemyExplorePawnBase.generated.h"

class UBlendSpace1D;
class UTimelineComponent;
class AAITerritory;
class UBoxComponent;
class AExplorePlayerController;
class AExploreHeroBase;
class USRGGameInstance;
class AInteractionDetector;
class AAIController;
class AObstacleBase;

/*
 * AEnemyExplorePawnBase는 탐색 설정에서 적을 나타내는 캐릭터 클래스입니다.
 * 적의 AI 동작, 이동, 상호 작용, 적의 전투 및 영토 속성을 처리합니다.
 */
UCLASS()
class SRG_API AEnemyExplorePawnBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyExplorePawnBase();

	// 캡슐 컴포넌트를 구성하는 함수
	void ConfigureCapsuleComponent();
	
	// 메시를 구성하는 함수
	void ConfigureMesh();

	// 캐릭터 움직임을 구성하는 함수
	void ConfigureCharacterMovement();

	// InteractionActor를 만드는 함수
	void CreateInteractionActor();

	// RightClickDetector를 만드는 함수
	void CreateRightClickDetector();
	
	// AIControllerClass를 할당하는 함수
	void AssignAIControllerClass();

	// WBP_EnemyInfoDialogueClass를 로드하는 함수
	void LoadWBP_EnemyInfoDialogueClass();

protected:
	virtual void BeginPlay() override;
	
	void SetupTimeLine();
	void SetupRightClickDetector();
	void SetupAIController();
	void SetupInteractionDetector();

public:
	virtual void Tick(float DeltaTime) override;

	/*******************************************
	 * AI Functions
	 *******************************************/
public:
	/**
 	* @brief AI를 활성화합니다.
 	*/
	void EnableAI();

	/**
 	* @brief AI를 중지합니다.
 	*/
	void StopAI();

protected:
	UFUNCTION()
	void EnableAIUpdateCallbackFunction();

	/**
 	* @brief AI 동작 완료 콜백 함수입니다.
 	*/
	UFUNCTION()
	void EnableAIFinishedCallbackFunction();

	/*******************************************
	 * Movement Functions
	 *******************************************/
protected:
	/**
 	* @brief 주어진 반경 내에서 임의의 도달 가능한 지점을 가져옵니다.
 	* @param World 월드 객체
 	* @param Origin 기준점
 	* @param Radius 반경
 	* @param RandomLocation 무작위 위치
 	* @return 가져오기 성공 여부
 	*/
	bool GetRandomReachablePointInRadius(UWorld* World, const FVector& Origin, float Radius,
	                                     FNavLocation& RandomLocation);

	/**
 	* @brief 위치로 이동합니다.
 	* @param AIController AI 컨트롤러
 	* @param Destination 목적지 위치
 	* @param AcceptanceRadius 도달 반경
 	* @param bStopOnOverlap 중첩시 멈출지 여부
 	*/
	void MoveToLocation(AAIController* AIController, const FVector& Destination, float AcceptanceRadius,
	                    bool bStopOnOverlap);

	
	/**
	 * @brief 이동 완료 콜백 함수입니다.
	 * @param RequestID AI 요청 ID
	 * @param Result 이동 결과
	 */
	void OnMoveCompletedCallback(FAIRequestID RequestID, const FPathFollowingResult& Result);


	/*******************************************
	 * Battle Functions
	 *******************************************/
public:

	/**
 	* @brief 전투를 시작하는 함수입니다.
 	*/
	void StartBattle();


	/*******************************************
	 * Territory Functions
	 *******************************************/
public:
	/**
 	* @brief AI 영역의 원점과 반지름을 가져오는 함수입니다.
 	* @param OutOrigin 가져온 원점을 저장할 변수입니다.
 	* @param OutRadius 가져온 반지름을 저장할 변수입니다.
 	*/
	void GetAITerritory(FVector& OutOrigin, float& OutRadius);


	/*******************************************
	 * Interaction Functions 
	 *******************************************/
public:
	/**
 	* @brief 상호작용 시 호출되는 핸들러 함수입니다.
 	* @param InButtonIndex 상호작용된 버튼의 인덱스입니다.
 	*/
	UFUNCTION()
	void OnInteracted_Handle(int32 InButtonIndex);

	/**
 	* @brief 로드 완료 시 호출되는 핸들러 함수입니다.
 	* @param bShouldLoadEnemyPawns 적 캐릭터를 로드해야하는지 여부입니다.
 	*/
	UFUNCTION()
	void OnLoaded_Handle(bool bShouldLoadEnemyPawns);

	/**
 	* @brief 오른쪽 클릭 감지기에서 커서가 올라갔을 때 호출되는 함수입니다.
 	* @param TouchedComponent 감지된 원시 컴포넌트입니다.
 	*/
	UFUNCTION()
	void OnBeginCursorOver_RightClickDetector(UPrimitiveComponent* TouchedComponent);

	/**
 	* @brief 오른쪽 클릭 감지기에서 커서가 벗어났을 때 호출되는 함수입니다.
 	* @param TouchedComponent 감지된 원시 컴포넌트입니다.
 	*/
	UFUNCTION()
	void OnEndCursorOver_RightClickDetector(UPrimitiveComponent* TouchedComponent);

	/**
 	* @brief 오른쪽 클릭 감지기에서 클릭이 발생했을 때 호출되는 함수입니다.
 	* @param TouchedComponent 감지된 원시 컴포넌트입니다.
 	* @param ButtonPressed 눌린 키입니다.
 	*/
	UFUNCTION()
	void OnClicked_RightClickDetector(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	/**
 	* @brief '계속하기' 버튼 클릭 이벤트 핸들러입니다.
 	*/
	UFUNCTION()
	void OnContinueClicked_Handler();


	/*******************************************
	 * Field Members
	 ******************************************/

	/*---	    Component Variables    ---*/
public:
	// 상호작용 액터 컴포넌트
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Explore|Components")
	UChildActorComponent* InteractionActor;

	// 오른쪽 클릭 감지기 컴포넌트
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Explore|Components")
	UBoxComponent* RightClickDetector;

	// 유휴 상태와 걷기 애니메이션 사이의 블렌드 공간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Explore|Components")
	UBlendSpace1D* IdleWalkBlendSpace;

	// 적 정보 대화 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Explore|Components")
	TSubclassOf<UUW_EnemyInfoDialogue> WBP_EnemyInfoDialogueClass;

	/*---	    Game State Variables    ---*/
public:
	// 게임 로드 후 적이 다시 생성되는지의 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Explore|Game State")
	bool bRespawnAfterLoad;

	// 게임 인스턴스
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Explore|Game State")
	USRGGameInstance* GameInstance;

	// 적이 가지고 있는 골드의 양
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Explore|Game State")
	int32 Gold;

	/*---	    Map and Territory Variables    ---*/
public:
	// 배틀맵의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Explore|Map and Territory")
	FName BattleMapName;

	// 적이 속한 AI 영토
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Explore|Map and Territory")
	AAITerritory* AITerritory;


	/*---	    Player and Hero Variables    ---*/
public:
	// 탐색 플레이어 컨트롤러
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Explore|Player and Hero")
	AExplorePlayerController* ExplorePlayerController;

	// 탐색 히어로 베이스
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Explore|Player and Hero")
	AExploreHeroBase* ExploreHero;

	/*---	    Interactions Variables    ---*/
public:
	// 상호작용 감지기
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Explore|Interactions")
	AInteractionDetector* InteractionDetector;

	/*---	    Enemy Attributes Variables    ---*/
public:
	// 적의 걷는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Explore|Enemy Attributes")
	float WalkSpeed = 300.0f;

	// 적이 드랍할 수 있는 장비 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Explore|Enemy Attributes")
	TArray<TSubclassOf<AEquipmentBase>> Drops;

	// 적 캐릭터 데이터 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Explore|Enemy Attributes")
	TArray<FEnemyCharacterData> EnemyCharacterList;

	// 장애물 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Explore|Enemy Attributes")
	TMap<TSubclassOf<AObstacleBase>, int32> ObstacleList;

	/*---	    Timeline Variables    ---*/
protected:
	// AI 활성화 타임라인
	FTimeline EnableAITimeLine;

	// 적 정보 대화창
	UPROPERTY()
	UUW_EnemyInfoDialogue* EnemyInfoDialogue;
};
