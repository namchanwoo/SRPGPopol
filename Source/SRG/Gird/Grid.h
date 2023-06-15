#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Slots/SlotBase.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "Grid.generated.h"

class AAreaTargetSpellBase;
class AAreaTargetActiveAbilityBase;
class AGridInnerOverlaySlot;
class AInnerSlot;
class AGridOverlaySlot;
class ASlot;
class UUW_DamageDetailsListUI;
class ABattleController;
class AObstacleBase;
class ASlotBase;
class AEnemySpawnLocation;
class AHeroSpawnLocation;
class ASpellBase;
class AActiveAbilityBase;
class USplineComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCursorChanged, EBattleCursorType,
                                              InCursorType, AActiveAbilityBase*, InActiveAbility, ASpellBase*,
                                              InSpell, TSubclassOf<ACharacterBase>, InCharacter);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCursorAngleChanged, float, InAngle);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseOverSlot, ASlotBase*, InSlot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseOutOfGrid);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSlotClicked, ASlotBase*, InSlot);

/*
 * AGrid 클래스는 전략 롤플레잉 게임(SRPG)의 격자 시스템을 제어하는 액터입니다.
 * 이 클래스는 플레이어의 이동 가능한 슬롯을 계산하고,플레이어의 마우스 위치에 가장 가까운 슬롯을 탐지하는 등의 기능을 수행합니다.
 * 그리드의 모든 슬롯은 AGrid 클래스에 의해 관리되며,이 클래스는 각 슬롯의 정보를 업데이트하고 전투 상황에 따라 다른 상호작용을 제공합니다.
 * 또한 AGrid 클래스는 전투 상태 변화에 대한 처리 로직도 포함하고 있습니다.
 * 플레이어가 스펠을 시전하거나 턴을 진행하는 등의 전투 상태 변화가 발생하면 이에 따른 UI 업데이트, 슬롯 강조 표시 등의 작업을 수행합니다.
 * 이 클래스는 AActor를 상속받아 Unreal Engine의 액터 생명주기와 이벤트를 활용합니다.
 * 그리고 클래스는 'SRPG' 그룹에 속하며,블루프린트에서 인스턴스화할 수 있도록 설정되어 있습니다.
 */
UCLASS(ClassGroup = ("SRPG"), meta = (BlueprintSpawnableComponent),
	HideCategories = ("HLOD","Rendering","Collision", "Physics", "WorldPartition",
		"Input", "Cooking","Replication", "DataLayers","Event"))
class SRG_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	AGrid();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


	/*---	           Grid Initialization and Event Handling         ---*/
public:
	/**
	 * @brief 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Grid Event")
	void InitializeEvent();
	virtual void InitializeEvent_Implementation();

	/**
	 * @brief 
	 */
	UFUNCTION(CallInEditor, BlueprintCallable, Category="Grid Event")
	void ReCreateSlots();

	/**
	 * @brief 
	 */
	UFUNCTION(CallInEditor, BlueprintCallable, Category="Grid Event")
	void RemoveAllSlots();

	/**
	 * @brief 
	 */
	UFUNCTION(CallInEditor, BlueprintCallable, Category="Grid Event")
	void Refresh();

protected:
	// 이 메서드는 그리드의 외부 슬롯을 만들고 초기화합니다.
	void CreateOuterSlots(UWorld* World);

	// 이 메서드는 그리드의 내부 슬롯을 만들고 초기화합니다.
	void CreateInnerSlots(UWorld* World);

	// 그리드에서의 위치에 따라 슬롯의 변환 계산
	FTransform CalculateSlotTransform(int32 Row, int32 Column);

	// 이 메서드는 그리드에서의 위치에 따라 내부 슬롯의 변환을 계산합니다.
	FTransform CalculateInnerSlotTransform(int32 Row, int32 Column);

	// 지정된 위치에 새 슬롯을 생성하고 초기화합니다.
	ASlot* CreateAndInitializeSlot(UWorld* World, const FTransform& SlotTransform, int32 Column, int32 Row);

	// 지정된 위치에서 오버레이 슬롯 생성 및 초기화
	AGridOverlaySlot* CreateAndInitializeOverlaySlot(UWorld* World, FTransform& SlotTransform, int32 Column, int32 Row);

	// 이 메서드는 새 내부 슬롯을 만들고 초기화합니다.
	AInnerSlot* CreateAndInitializeInnerSlot(UWorld* World, FTransform& InnerSlotTransform, int32 Column, int32 Row);

	// 이 메서드는 새 내부 오버레이 슬롯을 만들고 초기화합니다.
	AGridInnerOverlaySlot* CreateAndInitializeInnerOverlaySlot(UWorld* World, FTransform& InnerSlotTransform,
	                                                           int32 Column,
	                                                           int32 Row);

