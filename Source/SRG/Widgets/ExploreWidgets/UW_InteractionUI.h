#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_InteractionUI.generated.h"

class UVerticalBox;
class UUW_InteractionButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractClicked, int32, ButtonIndex);

/**
 * 이 클래스는 상호작용 버튼의 생성 및 관리, 그리고 이에 대한 이벤트 처리를 담당합니다.
 * 버튼 클릭 시 발생하는 이벤트를 외부에서 구독할 수 있도록 동적 멀티캐스트 델리게이트를 제공하며, 
 * 상호작용 UI의 보이기, 숨기기 기능 등을 제공합니다.
 */
UCLASS()
class SRG_API UUW_InteractionUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_InteractionUI(const FObjectInitializer& ObjectInitializer);

	/**
 	* @brief 상호작용 텍스트를 설정합니다.
 	* @param InTexts 상호작용 버튼의 텍스트
 	*/
	void SetInteractionTexts(const TArray<FText>& InTexts);

	/**
 	* @brief 상호작용 UI를 표시합니다.
 	*/
	void ShowInteractionUI();

	/**
 	* @brief 상호작용 UI를 숨깁니다.
 	*/
	void HideInteractionUI();

	/**
 	* @brief 상호작용 UI를 즉시 숨깁니다.
 	*/
	void InstantHide();

public:
	/**
 	* @brief 버튼 클릭 핸들러 함수입니다.
 	* @param ButtonIndex 클릭된 버튼의 인덱스
 	*/
	UFUNCTION()
	void OnButtonClickedHandler(int32 ButtonIndex);


	/*---	      	    Designer    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UVerticalBox* InteractionButtonList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", Meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowAnimation;

	
	/*---	      	    Interaction UI    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionUI|Setting")
	TSubclassOf<UUW_InteractionButton> WBP_InteractionButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionUI")
	TArray<FText> InteractionTexts;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="InteractionUI|Delegate")
	FOnInteractClicked OnInteractClicked;

};
