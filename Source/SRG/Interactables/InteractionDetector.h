#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRG/Characters/ExploreHeroBase.h"
#include "InteractionDetector.generated.h"

class UUW_InteractionUI;
class UWidgetComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, int32, InButtonIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionAppeared);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionDisappeared);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoaded, bool, bShouldLoadEnemyPawns);


/*
 *  사용자 인터페이스와의 상호작용을 관리하는 감지기. 이 클래스는 'AActor'를 상속받으며, 'UUW_InteractionUI' 위젯과의 상호작용을 관리합니다.
 * 이 클래스의 주요 기능은 상호작용 가능 여부의 설정, 상호작용 UI의 표시 및 숨김, 'ExploreHero'의 설정, 그리고 상호작용이 클릭되었을 때의 처리입니다.
 */
UCLASS()
class SRG_API AInteractionDetector : public AActor
{
	GENERATED_BODY()

public:
	AInteractionDetector();
	
	// 기본 씬 루트 설정
	void SetupDefaultSceneRoot();
	
	// 상호작용 구체 설정
	void SetupInteractionSphere();

	// 상호작용 위젯 설정
	void SetupInteractionWidget();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	/*******************************************
	 * Interaction Settings
	 *******************************************/
public:
	/*탐색 영웅을 설정합니다.*/
	void SetExploreHero(AExploreHeroBase* InExploreHero);

	/*상호작용을 활성화합니다.*/
	void EnableInteraction();

	/*상호작용을 비활성화 합니다.*/
	void DisableInteraction();

	/**
	 * @brief 상호작용을 비활성화하고 필요한 경우 적 캐릭터를 로드하는 함수입니다.
	 * @param IsShouldLoadEnemyPawns 만약 이 매개변수가 true로 설정되면, 적 캐릭터들이 로드됩니다.
	 */
	void Load(bool IsShouldLoadEnemyPawns);

	/*******************************************
	 * UI Handling
	 *******************************************/
public:
	/*상호작용 UI를 보여줍니다.*/
	void ShowInteractionUI();

	/*상호작용 UI를 숨깁니다.*/
	void HideInteractionUI();

	/*******************************************
	 * Event Handlers
	 *******************************************/
public:
	/**
 	* @brief 상호작용 UI의 버튼 클릭 이벤트 처리를 담당하는 함수입니다.
 	* @param ButtonIndex 클릭된 버튼의 인덱스를 받습니다.
 	*/
	UFUNCTION()
	void OnInteractClicked_InteractionUI(int32 ButtonIndex);


	/*******************************************
	 * Field Members
	 *******************************************/
	
	/*---	      	    Component    	      	---*/
public:
	// 기본적인 피벗 포인트를 제공하는 주 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction Detector|Component")
	USceneComponent* DefaultSceneRoot;

	// 상호작용 영역을 나타내는 구 형태의 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction Detector|Component")
	USphereComponent* InteractionSphere;

	// 상호작용 UI를 표현하는 위젯 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction Detector|Component")
	UWidgetComponent* InteractionWidget;

	/*---	      	    Interaction Detector    	      	---*/
public:
	// 상호작용이 가능한지 나타내는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Detector")
	bool bCanInteract = true;

	// 강제로 상호작용을 발생시키는지 여부를 결정하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Detector")
	bool bIsForceInteraction;

	// 상호작용 가능한 영역의 반경을 나타내는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Detector")
	float InteractionRadius = 100.0f;

	// 상호작용 UI가 나타나는 위치를 결정하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Detector")
	FVector InteractionUILocation;

	// 이 디텍터와 연관된 탐험용 영웅 객체
	UPROPERTY(BlueprintReadWrite, Category="Interaction Detector")
	AExploreHeroBase* ExploreHero;

	// 상호작용 UI를 관리하는 객체
	UPROPERTY(BlueprintReadWrite, Category="Interaction Detector")
	UUW_InteractionUI* InteractionUI;

	// 상호작용 UI에 표시되는 텍스트 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Detector")
	TArray<FText> InteractionTexts;

	/*---	      	    Delegate    	      	---*/
public:
	// 상호작용이 발생했을 때 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction Detector|Delegate")
	FOnInteracted OnInteracted;

	// 상호작용 UI가 나타났을 때 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction Detector|Delegate")
	FOnInteractionAppeared OnInteractionAppeared;

	// 상호작용 UI가 사라졌을 때 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction Detector|Delegate")
	FOnInteractionDisappeared OnInteractionDisappeared;

	// 로딩이 완료되었을 때 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Interaction Detector|Delegate")
	FOnLoaded OnLoaded;
};