private:
	/**
	 * @brief 슬롯을 삭제하고 연관된 액터들을 파괴하는 도우미 함수
	 * @param SlotMap 파괴할 슬롯의 맵
	 * @param bClearCallbacks 콜백을 제거할지 여부
	 */
	void DestroySlots(TMap<FVector2D, ASlotBase*>& SlotMap, bool bClearCallbacks);

protected:

	/**
 	 * OnBattleStateChanged 이벤트에 대한 이벤트 처리기입니다.
 	 * 새로운 전투 상태에 따라 논리를 처리합니다.
 	 * @param NewBattleState 새로운 전투 상태.
 	 */
	UFUNCTION()
	void OnBattleStateChangedHandler(EBattleState NewBattleState);

	/**
	 * @brief 
	 */
	UFUNCTION()
	void OnMovableSlotTimeline_Detect();


	/*---	           Grid Slot Handling         ---*/
public:
	/**/
	void ResetCurrentSlotUnderMouse();

	/**/
	void CheckSlotUnderMouse();

	/**/
	void ResetSlots();

	/**
	 * @brief 
	 * @param InSlotTransform 
	 */
	void SetHeroSpawnPoint(const FTransform& InSlotTransform);

protected:
	/**
	 * @brief 
	 * @param InSlot 
	 * @param bIsInnerSlot 
	 */
	UFUNCTION()
	void OnMouseOverSlot(ASlotBase* InSlot, bool bIsInnerSlot);

	/**
	 * @brief 
	 * @param InSlot 
	 * @param InButton 
	 */
	UFUNCTION()
	void OnSlotClicked(ASlotBase* InSlot, FKey InButton);

	/**
	 * @brief 
	 * @param InSlot 
	 */
	UFUNCTION()
	void OnMouseLeaveSlot(ASlotBase* InSlot);

private:
	// 슬롯을 재설정하고 마우스 아래의 현재 슬롯을 null로 설정하고 전투 상태를 PlayerIsPlaying으로 변경합니다.
	void FinishAction();
	// 배치 단계를 처리하는 함수
	void HandleDeploymentPhase(ASlotBase* InSlot);
	// 플레이어 액션 대기 단계를 처리하는 함수
	void HandleWaitingForPlayerActionPhase(ASlotBase* InSlot, bool bIsInnerSlot);
	// 아군 대상 능력을 처리하는 함수
	void HandleAllyTargetAbility(ASlotBase* InSlot);
	// 범위 내의 슬롯을 처리하는 함수
	void HandleSlotInRange(ASlotBase* InSlot);
	// 적 대상 능력을 처리하는 함수
	void HandleEnemyTargetAbility(ASlotBase* InSlot, bool bIsInnerSlot);
	// 능력 슬롯을 강조 표시하는 함수
	void HighlightAbilitySlots();
	// 커서 타입을 업데이트하는 함수
	void UpdateCursorType();
	// 범위 내 공격 유형을 결정하는 함수
	void DetermineRangeAttackType(ASlotBase* InSlot);
	// 클릭한 슬롯의 유효성 및 상태에 따라 전투 동작을 처리합니다.
	void HandleBattleClick(ASlotBase* InSlot);
	// 능력을 활성화하고, 슬롯을 재설정하고, 마우스 아래의 현재 슬롯을 null로 설정하고, 전투 상태를 PlayerIsPlaying으로 변경합니다.
	void ActivateAbility(ASlotBase* InSlot);
	// 공격 유형에 따라 적의 공격이나 움직임의 논리를 처리합니다.
	void HandleEnemyAttackOrMovement(ASlotBase* InSlot);
	// 범위 공격을 수행하고, 슬롯을 재설정하고, 마우스 아래의 현재 슬롯을 null로 설정하고, 전투 상태를 PlayerIsPlaying으로 변경합니다.
	void RangeAttack(ASlotBase* InSlot);
	// 슬롯이 적 슬롯인지 여부에 따라 근접 공격 또는 이동의 논리를 처리합니다.
	void HandleMeleeAttackOrMovement(ASlotBase* InSlot, bool IsEnemySlot);
	// 슬롯이 플레이 캐릭터의 슬롯이면 근접 공격을 수행하고 그렇지 않으면 플레이 캐릭터를 슬롯으로 이동합니다.
	void MeleeAttackOrMoveEnemy(ASlotBase* InSlot);
	// 슬롯이 이동 가능한 슬롯인 경우 플레이 중인 캐릭터를 슬롯으로 이동시킵니다.
	void MoveAction(ASlotBase* InSlot);


	/*---	           Grid Movement and Slot Highlighting        ---*/
