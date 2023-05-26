#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_InteractionButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionButtonClicked, int32, ButtonIndex);

/**
 * UUW_InteractionButton 클래스는 사용자 인터페이스에 사용되는 상호작용 버튼 위젯입니다.
 * 이 클래스는 버튼 클릭에 대한 처리와 텍스트 레이블을 관리합니다.
 * 또한, 버튼 클릭 시 실행되는 델리게이트를 제공합니다.
 */
UCLASS()
class SRG_API UUW_InteractionButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;


	UFUNCTION()
	void OnButtonClicked_InteractionButton();


	/*---	      	    Designer    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UButton* InteractionButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* InteractionText;


	/*---	      	    InteractionButton    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionButton")
	FText ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionButton")
	int32 ButtonIndex;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="InteractionButton|Delegate")
	FOnInteractionButtonClicked OnInteractionButtonClicked;
};
