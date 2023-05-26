#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitBase.generated.h"

class AGrid;
class ASlotBase;

/*
 * @brief 유닛을 나타내는 게임의 기본 개체입니다.
 * 이 클래스는 그리드에서의 위치, 크기 및 하이라이트 상태를 포함하여 유닛의 핵심 속성과 동작을 구현합니다.
 * 그리드에서 장치를 이동하는 메커니즘과 장치를 강조 표시하는 방법을 제공합니다.
 * @note SRG 게임에서 모든 유닛 유형의 기반 역할을 합니다.
 */
UCLASS()
class SRG_API AUnitBase : public AActor
{
	GENERATED_BODY()

public:
	AUnitBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/*******************************************
	 * Unit Event
	 *******************************************/
public:

	/**
	* @brief 유닛과 관련된 모든 이벤트를 초기화합니다.
	*/
	virtual void InitializeEvent();

	/**
	 * @brief 유닛의 위치를 그리드의 특정 슬롯으로 설정합니다.
	 * @param InSlot 유닛이 위치해야 하는 그리드의 슬롯입니다.
	 */
	virtual void SetUnitAnchorSlot(ASlotBase* InSlot);

	/**
	 * @brief 유닛을 UI에서 강조하여 선택되었거나 특별하게 표시합니다.
	 * @param IsOn 하이라이트를 켜거나 끌지를 결정하는 불린 값입니다.
	 */
	virtual void Highlight(bool IsOn);

	/**
	 * @brief 유닛이 새로운 슬롯으로 이동한 후의 이벤트와 업데이트를 처리합니다.
	 */
	virtual void OnMovedToSlot();

	/**
	 * @brief 유닛이 현재 슬롯에서 제거된 후의 이벤트와 업데이트를 처리합니다.
	 */
	virtual void OnRemovedFromSlot();



	/*******************************************
	 * Field Members
	 *******************************************/
public:
	// 기본 씬 컴포넌트를 가리키는 포인터입니다. 이 컴포넌트는 이 액터의 위치와 방향을 결정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Unit|Component")
	USceneComponent* DefaultSceneRoot;

	// 유닛이 초기화되었는지 여부를 나타내는 불리언 값입니다. true일 경우 유닛이 제대로 초기화되었음을 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Unit")
	bool bInitialized = false;

	// 유닛의 크기를 나타내는 값입니다. 이 값은 유닛이 차지하는 그리드 슬롯의 수를 결정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Unit")
	int32 UnitSize = 1;

	// 이 유닛이 위치하고 있는 그리드를 가리키는 포인터입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Unit")
	AGrid* Grid;

	// 이 유닛이 현재 차지하고 있는 그리드 슬롯을 가리키는 포인터입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Unit")
	ASlotBase* Slot;
};