public:
	/**/
	void ResetMovementData();

	/**
	 * @brief 
	 * @param InCharacter 
	 */
	void ShowMovementForPlayerCharacter(ACharacterBase* InCharacter);

	/**
	 * @brief 
	 * @param InCharacter 
	 * @param InTargetSlot 
	 */
	void ShowMovementPath(ACharacterBase* InCharacter, ASlotBase* InTargetSlot);

	/**/
	void ClearMovementPath();

	/**/
	void HighlightMovementSlots(ASlotBase* InSlot, int32 InSize);

	/**
	 * @brief 주어진 캐릭터의 가능한 움직임을 보여줍니다.
	 * 이 기능은 캐릭터가 이동할 수 있는 가능한 슬롯을 계산하고 강조 표시합니다.
	 * @param InCharacter 움직임을 보여주고 싶은 캐릭터.
	 */
	void ShowCharacterMovementOverlay(ACharacterBase* InCharacter);

	/**/
	void ClearAttackSlots();

	/**/
	void RemoveHighlightOnCharacters();

	/**
 	 * 그리드의 특정 슬롯을 강조 표시합니다. 매개변수 'IsOn'에 따라 기능은 슬롯과 그 이웃을 표시하거나 숨길 수 있습니다.
 	 * @param InSlot 강조 표시할 슬롯입니다.
 	 * @param InSize 슬롯의 크기입니다.
 	 * @param IsOn 슬롯을 강조 표시해야 하는지(true) 또는 숨겨야 하는지(false) 여부입니다.
 	 */
	void HighlightSlot(ASlotBase* InSlot, int32 InSize, bool IsOn);

	/**
	 * @brief 
	 * @param InSlot 
	 */
	void HighlightPlayingCharacterSlot(ASlotBase* InSlot);

	/**
	 * @brief 
	 * @param InSlot 
	 */
	void HighlightEnemySlots(ASlotBase* InSlot);

	/**
	 * @brief 
	 * @param InCharacter 
	 * @param OnlyVisual 
	 * @param OutMoveAbleSlots 
	 * @param OutSlotsInRange 
	 * @param OutEnemySlots 
	 * @param OutAllySlots 
	 * @return 
	 */
	bool CalculateMovableSlots(ACharacterBase* InCharacter, bool OnlyVisual, TArray<ASlotBase*>& OutMoveAbleSlots,
	                           TArray<ASlotBase*>& OutSlotsInRange, TArray<ASlotBase*>& OutEnemySlots,
	                           TArray<ASlotBase*>& OutAllySlots);

