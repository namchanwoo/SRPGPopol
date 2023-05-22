#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SRGCore/SRGEnumStruct.h"
#include "ExploreHeroBase.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UBoxComponent;
class USphereComponent;

class AEquipmentBase;
class AExploreNavigationPath;
class AExplorePlayerController;
class ABattleQuestBase;
class AEnemyExplorePawnBase;
class AQuestBase;

class UUW_HeroInventory;
class UUW_QuestNotificationUI;
class UUW_LevelUpDialogue;
class UUW_GarrisonListClass;
class UUW_ClaimDialogue;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGarrisonListClosed);

/**
 * 게임의 탐색 모드 영웅을 위한 캐릭터 기반 클래스입니다.
 * AExploreHeroBase는 탐색 모드에서 영웅에 대한 고유한 움직임, 인벤토리 및 상호 작용과 같은 탐색 관련 기능을 추가합니다.
 */
UCLASS()
class SRG_API AExploreHeroBase : public ACharacter
{
	GENERATED_BODY()

public:

	AExploreHeroBase();

protected:
	virtual void BeginPlay() override;
	
	// 상호 작용 이벤트를 해당 메서드에 바인딩합니다.
	void BindInteractions();

	// 플레이어 컨트롤러의 유효성을 검사하고 유효한 경우 설정하고 캐스팅에 실패하면 false를 반환합니다.
	bool ValidateAndSetPlayerController();

	// 최대 보행 속도, 전체 마나 및 캐릭터 새로 고침을 포함한 초기 캐릭터 설정을 지정합니다.
	void SetInitialCharacterSettings();

	// 퀘스트 알림 UI를 만들고 구성한 다음 뷰포트에 추가합니다.
	void CreateQuestNotificationUI();

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/*******************************************
	 * Character Management
	 *******************************************/
public:
	/**
	* @brief MaxCharacterLimit와 일치하도록 PlayerCharacterList의 캐릭터 수를 조정합니다.
	* 제한보다 더 많은 캐릭터가 있으면 초과 문자를 제거합니다.
	* 제한보다 적은 문자가 있으면 빈 캐릭터를 추가합니다.
	*/
	void SetCharacters();

	/**
	 * @brief 플레이어의 캐릭터 목록에서 캐릭터를 제거합니다.
	 * @param Index 목록에서 제거할 문자의 인덱스입니다.
	 */
	void DismissCharacter(int32 Index);

	/**
	 * @brief PlayerCharacterList에 있는 유효한 캐릭터가 정확히 하나인지 확인합니다.
	 * @return 정확히 하나라면 True를 반환합니다.
	 */
	bool HasOnlyCharacter();

	/**
	 * @brief 플레이어 캐릭터 목록에 빈 슬롯이 있는지 확인하고 그 위치를 반환합니다.
	 * @param EmptySlotIndex - 빈 슬롯의 위치를 반환하는 데 사용됩니다.
	 * @return 플레이어 캐릭터 목록에 빈 슬롯이 있으면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool HasEmptySlot(int32& EmptySlotIndex);

	/**
	 * @brief 플레이어의 캐릭터 리스트에 지정된 캐릭터가 있는지 확인하고 그 인덱스를 반환합니다.
	 * @param InCharacter - 확인하려는 캐릭터.
	 * @param Index - 해당 캐릭터의 위치를 반환하는 데 사용됩니다.
	 * @return 플레이어의 캐릭터 리스트에 지정된 캐릭터가 있으면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool HasThisCharacter(TSubclassOf<ACharacterBase> InCharacter, int32& Index);

	/**
	* 제공된 캐릭터를 플레이어의 수비대에 추가합니다.
	* @param InCharacters - 추가할 캐릭터를 나타내는 데이터 구조의 배열
	*/
	void AddCharacters(const TArray<FPlayerCharacterData>& InCharacters);


	/*******************************************
	*  Inventory Management
	*******************************************/
public:
	/**
	* @brief 영웅의 총 골드에 지정된 양의 골드를 추가합니다. 금액은 양수여야 합니다.
	* @param Amount 추가할 금의 양입니다.
	*/
	void AddGold(int32 Amount);

	/**
	* @brief 영웅의 총 골드에서 지정된 양의 골드를 제거합니다. 금액은 양수여야 하며 현재 금 금액보다 작거나 같아야 합니다.
	* @param Amount 제거할 금의 양입니다.
	*/
	void RemoveGold(int32 Amount);

