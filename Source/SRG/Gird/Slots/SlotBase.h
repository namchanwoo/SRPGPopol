#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "SlotBase.generated.h"

class UTextRenderComponent;
class AAuraBase;
class AUnitBase;
class ACharacterBase;
class AGrid;
class UPaperSprite;
class ABattleController;
class UPaperSpriteComponent;


/**
 * ASlotBase 클래스는 그리드 기반 게임 환경의 슬롯 또는 셀을 나타냅니다. 게임의 그리드 시스템의 필수적인 부분입니다.
 * 이 클래스의 각 인스턴스는 캐릭터 또는 장애물과 같은 유닛을 포함할 수 있으며 플레이어 캐릭터의 스폰 지점인지 여부,
 * 현재 움직일 수 있는지 여부, 이동과 관련된 총 비용 등 게임 플레이와 관련된 다양한 속성을 가지고 있습니다.
 * 또한 슬롯에 포함된 유닛에 대한 오라 적용을 처리합니다.
 * 이 클래스는 슬롯의 유형을 설정하고 슬롯이 비어 있는지 또는 장애물이 있는지 확인하고 연결된 오라를 관리하는 방법을 제공합니다.
 * 또한 슬롯을 강조 표시하고 총 비용을 표시하는 것과 관련된 기능을 제공합니다.
 */
UCLASS()
class SRG_API ASlotBase : public AActor
{
	GENERATED_BODY()

public:
	ASlotBase();
	
	void SetSlotSprite(UPaperSprite*& SlotSprite, const FName& SpriteName);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/*---	      	    Slot Initialization and Configuration    	      	---*/
public:
	// 주어진 설정으로 슬롯을 초기화합니다.
	void InitializeEvent(const FSlotSetting& InSlotSetting);

	// 슬롯의 유형을 설정합니다.
	void SetSlotType(ESlotType NewSlotType, int32 InSize = 1);

	// 슬롯에 포함된 단위를 설정합니다.
	void SetUnit(AUnitBase* InUnit);

	// 슬롯을 플레이어의 스폰 지점으로 설정합니다.
	UFUNCTION(CallInEditor, BlueprintCallable, Category="Slot Initialization and Configuration")
	void SetPlayerSpawnSlot();

	/*---	      	    Slot Aura Management    	      	---*/
public:
	// 슬롯에 있는 모든 오라를 포함된 유닛에 적용합니다.
	void ApplyAuraToContainedUnit();

	// 슬롯에 아우라를 추가합니다.
	void AddAura(AAuraBase* InAura, ACharacterBase* InAuraOwner);

	// 슬롯에서 특정 캐릭터의 모든 오라를 제거합니다.
	void RemoveAura(ACharacterBase* InCharacter);


	/*---	      	    Slot State Queries    	      	---*/
public:
	// 슬롯이 비어 있는지 확인합니다.
	UFUNCTION(BlueprintPure, Category="Slot State Queries")
	bool IsEmpty();

	// 슬롯에 주어진 캐릭터의 적이 있는지 확인합니다.
	UFUNCTION(BlueprintPure, Category="Slot State Queries")
	bool IsEnemySlot(ACharacterBase* InCharacter, ACharacterBase*& OutEnemyCharacter);

	// 슬롯에 주어진 캐릭터의 아군이 포함되어 있는지 확인합니다.
	UFUNCTION(BlueprintPure, Category="Slot State Queries")
	bool IsAllySlot(ACharacterBase* InCharacter, ACharacterBase*& OutAllyCharacter);

	// 슬롯에 장애물이 있는지 확인합니다.
	UFUNCTION(BlueprintPure, Category="Slot State Queries")
	bool ContainsObstacle();

	//
	FSlotSprites GetSlotSpriteStruct(int32 InSize);
	
	/*---	      	    Slot Display and Interaction   	      	---*/
public:
	// 슬롯과 관련된 총 비용을 표시합니다.
	void ShowTotalCost();

	// 슬롯을 숨깁니다.
	void HideSlot();

	// 슬롯을 이동 가능하거나 이동 불가능하게 만듭니다.
	void Movable(bool IsOn);

	// 슬롯을 강조 표시하거나 강조 표시 해제합니다.
	void Highlight(bool IsOn, int32 InSize);

	// 슬롯을 적 슬롯으로 설정합니다.
	void SetEnemySlot(int32 InSize);

	// 슬롯을 아군 슬롯으로 설정합니다.
	void SetAllySlot(int32 InSize);

	// 슬롯을 주문 슬롯으로 설정합니다.
	void SetSpellSlot();