private:
	// 스플라인 포인트를 준비하는 함수입니다.
	void PrepareSplinePoints(ACharacterBase* InCharacter, ASlotBase* InTargetSlot,
	                         const TMap<FVector2D, ASlotBase*>& AllSlots);
	// 스플라인 접선을 조정하는 함수입니다.
	void AdjustSplineTangents(int PointIndex);
	// 경로 메시를 렌더링하는 함수입니다.
	void RenderPathMesh();
	// 경로 메시 인스턴스를 추가하는 함수입니다.
	void AddPathMeshInstance(int Index);
	// 이전 경로 메시 인스턴스의 회전을 업데이트하는 함수입니다.
	void UpdatePathMeshInstanceRotation(int Index);
	// 마지막 경로 메시 인스턴스의 회전을 업데이트하는 함수입니다.
	void UpdateLastPathMeshInstanceRotation();
	//주어진 캐릭터의 오버레이 슬롯을 계산하고 저장합니다. 	
	TArray<ASlotBase*> CalculateAndStoreOverlaySlots(ACharacterBase* InCharacter);
	//주어진 캐릭터에 대한 이동 오버레이의 모든 슬롯을 강조 표시합니다.
	void HighlightMovementOverlaySlots(ACharacterBase* InCharacter);
	//그리드에서 빈 슬롯을 강조 표시합니다.	
	void HighlightEmptySlot(ASlotBase* InSlot);
	//적을 포함하는 슬롯을 강조 표시합니다.	
	void HighlightEnemySlot(ACharacterBase* InCharacter, ASlotBase* InSlot);
	//적 캐릭터의 이동 슬롯을 계산합니다. 	
	void MovementSlotsForEnemy(ACharacterBase* InCharacter, ASlotBase* InSlot);
	//크기가 짝수이고 현재 플레이 중인 캐릭터가 차지하지 않는 슬롯을 숨깁니다.
	void HideEvenSizedNonPlayerSlots(ASlotBase* CurrentSlot, int32 CurrentSize);
	//슬롯을 표시할지 숨길지에 따라 범위 내 슬롯의 가시성을 조정합니다. 'IsOn'이 true이면 슬롯을 숨기고 그렇지 않으면 강조 표시합니다. 	 
	void AdjustSlotsInRangeVisibility(const TArray<ASlotBase*>& OutSlotsInRange, bool IsOn, ASlotBase* CurrentSlot);

	bool DetermineMeleeAttack(ACharacterBase* CurrentCharacter);
	void ResetDataBasedOnVisual(bool CurrentOnlyVisual, ACharacterBase* CurrentCharacter);

	void HandleSlotBasedOnMeleeAttack(bool CurrentIsMeleeAttack, ACharacterBase* CurrentCharacter, ASlotBase* InSlot,
	                                  TArray<ASlotBase*>& CurrentAllySlots, TArray<ASlotBase*>& CurrentEnemySlots);

	void HandleMovableSlot(ACharacterBase* CurrentCharacter, bool CurrentOnlyVisual, ASlotBase* InSlot,
	                       TArray<ASlotBase*>& CurrentMovableSlots);

	void HandleWalkMovementType(ACharacterBase* CurrentCharacter, bool CurrentOnlyVisual,
	                            TArray<ASlotBase*>& OpenSlots, TArray<ASlotBase*>& ClosedSlots,
	                            TArray<ASlotBase*>& CurrentSlotsInRange, TArray<ASlotBase*>& CurrentMovableSlots,
	                            TArray<ASlotBase*>& CurrentAllySlots, TArray<ASlotBase*>& CurrentEnemySlots);

	void HandleFlyMovementType(ACharacterBase* CurrentCharacter, bool CurrentOnlyVisual,
	                           TArray<ASlotBase*>& CurrentMovableSlots, TArray<ASlotBase*>& CurrentSlotsInRange,
	                           TArray<ASlotBase*>& CurrentAllySlots, TArray<ASlotBase*>& CurrentEnemySlots);

	void HandleImmobilieMovementType(ACharacterBase* CurrentCharacter, bool CurrentOnlyVisual,
	                                 TArray<ASlotBase*>& CurrentSlotsInRange, TArray<ASlotBase*>& CurrentAllySlots,
	                                 TArray<ASlotBase*>& CurrentEnemySlots);


	/*---	           Grid UI and Overlay Handling         ---*/
public:
	/**/
	void ResetOverlayData();
	void HideOverlaySlots();
	void HideDamageDetailsUI();
	void ShowDamageDetailsUI(ASlotBase* InTargetSlot, ASlotBase* InStandingSlot);
	void ShowDamageDetailsUIForAreaAbility();
	void ShowDamageDetailsUIForAreaSpell();
	void OnMouseOverUI();

	/*---	           Grid Spells, Abilities, and Event Handling         ---*/
