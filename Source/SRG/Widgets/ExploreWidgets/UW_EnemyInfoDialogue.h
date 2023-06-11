#pragma once

#include "CoreMinimal.h"
#include "UW_ExploreWidget.h"
#include "Blueprint/UserWidget.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "UW_EnemyInfoDialogue.generated.h"


class UTextBlock;
class UUW_RewardItem;
class UUW_MenuButton;
class UImage;
class UHorizontalBox;


/**
 * 적 캐릭터들에 대한 정보를 보여주는 사용자 위젯입니다.
 * 이 위젯은 적 리스트, 버튼 및 인터랙티브 요소, 적 캐릭터 데이터, 텍스트 및 이미지 등을 포함하고 있습니다.
 */
UCLASS()
class SRG_API UUW_EnemyInfoDialogue : public UUW_ExploreWidget
{
	GENERATED_BODY()

public:
	UUW_EnemyInfoDialogue(const FObjectInitializer& ObjectInitializer);

	/**
 	* @brief EnemyInfoDialogue 위젯을 초기화합니다.
 	*/
	virtual void NativeConstruct() override;

	/**
 	* @brief 이 위젯에 대한 이벤트 핸들러를 등록합니다.
 	*/
	void RegisterEventHandlers();

	/**
 	* @brief 적 정보를 가져오고 해당 위젯을 생성하여 적 목록을 구성합니다.
 	*/
	void ConstructEnemyList();


	/**
 	* @brief 위젯에 단일 적을 추가합니다.
 	* @param EnemyInfo 추가할 적에 대한 정보입니다.
 	*/
	void AddEnemyToWidget(TTuple<TSubclassOf<ACharacterBase>, FVector2D> EnemyInfo);

	/**
 	* @brief 지정된 금액 범위를 기준으로 항목 금액 문자열의 서식을 지정합니다.
 	* @param AmountRange 문자열로 형식을 지정할 금액의 범위입니다.
 	* @return 금액 범위를 나타내는 형식이 지정된 문자열입니다.
 	*/
	FString FormatItemAmountString(UE::Math::TVector2<double> Value);

public:
	/**
 	* @brief 적 정보를 가져오는 함수입니다. 각 적 캐릭터들에 대한 정보를 맵 형태로 반환합니다.
 	* @return TMap<TSubclassOf<ACharacterBase>, FVector2D> 형태의 적 정보
 	*/
	TMap<TSubclassOf<ACharacterBase>, FVector2D> GetEnemyInfo();

	UFUNCTION()
	void OnButtonClicked_ContinueButton();

	UFUNCTION()
	FEventReply OnMouseButtonDown_BackGround(FGeometry MyGeometry, const FPointerEvent& MouseEvent);


	/*******************************************
	 * Field Members
	 *******************************************/

	/*---	      Designer    	      	---*/
public:
	// 적 리스트를 표시하는 가로 박스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UHorizontalBox* EnemyList;

	// 대화 상자의 배경 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UImage* Background;

	// "계속" 버튼을 나타내는 위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UUW_MenuButton* ContinueButton;

	// 대화 상자의 제목을 표시하는 텍스트 블록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer", meta = (BindWidget))
	UTextBlock* Title;

	/*---	      Properties    	      	---*/
public:
	// 보상 아이템 위젯의 클래스를 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Info Dialogue")
	TSubclassOf<UUW_RewardItem> WBP_RewardItemClass;

	// 적 캐릭터의 데이터 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Info Dialogue")
	TArray<FEnemyCharacterData> EnemyCharacterList;

	// "계속" 버튼 클릭 시 발동하는 델리게이트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Enemy Info Dialogue|Delegate")
	FOnContinueClicked OnContinueClicked;
};
