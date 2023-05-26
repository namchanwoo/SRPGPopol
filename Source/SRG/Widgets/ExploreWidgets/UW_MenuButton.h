#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MenuButton.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

/**
 * @brief 이 클래스는 주로 텍스트 표시 및 클릭 처리를 포함하여 메뉴 버튼의 기능을 캡슐화합니다.
 * 버튼 텍스트, 글꼴 크기, 버튼 크기 및 클릭 이벤트 핸들러를 설정하기 위한 멤버 변수도 포함되어 있습니다.
*/
UCLASS()
class SRG_API UUW_MenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_MenuButton(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetButtonText(FText InText);

	UFUNCTION()
	void OnClicked_MenuButton();


	/*******************************************
	 * Field Members
	 *******************************************/

	/*---	      	    Designer    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UButton* MenuButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* ButtonText;


	/*---	      	    Properties    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuButton")
	float FontSize = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuButton")
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuButton")
	FVector2D ButtonSize;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="MenuButton|DelegateEvent")
	FOnButtonClicked OnButtonClicked;
};