public:
	void OnAreaTargetSpellEnabled(AAreaTargetSpellBase* InAreaTargetSpell);
	void OnAreaTargetSpellDisabled();
	void OnAreaTargetAbilityEnabled(AAreaTargetActiveAbilityBase* InAreaTargetActiveAbility);
	void OnAreaTargetAbilityDisabled();
	void OnEnemyTargetSpellEnabled();
	void OnEnemyTargetSpellDisabled();
	void OnAllyTargetSpellEnabled();
	void OnAllyTargetSpellDisabled();
	void OnAllyTargetAbilityEnabled();
	void OnAllyTargetAbilityDisabled();
	void ShowSpellSlots(ASlotBase* InTargetSlot);
	void ShowAreaTargetAbilitySlots(ASlotBase* InTargetSlot);
	void SetSpellTarget(ASlotBase* InTargetSlot, ACharacterBase* InTargetCharacter);
	void CastSpellOnSlot(ASlotBase* InTargetSlot);
	void StopDetectingClosestMovableSlot();

	/*---	           Grid Helper Functions         ---*/
public:
	int32 UnitSizeAdjustmentAmount(int32 InUnitSize);

	TArray<ASlotBase*> GetNeighbourSlots(ASlotBase* InSlot, int32 InCharacterSize, bool OnlyEmpty);

	int32 GetDistanceCost(ASlotBase* Slot1, ASlotBase* Slot2);

	ASlotBase* GetLowestTravelCostSlot(const TArray<ASlotBase*>& InSlots);

	UFUNCTION(BlueprintCallable, Category="Grid Event")
	void GetSlotsInRange(ASlotBase* InSlot, int32 InRange, bool OnlyEmptySlots, TArray<ASlotBase*>& OutSlotsInRange,
	                     bool& OutAllFree);

	ASlotBase* GetClosestSlotInRange(ASlotBase* InSlot, const TArray<ASlotBase*>& InSlots);

	UFUNCTION(BlueprintCallable, Category="Grid Event")
	void GetSlotsInCharacterRange(ACharacterBase* InCharacter, int32 InRange, ASlotBase* InStartingSlot,
	                              bool ExcludeSelf, TArray<ASlotBase*>& OutSlotsInRange,
	                              TArray<ASlotBase*>& OutEmptySlotsInRange,
	                              TArray<ACharacterBase*>& OutAlliesInRange,
	                              TArray<ACharacterBase*>& OutEnemiesInRange);

	bool IsEnemyAdjacent(ACharacterBase* InCharacter);

	float GetDistanceBetweenCoordinates(FVector2D Coordinate1, FVector2D Coordinate2);

	float GetDistanceToSlot(ASlotBase* Slot1, ASlotBase* Slot2);

	ASlotBase* GetClosestMovableSlot(ASlotBase* InSlot, int32 InUnitSize, const TArray<ASlotBase*>& InMovableSlots);

	ASlotBase* GetClosestSlotToMouse(ASlotBase* InSlotToAttack, const TArray<ASlotBase*>& InMovableSlots);

	ASlotBase* GetClosestSlotToMouseImproved(ASlotBase* InSlotToAttack, const TArray<ASlotBase*>& InMovableSlots);

	TMap<FVector2D, ASlotBase*>& GetAllOverlaySlots(int32 InUnitSize);

	TMap<FVector2D, ASlotBase*>& GetAllSlots(int32 InUnitSize);

	/*---	           Grid Spawn and Remove Units         ---*/
public:
	void RemoveEnemySpawnLocation();
	bool SpawnCharacter(TSubclassOf<ACharacterBase> InUnitClass, FVector2D InCoordinates, bool IsAI, int32 InStack,
	                    const FHeroStats& InHeroStats, ACharacterBase*& OutSpawnedCharacter);

	void RemoveUnitOnSlot(ACharacterBase* InUnit);
	TArray<ASlotBase*> GetSlotsForObstacleSpawn();
	bool SpawnObstacle(TSubclassOf<AObstacleBase> InObstacle, FVector2D InCoordinates,
	                   AObstacleBase*& OutSpawnedObstacle);

	void SetUnitOnSlot(AUnitBase* InUnit, ASlotBase* InSlot);
	bool CanUnitFitTo(ASlotBase* InSlot, AUnitBase* InUnit, TArray<ASlotBase*>& OutAllSlotsUnitWillStandOn);

	void GetAllCharacterSlots(TArray<ASlotBase*>& OutAllySlots, TArray<ASlotBase*>& OutEnemySlots);

	TArray<ASlotBase*> GetAttackSlots(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter);


	
	/*******************************************
	 * Field Members
	 *******************************************/

	/*---	           Grid Settings         ---*/
