#pragma once

#include "CoreMinimal.h"
#include "SlotBase.h"
#include "Slot.generated.h"


class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMouseOverSlot, ASlotBase*, InSlot, bool, IsInnerSlot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotClicked, ASlotBase*, InSlot, FKey, InButton);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseLeaveSlot, ASlotBase*, InSlot);

/*
 * ASlot 클래스는 속성과 메서드를 상속하는 ASlotBase 클래스의 자식입니다.
 * 추가 속성 및 동작과 함께 게임의 특정 유형의 슬롯을 나타냅니다.
 * 이 클래스에는 충돌 감지 및 상호 작용에 사용되는 BoxComponent가 포함되어 있습니다.
 * 또한 호버링 및 클릭과 같은 마우스 이벤트를 처리하기 위한 여러 델리게이트 기능이 있습니다.
 * ASlot 클래스는 주로 게임의 슬롯과 사용자 상호 작용을 처리하는 데 사용됩니다.
 */
UCLASS()
class SRG_API ASlot : public ASlotBase
{
	GENERATED_BODY()

public:
	ASlot();

protected:

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void ClearAllCallbacks() override;

	virtual void MouseOverSlot();
	
protected:
	UFUNCTION()
	void OnClickedHandler(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	
	UFUNCTION()
	void OnBeginCursorOverHandler(UPrimitiveComponent* TouchedComponent);
	
	UFUNCTION()
	void OnEndCursorOverHandler(UPrimitiveComponent* TouchedComponent);

	
	/*******************************************
	 * Slot Configuration
	 *******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slot|Configuration")
	bool bIsInnerSlot;

	/*******************************************
 	* Slot Components
 	*******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slot|Components")
	UBoxComponent* Box;

	/*******************************************
	 * Delegate
	 *******************************************/
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Slot|Delegate")
	FOnMouseOverSlot OnMouseOverSlot;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Slot|Delegate")
	FOnSlotClicked OnSlotClicked;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Slot|Delegate")
	FOnMouseLeaveSlot OnMouseLeaveSlot;
};
