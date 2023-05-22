#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SRGCore/SRGEnumStruct.h"
#include "SRGGameInstance.generated.h"

class ABattleHeroBase;
class AShopBase;
class AQuestBase;
class URPGSaveGame;
class AObstacleBase;
class ABattleQuestBase;
class AInteractionDetector;
class AEquipmentBase;



/**
 *USRGGameInstance는 게임 상태, 맵 간 전환, SRG 프로젝트의 퀘스트, 상점 및 영웅 변형과 같은 요소를 관리합니다.
 */
UCLASS()
class SRG_API USRGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USRGGameInstance(const FObjectInitializer& ObjectInitializer);


	

	/*******************************************
 	* Game Initialization
 	*******************************************/
public:
	/**
	@brief 새 게임이 시작될 때 필요한 조치를 수행합니다.
	이 함수는 새로운 게임이 시작될 때 게임 관련 데이터 및 상태를 초기화하기 위해 호출됩니다.
	*/
	void OnNewGameStarted();

	/**
	@brief 영웅, 상호 작용 가능 항목, 퀘스트, 상점, 영웅 변형 및 경험을 포함한 게임 데이터를 로드합니다.
	*/
	void LoadGame();

	/**
	@brief 지정된 RPG 저장 게임 데이터를 불러와 게임 불러오기를 준비합니다.
	@param InRPGSaveGame 로드할 RPG 저장 게임 데이터입니다.
	*/
	void LoadThisGame(URPGSaveGame* InRPGSaveGame);

	bool GetShouldLoadGame();


	/*******************************************
	 * Game Save and Load Operations
	 *******************************************/
public:
	/**
	@brief 지정된 슬롯에 게임 데이터를 저장합니다.
	@param InSlotName 저장 슬롯의 이름입니다.
	*/
	void SaveGame(FString InSlotName);

	/**
	@brief 저장된 게임 데이터에서 상호 작용 가능한 개체를 로드합니다.
	*/
	void LoadInteractables();

	/**
	@brief 저장된 게임 데이터에서 영웅 캐릭터를 불러옵니다.
	*/
	void LoadHero();

	/**
	@brief 저장된 게임 데이터에서 지정된 퀘스트를 불러옵니다.
	@param Quest 로드 퀘스트.
	*/
	void LoadQuest(AQuestBase* Quest);

	/**
	@brief 저장된 게임 데이터에서 모든 퀘스트를 불러옵니다.
	*/
	void LoadQuests();

	/**
	 * @brief 레벨의 모든 ShopBase 액터에 대한 상점 데이터를 로드합니다.
	 */
	void LoadShops();

	/**
 	* @brief게임 로딩 중 영웅 캐릭터의 위치를 불러옵니다.
 	* 이 함수는 저장된 게임 데이터를 기반으로 영웅 캐릭터의 변형 로드, 위치 및 회전 설정을 담당합니다.
 	* 또한 부드러운 전환을 위해 카메라 지연 효과를 일시적으로 비활성화하고 0.5초의 짧은 지연 후에 다시 활성화합니다.
 	*/
	void LoadHeroTransform();


	/*******************************************
	 * Hero Operations
	 *******************************************/
public:
	/**
	* @brief 탐험 영웅의 상태를 세이브 게임 인스턴스에 저장합니다.
	*/
	void SaveExploreHero();

	/**
	* @brief 전투 영웅의 상태를 저장 게임 인스턴스에 저장합니다.
	* @param InBattleHero 상태를 저장할 전투 영웅에 대한 포인터입니다.
	*/
	void SaveBattleHero(ABattleHeroBase* InBattleHero);

	/**
	* @brief 전투에서 살아남은 플레이어 캐릭터 목록을 검색합니다.
	* @param InBattleHero 팀을 분석할 전투 영웅에 대한 포인터입니다.
	* @return 살아남은 캐릭터를 나타내는 FPlayerCharacterData 인스턴스의 배열입니다.
	*/
	TArray<FPlayerCharacterData> GetSurvivingPlayerCharacters(ABattleHeroBase* InBattleHero);

	/**
	* @brief 플레이어의 합계에 경험치를 추가합니다.
	*/
	void AddExp();


	/*******************************************
	 * Quest Operations
	 *******************************************/
public:
	/**
	* @brief 게임 인스턴스의 활성 퀘스트에 퀘스트를 추가합니다.
	* @param InQuest 추가되는 퀘스트입니다.
	*/
	void AddQuest(AQuestBase* InQuest);

	/**
	* @brief 게임 인스턴스의 활성 퀘스트에서 퀘스트를 제거합니다.
	* @param InQuest 제거할 퀘스트입니다.
	*/
	void RemoveQuest(AQuestBase* InQuest);

	/**
	 * @brief 제공된 퀘스트 배열을 기반으로 BattleQuest를 업데이트합니다.
	 * @param AllQuests 반복할 퀘스트의 배열입니다.
	 */
	void UpdateBattleQuest(const TArray<AQuestBase*>& AllQuests);


	/**
	* @brief 추가 참조를 위해 게임 인스턴스에 현재 전투 퀘스트를 저장합니다.
	* @param InBattleQuest 저장 할 전투 퀘스트
	*/
	void SaveBattleQuest(ABattleQuestBase* InBattleQuest);


	/*******************************************
	 * Interaction Operations
	 *******************************************/