public:
	// 이 Grid가 Hex 형태인지 여부를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid|Settings")
	bool bIsHexGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid|Settings")
	bool bIsMeleeAttack; // 이 Grid가 Melee 공격을 사용하는지 여부를 나타냅니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid|Settings")
	int32 NumberOfRows = 7; // Grid의 행의 수를 나타냅니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid|Settings")
	int32 NumberOfColumns = 12; // Grid의 열의 수를 나타냅니다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid|Settings")
	int32 PlayerSpawnColumn = 2; // 플레이어가 스폰되는 열을 나타냅니다.

	/*---	           Grid Abilities         ---*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Grid|Abilities")
	bool bEnemyTargetSpellEnabled; // 적 대상 주문이 활성화되어 있는지 여부를 나타냅니다.

	UPROPERTY(BlueprintReadWrite, Category="Grid|Abilities")
	bool bAllyTargetAbilityEnabled; // 아군 대상 능력이 활성화되어 있는지 여부를 나타냅니다.

	UPROPERTY(BlueprintReadWrite, Category="Grid|Abilities")
	bool bAllyTargetSpellEnabled; // 아군 대상 주문이 활성화되어 있는지 여부를 나타냅니다.

	UPROPERTY(BlueprintReadWrite, Category="Grid|Abilities")
	bool bAreaSpellEnabled; // 지역 주문이 활성화되어 있는지 여부를 나타냅니다.

	UPROPERTY(BlueprintReadWrite, Category="Grid|Abilities")
	bool bAreaTargetAbilityEnabled; // 지역 대상 능력이 활성화되어 있는지 여부를 나타냅니다.

	/*---	           Grid State         ---*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Grid|State")
	ASlotBase* CurrentSlotUnderMouse; // 현재 마우스 아래에 있는 슬롯을 나타냅니다.

	UPROPERTY(BlueprintReadWrite, Category="Grid|State")
	AAreaTargetSpellBase* CurrentAreaTargetSpell; // 현재 지역 대상 주문을 나타냅니다.

	UPROPERTY(BlueprintReadWrite, Category="Grid|State")
	AAreaTargetActiveAbilityBase* CurrentAreaTargetActiveAbility; // 현재 지역 대상 활성 능력을 나타냅니다.

	UPROPERTY(BlueprintReadOnly, Category="Grid|State")
	TArray<ASlotBase*> MovementOverlaySlotsInRange; // 범위 내에 있는 움직임 오버레이 슬롯의 배열입니다.

	UPROPERTY(BlueprintReadOnly, Category="Grid|State")
	TArray<ASlotBase*> MovableSlots; // 이동 가능한 슬롯의 배열입니다.

	UPROPERTY(BlueprintReadOnly, Category="Grid|State")
	TArray<ASlotBase*> SlotsInRange; // 범위 내의 슬롯의 배열입니다.

	UPROPERTY(BlueprintReadOnly, Category="Grid|State")
	TArray<ASlotBase*> EnemySlots; // 적 슬롯의 배열입니다.

	UPROPERTY(BlueprintReadOnly, Category="Grid|State")
	TArray<ASlotBase*> AllySlots; // 아군 슬롯의 배열입니다.

	UPROPERTY(BlueprintReadOnly, Category="Grid|State")
	TArray<ACharacterBase*> HighlightedCharacters; // 강조 표시된 캐릭터의 배열입니다.

	/*---	           Grid Slots and Characters         ---*/