	/**
	* 플레이어의 배낭에 특정 장비를 추가합니다.
	* @param InEquipment - 추가할 장비 등급에 대한 참조
	*/
	void AddEquipment(TSubclassOf<AEquipmentBase> InEquipment);

	/**
	* 플레이어의 배낭에 여러 장비를 추가합니다.
	* @param InEquipments - 추가할 장비 클래스를 포함하는 배열에 대한 참조
	*/
	void AddEquipments(const TArray<TSubclassOf<AEquipmentBase>> InEquipments);

	/**
	* 플레이어의 배낭에서 특정 장비를 제거합니다.
	* @param InEquipment - 제거할 장비 등급에 대한 참조
	*/
	void RemoveEquipment(TSubclassOf<AEquipmentBase> InEquipment);

	/**
	* 지정된 장비 아이템을 플레이어 캐릭터에게 장착하여 배낭에서 장비 아이템 목록으로 이동시킵니다.
	* @param InEquipment - 장착할 특정 장비 항목입니다.
	*/
	void Equip(TSubclassOf<AEquipmentBase> InEquipment);

	/**
	* 플레이어 캐릭터에서 지정된 장비 아이템을 장착 해제하고 장비 아이템 목록에서 백팩으로 다시 이동합니다.
	* @param InEquipment - 장착 해제할 특정 장비 항목입니다.
	*/
	void UnEquip(TSubclassOf<AEquipmentBase> InEquipment);


	/*******************************************
	*  Quest Management
	*******************************************/
public:
	/**
	* @brief 플레이어의 활성 퀘스트 목록에 새 퀘스트를 추가합니다.
	* 퀘스트가 새로운 경우 ActiveQuestData 및 ActiveQuests에 추가된 다음 게임 인스턴스에 추가됩니다. 퀘스트에 알림이 있으면 UI에 표시됩니다.
	* @param NewQuest 추가할 퀘스트에 대한 포인터입니다.
	*/
	void AddQuest(AQuestBase* NewQuest);

	/**
	* @brief 플레이어의 퀘스트를 로드합니다. 이 함수는 단순히 전달된 퀘스트를 ActiveQuests 배열에 추가합니다.
	* @param InQuest 로드할 퀘스트에 대한 포인터입니다.
	*/
	void LoadQuests(AQuestBase* InQuest);

	/**
	* @brief 완료된 퀘스트를 제공합니다.
	* 퀘스트를 전달할 준비가 되면 ActiveQuestData 및 ActiveQuests에서 제거되고 CompletedQuestData에 추가되며 게임 인스턴스에서 제거됩니다.
	* 그러면 퀘스트 보상이 표시됩니다.
	* @param InQuest 전달할 퀘스트에 대한 포인터입니다.
	*/
	void DeliverQuest(AQuestBase* InQuest);


	/**
	* 플레이어에게 퀘스트의 보상을 보여줍니다.
	* @param QuestRewards - 완료된 퀘스트의 보상 구조에 대한 참조
	*/
	void ShowQuestRewards(const FQuestRewards& QuestRewards);

	/**
	* 완료된 퀘스트의 보상을 받아 플레이어의 인벤토리에 추가합니다.
	* @param InQuestRewards - 완료된 퀘스트의 보상 구조에 대한 참조
	*/
	void ReceiveQuestRewards(const FQuestRewards& InQuestRewards);

	/**
 	* @brief 특정 퀘스트가 사용 가능한지 (모든 필요 퀘스트가 완료되었는지) 확인합니다.
 	* @param InQuest - 사용 가능한지 확인할 퀘스트.
 	* @return 퀘스트가 사용 가능하면 true, 그렇지 않으면 false를 반환합니다.
 	*/
	bool IsQuestAvailable(TSubclassOf<AQuestBase> InQuest);

	/**
	 * @brief 특정 퀘스트의 상태를 반환합니다.
	 * @param InQuest - 상태를 확인할 퀘스트.
	 * @return 퀘스트의 현재 상태를 EQuestStatus 열거형 형태로 반환합니다.
	 */
	EQuestStatus GetQuestStatus(TSubclassOf<AQuestBase> InQuest);

	/**
	 * @brief 퀘스트 업데이트 함수입니다.
	 * @param InQuest - 업데이트할 퀘스트입니다.
	 * @param StepsProgress - 퀘스트의 진행 단계입니다.
	 * @param OverrideProgress - 기존 진행률을 덮어씌울지 여부입니다.
	 * @param IgnoreNotification - 알람을 무시할지 여부입니다.
	 */
	void UpdateQuest(AQuestBase* InQuest, int StepsProgress, bool OverrideProgress, bool IgnoreNotification);

