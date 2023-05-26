#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_RewardItem.generated.h"

// class UVerticalBox;
class UTextBlock;
class UImage;
// class UHorizontalBox;
// class UOverlay;
// class USizeBox;
// class UButton;
// class UCanvasPanel;

/**
 * @brief 이 클래스는 보상 아이템의 사용자 인터페이스를 나타냅니다.
 * UUW_RewardItem은 UUserWidget를 상속받아 사용자 정의 위젯을 생성합니다. 주로 보상 아이템의 이름, 아이콘, 그리고 수량을
 * 표시하는데 사용됩니다. 또한, 이 클래스에는 아이템의 이름, 아이콘, 수량을 설정할 수 있는 멤버 변수들이 포함되어 있습니다.
 */
UCLASS()
class SRG_API UUW_RewardItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_RewardItem(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateAmount(FText NewAmount);


	/*---	      	    Designer    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* ItemAmountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* ItemNameText;


	/*---	      	    Reward Item    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reward Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reward Item")
	FText ItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reward Item")
	UTexture2D* ItemIcon;
};