public:
	UPROPERTY(BlueprintReadOnly, Category="Grid|SlotsAndCharacters")
	TMap<FVector2D, ASlotBase*> Slots; // 슬롯의 위치에 대한 슬롯의 맵입니다.

	UPROPERTY(BlueprintReadOnly, Category="Grid|SlotsAndCharacters")
	TMap<FVector2D, ASlotBase*> OverlaySlots; // 오버레이 슬롯의 위치에 대한 슬롯의 맵입니다.

	UPROPERTY(BlueprintReadOnly, Category="Grid|SlotsAndCharacters")
	TMap<FVector2D, ASlotBase*> InnerSlots; // 내부 슬롯의 위치에 대한 슬롯의 맵입니다.

	UPROPERTY(BlueprintReadOnly, Category="Grid|SlotsAndCharacters")
	TMap<FVector2D, ASlotBase*> InnerOverlaySlots; // 내부 오버레이 슬롯의 위치에 대한 슬롯의 맵입니다.

	UPROPERTY(BlueprintReadOnly, Category="Grid|SlotsAndCharacters")
	TMap<ACharacterBase*, FListOfSlots> CharacterMovementOverlaySlots; // 캐릭터의 이동 오버레이 슬롯에 대한 맵입니다.

	/*---	           Grid Component         ---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Component")
	USceneComponent* DefaultSceneRoot; // 이 그리드의 기본 SceneRoot를 나타냅니다.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Component")
	USplineComponent* PathSpline; // 이 그리드의 경로를 나타내는 Spline 컴포넌트입니다.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid|Component")
	UInstancedStaticMeshComponent* PathMesh; // 이 그리드의 경로를 나타내는 Mesh 컴포넌트입니다.

	/*---	           Grid Reference         ---*/
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Grid|Reference", meta=(AllowPrivateAccess="true"))
	ABattleController* BattleControllerRef; // BattleController에 대한 참조입니다.

	UPROPERTY(BlueprintReadWrite, Category="Grid|Reference", meta=(AllowPrivateAccess="true"))
	AHeroSpawnLocation* HeroSpawnLocation; // HeroSpawnLocation에 대한 참조입니다.

	UPROPERTY(BlueprintReadWrite, Category="Grid|Reference", meta=(AllowPrivateAccess="true"))
	TArray<AEnemySpawnLocation*> EnemySpawnLocations; // EnemySpawnLocation에 대한 참조 배열입니다.

	/*---	           Grid Spawn         ---*/
private:
	UPROPERTY(EditAnywhere, Category="Grid|Spawn", meta=(AllowPrivateAccess="true"))
	TSubclassOf<ASlot> SlotClass; // 슬롯 클래스입니다.

	UPROPERTY(EditAnywhere, Category="Grid|Spawn", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AGridOverlaySlot> OverlaySlotClass; // 오버레이 슬롯 클래스입니다.

	UPROPERTY(EditAnywhere, Category="Grid|Spawn", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AInnerSlot> InnerSlotClass; // 내부 슬롯 클래스입니다.

	UPROPERTY(EditAnywhere, Category="Grid|Spawn", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AGridInnerOverlaySlot> InnerOverlaySlotClass; // 내부 오버레이 슬롯 클래스입니다.

	UPROPERTY(EditAnywhere, Category="Grid|Spawn", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AHeroSpawnLocation> HeroSpawnLocationClass; // 영웅 스폰 위치 클래스입니다.

	/*---	           Grid Widget         ---*/
private:
	UPROPERTY(EditAnywhere, Category="Grid|Widget")
	TSubclassOf<UUW_DamageDetailsListUI> WBP_DamageDetailsListUIClass; // 데미지 상세 목록 UI 클래스입니다.

	UPROPERTY(BlueprintReadWrite, Category="Grid|Widget", meta=(AllowPrivateAccess="true"))
	UUW_DamageDetailsListUI* UW_DamageDetailsListUI; // 데미지 상세 목록 UI 인스턴스입니다.

	/*---	           Grid Timeline         ---*/
protected:
	UPROPERTY()
	FTimeline DetectClosestMovableSlotTimeline; // 가장 가까운 이동 가능한 슬롯을 감지하는 타임라인입니다.

	UPROPERTY()
	ASlotBase* m_CurrentMouseOverSlot; // 현재 마우스 오버 상태인 로컬 슬롯입니다.

	/*---	           Grid Delegate         ---*/
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Grid|Delegate")
	FOnCursorChanged OnCursorChanged; // 커서 변경 이벤트를 위한 델리게이트입니다.

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Grid|Delegate")
	FOnCursorAngleChanged OnCursorAngleChanged; // 커서 각도 변경 이벤트를 위한 델리게이트입니다.

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Grid|Delegate")
	FMouseOverSlot MouseOverSlot; // 마우스 오버 슬롯 이벤트를 위한 델리게이트입니다.

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Grid|Delegate")
	FOnMouseOutOfGrid OnMouseOutOfGrid; // 마우스가 그리드 밖으로 나갈 때의 이벤트를 위한 델리게이트입니다.

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Grid|Delegate")
	FSlotClicked SlotClicked; // 슬롯 클릭 이벤트를 위한 델리게이트입니다.
};