	/**
	 * @brief 상호작용 퀘스트를 업데이트하는 함수입니다.
	 * @param InteractableActor - 플레이어가 상호작용하려는 엑터입니다.
	 */
	void UpdateInteractionQuest(AActor* InteractableActor);

	/**
	 * @brief 전투 퀘스트를 업데이트하는 함수입니다.
	 * @param EnemyPawn - 플레이어가 전투를 수행하려는 적 피스입니다.
	 * @return 해당 전투 퀘스트를 반환합니다. 적 피스가 퀘스트에 없으면 nullptr를 반환합니다.
	 */
	ABattleQuestBase* UpdateBattleQuest(AEnemyExplorePawnBase* EnemyPawn);


	/*******************************************
	*  Hero Status
	*******************************************/
public:
	/**
	 * @brief 영웅의 현재 마나를 영웅의 지식 스탯에서 계산된 최대값으로 설정합니다.
	 */
	void SetFullMana();

	/**
 	* @brief 영웅에게 경험치를 추가하고 필요한 경우 레벨업 처리
 	* @param AddExperience 추가할 경험치의 양입니다.
 	*/
	void AddExp(int32 AddExperience);

	/**
	 * @brief 레벨 업 대화 상자 표시 및 게임 일시 중지
	 * @param GainedStats 레벨업 시 얻는 스탯
	 * @param LevelUpAmount 획득한 레벨 수
	 * @param CurrentLevel 영웅의 현재 레벨
	 */
	void ShowLevelUp(const FHeroStats& GainedStats, int32 LevelUpAmount, int32 CurrentLevel);


	/*******************************************
	*  Interaction Management 
	*******************************************/
public:
	/**
	* @brief지정된 행위자와 구성요소가 유효한 상호작용을 구성하는지 확인하고, 그렇다면 해당 상호작용을 시작합니다.
 	* @param Actor 상호 작용에 관여하는 배우.
 	* @param Component 상호 작용에 관련된 액터의 구성 요소입니다.
 	*/
	void DetectInteraction(AActor* Actor, UPrimitiveComponent* Component);

	/**
	 * @brief 겹치는 구성 요소와의 모든 현재 상호 작용을 업데이트하고 검증합니다.
	 */
	void RefreshInteractions();


	/*******************************************
	*  Movement Management 
	*******************************************/
public:
	/**
	 * @brief 경로를 표시하는 함수입니다.
	 * @param TargetLocation - 목표 위치입니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Explore Hero|Movement Management")
	void ShowPath(FVector TargetLocation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Explore Hero")
	void CheckMovement();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Explore Hero")
	void StopCheckingMovement();


	/*******************************************
	*  Input Controller 
	*******************************************/
public:
	/**
	* 플레이어의 입력이 차단 또는 허용되는지 여부를 제어합니다.
	* @param IsBlocked - true인 경우 플레이어 입력을 차단하는 부울입니다. false인 경우 플레이어 입력 허용
	*/
	void BlockPlayerInput(bool IsBlocked);