	/*---	      	    Slot Pathfinding and Movement   	      	---*/
public:
	// 슬롯과 관련된 이동 데이터를 재설정합니다.
	void ResetMovementData(bool IsOverlayData);

	// 슬롯의 현재 유형을 저장합니다.
	void SaveSlotType();

	// 이 슬롯을 공격할 수 있는 슬롯 목록에 슬롯을 추가합니다.
	void AddSlotToCanAttackFromSlots(ASlotBase* InSlot);

	// 이 슬롯을 공격할 수 있는 슬롯 목록을 지웁니다.
	void ClearCanAttackFromSlots();


	/*---	      	    Slot Event Handlers	      	---*/
public:
	// 주문이 비활성화되는 이벤트를 처리합니다.
	void OnSpellDisabled();

	// 슬롯과 관련된 모든 콜백을 지웁니다.
	virtual void ClearAllCallbacks();

protected:
	// 슬롯에서 캐릭터가 제거되는 이벤트를 처리합니다.
	UFUNCTION()
	void OnCharacterRemovedFromSlotHandler(ACharacterBase* InCharacter);


	/*******************************************
 	* Slot Configuration
 	*******************************************/
public:
	// 슬롯과 관련된 설정입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot|Configuration")
	FSlotSetting SlotSetting;

	// 슬롯의 현재 유형입니다.
	UPROPERTY(BlueprintReadWrite, Category="Slot|Configuration")
	ESlotType SlotType;

	// 슬롯의 이전 유형입니다.
	UPROPERTY(BlueprintReadWrite, Category="Slot|Configuration")
	ESlotType PreviousSlotType;

	// 그리드의 슬롯 좌표입니다.
	UPROPERTY(BlueprintReadWrite, Category="Slot|Configuration")
	FVector2D Coordinates;

	/*******************************************
	 * Slot State
	 *******************************************/
public:
	// 플레이어 캐릭터가 이 슬롯에서 스폰될 수 있는지 여부를 결정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot|State")
	bool bCanPlayerCharacterSpawn;

	// 슬롯이 이동 가능한지 여부를 나타냅니다.
	UPROPERTY(BlueprintReadWrite, Category="Slot|State")
	bool bIsMovable;

	// 슬롯이 숨겨져 있는지 여부를 나타냅니다.
	UPROPERTY(BlueprintReadWrite, Category="Slot|State")
	bool bIsHiddenSlot;

	/*******************************************
	 * Slot Cost
	 *******************************************/
public:
	// 이 슬롯으로 이동하는 것과 관련된 총 비용입니다.
	UPROPERTY(BlueprintReadWrite, Category="Slot|Cost")
	int32 TotalCost;

	/*******************************************
	 * Slot Content
	 *******************************************/
public:
	// 슬롯에 포함된 유닛(있는 경우)입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot|Content")
	AUnitBase* ContainedUnit;

	// 슬롯에 적용되는 오라는 각 소유자에게 매핑됩니다.
	UPROPERTY(BlueprintReadOnly, Category="Slot|Content")
	TMap<AAuraBase*, ACharacterBase*> Auras;

	/*******************************************
	 * Slot Pathfinding
	 *******************************************/
public:
	// 경로 찾기 컨텍스트의 상위 슬롯입니다.
	UPROPERTY(BlueprintReadWrite, Category="Slot|Pathfinding")
	ASlotBase* Parent;

	// 이 슬롯의 경로입니다.
	UPROPERTY(BlueprintReadOnly, Category="Slot|Pathfinding")
	TArray<ASlotBase*> Path;

	// 이 슬롯에 대한 오버레이 경로입니다.
	UPROPERTY(BlueprintReadOnly, Category="Slot|Pathfinding")
	TArray<ASlotBase*> OverlayPath;

	// 이 슬롯을 공격할 수 있는 슬롯입니다.
	UPROPERTY(BlueprintReadOnly, Category="Slot|Pathfinding")
	TArray<ASlotBase*> CanAttackFromSlots;

	/*******************************************
	 * Slot Grid
	 *******************************************/
public:
	// 슬롯이 속한 그리드입니다.
	UPROPERTY(BlueprintReadWrite, Category="Slot|Grid")
	AGrid* Grid;

	/*******************************************
	 * Slot Components
	 *******************************************/
public:
	// 기본 장면 루트 구성 요소입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slot|Components")
	USceneComponent* DefaultSceneRoot;

	// 총 비용을 표시하는 데 사용되는 텍스트 구성 요소입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slot|Components")
	UTextRenderComponent* TotalCostText;

	// 슬롯을 시각적으로 나타내는 데 사용되는 스프라이트 구성 요소입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slot|Components")
	UPaperSpriteComponent* PaperSprite;
};
