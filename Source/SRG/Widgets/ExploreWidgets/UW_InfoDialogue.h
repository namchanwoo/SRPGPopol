#pragma once

#include "CoreMinimal.h"
#include "UW_ExploreWidget.h"
#include "Blueprint/UserWidget.h"
#include "UW_InfoDialogue.generated.h"

// class UVerticalBox;
// class UHorizontalBox;
// class UOverlay;
// class USizeBox;
// class UCanvasPanel;

class UImage;
class UUW_MenuButton;
class UTextBlock;
class UButton;


/**
  * @brief 이 클래스는 게임의 대화 상자 위젯을 나타냅니다.
  * UUW_InfoDialogue는 UUW_ExploreWidget의 하위 클래스이며 대화 상자 인터페이스를 만드는 데 사용됩니다.
  * 대화 상자에는 제목, 설명, 확인 버튼 및 배경 이미지가 포함됩니다.
  * 대화 상자의 내용은 DialogueTitle, DialogueText, YesButtonText 속성을 통해 설정할 수 있습니다.
  * OK 버튼을 클릭하거나 배경을 클릭하는 것과 같은 이벤트는 각각 OnButtonClicked() 및 OnBackgroundMouseButtonDown() 메서드에 의해 처리됩니다.
  * 이 위젯은 일반적으로 퀘스트에 대한 세부 정보나 NPC의 메시지와 같은 정보를 사용자에게 표시하는 데 사용됩니다.
  * @note 이것은 SRG 게임에서 사용자 인터페이스의 중요한 구성 요소입니다.
  */
UCLASS()
class SRG_API UUW_InfoDialogue : public UUW_ExploreWidget
{
	GENERATED_BODY()

public:
	UUW_InfoDialogue(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	void SetUpInfoDialogue();
public:
	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	FEventReply OnBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);


	/*---	      	    Designer    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UImage* Background;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UUW_MenuButton* OkButton;


	/*---	      	    Properties    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info Dialogue")
	FText DialogueTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info Dialogue")
	FText DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info Dialogue")
	FText YesButtonText;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Info Dialogue|Delegate")
	FOnOkClicked OnOkClicked;
};