	/*******************************************
	 * On Event
	 *******************************************/
protected:
	UFUNCTION()
	void OnComponentBeginOverlap_InteractionDetector(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                                 UPrimitiveComponent* OtherComp,
	                                                 int32 OtherBodyIndex, bool bFromSweep,
	                                                 const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap_InteractionDetector(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                               UPrimitiveComponent* OtherComp,
	                                               int32 OtherBodyIndex);

	UFUNCTION()
	void OnBeginCursorOver_RightClickDetector(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnEndCursorOver_RightClickDetector(UPrimitiveComponent* TouchedComponent);
	
	UFUNCTION()
	void OnClicked_RightClickDetector(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnOkClickedHandle();
	
	UFUNCTION()
	void OnLevelUpClosed();

	UFUNCTION()
	void OnQuestRewardsClaimed();

	UFUNCTION()
	void OnGarrisonListClosedHandler();


	/*******************************************
    * Field Members
    *******************************************/

	/*--- 설정 (Settings) ---*/
public:
	// 최대 레벨
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Settings")
	int32 MaxLevel = 10;

	// 최대 캐릭터 제한
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Settings")
	int32 MaxCharacterLimit = 7;
	
	// 퀘스트 알림 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Base|Settings|Widget")
	TSubclassOf<UUW_QuestNotificationUI> WBP_QuestNotificationUIClass;

	// 레벨업 대화 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Base|Settings|Widget")
	TSubclassOf<UUW_LevelUpDialogue> WBP_LevelUpDialogueClass;

	// 청구 대화 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Base|Settings|Widget")
	TSubclassOf<UUW_ClaimDialogue> WBP_ClaimDialogueClass;

	// Garrison 목록 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Base|Settings|Widget")
	TSubclassOf<UUW_GarrisonListClass> WBP_GarrisonListClass;

	// 영웅 인벤토리 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Base|Settings|Widget")
	TSubclassOf<UUW_HeroInventory> WBP_HeroInventoryClass;
	

	// 아이들 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Base|Settings|Animation")
	UAnimSequence* IdleAnimation;

	// 걷는 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero Base|Settings|Animation")
	UAnimSequence* WalkAnimation;



	/*--- 영웅 속성 (Hero Properties) ---*/
public:
	// 연속 움직임 여부
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	bool bContinuousMovement = true;

	// 입력 차단 여부
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	bool bIsInputBlocked;

	// 경험치
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	int32 Exp;

	// 현재 마나
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	int32 CurrentMana;

	// 골드
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	int32 Gold;

	// 목표 줌
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	float TargetZoom = 800.0f;

	// 최소 줌
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	float MinZoom = 400.0f;

	// 최대 줌
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	float MaxZoom = 1200.0f;

	// 움직임 속도
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	float MovementSpeed = 400.f;

	// 경험치 지수
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	float ExpExponent = 15.0f;

	// 초기 회전
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	FRotator InitialRotation;

	// 초기 통계
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	FHeroStats InitialStats;

	// 첫 레벨 통계
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	FHeroStats FirstLevelStats;

	// 마지막 레벨 통계
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Hero Properties")
	FHeroStats LastLevelStats;


	/*--- 참조 (References) ---*/
public:
	// 탐험 플레이어 컨트롤러 참조
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|References")
	AExplorePlayerController* ExplorePlayerController;

	// 탐험 네비게이션 경로 참조
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|References")
	AExploreNavigationPath* NavigationPath;
	
	// 퀘스트 알림 UI 참조
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|References|Widget")
	UUW_QuestNotificationUI* WBP_QuestNotificationUI;
	
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|References|Widget")
	UUW_LevelUpDialogue* WBP_LevelUpDialogue;
	
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|References|Widget")
	UUW_ClaimDialogue* WBP_ClaimDialogue;
	
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|References|Widget")
	UUW_GarrisonListClass* WBP_GarrisonList;

	UPROPERTY(BlueprintReadWrite, Category="Hero Base|References|Widget")
	UUW_HeroInventory* WBP_HeroInventory;


	/*--- 컴포넌트 (Components) ---*/
public:
	// 스프링 암 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hero Base|Components")
	USpringArmComponent* SpringArm;

	// 카메라 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hero Base|Components")
	UCameraComponent* Camera;

	// 상호작용 탐지기 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hero Base|Components")
	USphereComponent* InteractionDetector;

	// 우클릭 탐지기 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hero Base|Components")
	UBoxComponent* RightClickDetector;


	/*--- 게임 플레이 관련 (Gameplay Related) ---*/
public:
	// 백팩
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Gameplay Related")
	TArray<TSubclassOf<AEquipmentBase>> BackPack;

	// 장비
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Gameplay Related")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;

	// 플레이어 캐릭터 리스트
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Gameplay Related")
	TArray<FPlayerCharacterData> PlayerCharacterList;

	// 활성 퀘스트
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Gameplay Related")
	TArray<AQuestBase*> ActiveQuests;

	// 활성 퀘스트 데이터
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Gameplay Related")
	TMap<TSubclassOf<AQuestBase>, FQuestStepData> ActiveQuestData;

	// 완료된 퀘스트 데이터
	UPROPERTY(BlueprintReadWrite, Category="Hero Base|Gameplay Related")
	TMap<TSubclassOf<AQuestBase>, int32> CompletedQuestData;


	/*--- 함수 로직 (Methods Logic Members) ---*/
private:
	FQuestRewards CurrentQuestRewards;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Explore Hero|Delegate")
	FOnGarrisonListClosed OnGarrisonListClosed;
};