public:
	/**
	* @brief 게임 플레이 중 추적 및 액세스를 위해 게임 인스턴스에 상호 작용을 추가합니다.
	* @param InteractableBase 추가 할 인터랙션
	*/
	void AddInteractions(AInteractionDetector* InteractableBase);


	/*******************************************
	 * Shop Operations
	 *******************************************/
public:
	/**
 	* 게임 인스턴스에 모집 상점을 추가합니다.
 	* @param InShop 추가할 상점입니다.
 	* @param InValue 상점과 관련된 채용 목록입니다.
 	*/
	void AddRecruitShop(AShopBase* InShop, FShopRecruitmentList InValue);


	/**
	 * 저장 게임에 장비 상점을 추가합니다.
	 * @param InShop 추가할 상점입니다.
	 * @param InEquipment 상점과 관련된 장비 목록입니다.
	 */
	void AddEquipmentShop(AShopBase* InShop, FShopEquipmentList InEquipment);


	/*******************************************
	 * Map Operations
	 *******************************************/
public:
	/**
 	* @brief 맵을 변경합니다.
 	* @param InMapName 변경 될 맵 이름입니다.
 	*/
	void ChangeMap(FName InMapName);

	/**
	@brief 전투를 위해 맵을 변경하고 필요한 데이터를 설정하고 새 맵으로 전환합니다.
	@param InEnemyList 전투를 위한 적 캐릭터 데이터 목록입니다.
	@param InObstacleList 장애물 클래스의 맵과 전투의 카운트.
	@param InGold 전투를 위한 금의 양입니다.
	@param InBattleQuest 저장하고 진행하는 전투 퀘스트.
	@param InDrops 전투 중 드롭되는 장비 클래스 목록입니다.
	@param InMapName 전환할 맵의 이름입니다.
	*/
	void ChangeMapForBattle(const TArray<FEnemyCharacterData>& InEnemyList,
	                        const TMap<TSubclassOf<AObstacleBase>, int32>& InObstacleList, int32 InGold,
	                        ABattleQuestBase* InBattleQuest, const TArray<TSubclassOf<AEquipmentBase>>& InDrops,
	                        const FName& InMapName);

	/**
	@brief 전투 후 맵을 변경합니다.
	@param InBattleHero 전투에 참가한 전투 영웅.
	@param InExpReward 전투에서 얻은 경험치 보상입니다.
	 */
	void ChangeMapAfterBattle(ABattleHeroBase* InBattleHero, int32 InExpReward);


	/*******************************************
 	* Help Functions
 	*******************************************/
protected:
	/**
	@brief 저장 게임 인스턴스(RpgSaveGame)가 초기화되었는지 확인합니다. 그렇지 않은 경우 새 게임 인스턴스를 만듭니다.
	*/
	void EnsureSaveGameInitialized();

	/**
	* @brief 현재 레벨 이름과 액터 이름을 기반으로 지정된 액터에 대한 고유 식별자 문자열을 생성합니다.
	* @param Actor 고유 식별자를 생성할 액터입니다.
	* @return 액터의 고유 식별자 문자열입니다.
	 */
	FString GenerateUniqueIdentifier(AActor* Actor);

	/**
	 * @brief 
	 * @param PlayerCharacter 
	 * @return 
	 */
	FPlayerCharacterData GeneratePlayerCharacterData(ACharacterBase* PlayerCharacter);


	/*******************************************
 	* Field Members
 	*******************************************/
public:
	// 게임을 로드해야 하는지 여부를 결정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	bool bShouldLoadGame;

	// 영웅의 변환을 로드해야 하는지 여부를 결정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	bool bShouldLoadTransform;

	// 적 폰을 로드할지 여부를 결정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	bool bShouldLoadEnemyPawns;

	// 영웅에 경험치를 추가할지 여부를 결정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	bool bShouldAddExp;

	// 현재 금의 양을 저장합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	int32 Gold;

	// 전투에서 얻은 경험치를 저장합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	int32 ExpFromBattle;

	// 저장 슬롯의 이름을 저장합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	FString SlotName;

	// 현재 전투 퀘스트의 이름을 저장합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	FString BattleQuest;

	// RPG 저장 게임 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	URPGSaveGame* RpgSaveGame;

	// 적 캐릭터 목록 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	TArray<FEnemyCharacterData> EnemyCharacterList;

	// 장비 드롭 목록을 저장합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	TArray<TSubclassOf<AEquipmentBase>> Drops;

	// 장애물 목록과 해당 수량을 저장합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SRG Game Instance")
	TMap<TSubclassOf<AObstacleBase>, int32> ObstacleList;
};
