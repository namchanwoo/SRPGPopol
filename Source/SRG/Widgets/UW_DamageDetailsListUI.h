
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "UW_DamageDetailsListUI.generated.h"

class UOverlay;
class UVerticalBox;
class AGrid;
class ABattleController;
class ACharacterBase;
class UUW_DamageDetailsUI;

/**
 * UUW_DamageDetailsListUI는 게임에서 손상 세부 정보 목록을 표시하기 위한 UI를 나타내는 UserWidget 클래스입니다.
 * 게임 내 각 대상 캐릭터에 대한 개별 손상 세부 정보를 표시하기 위한 인터페이스를 제공합니다.
 * UI에는 손상 세부 정보 목록을 표시하기 위한 세로 상자와 배치 및 레이아웃을 위한 오버레이 컨테이너가 포함되어 있습니다.
 * 이 클래스는 또한 공격 캐릭터, 주요 대상, 전투 컨트롤러 및 대상 캐릭터 목록과 같은 다양한 게임 개체에 대한 참조를 보유합니다.
 * 이 클래스는 손상 세부 정보 목록을 표시하고 업데이트하는 여러 메서드를 제공합니다. 또한 게임의 틱 이벤트를 기반으로 UI를 업데이트하는 방법도 포함되어 있습니다.
 */
UCLASS()
class SRG_API UUW_DamageDetailsListUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_DamageDetailsListUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ShowDamageDetailsUI();

	void ShowDamageDetailsUIForAbility(const TMap<ACharacterBase*, FDamageData>& InDamageData);


	/*******************************************
	 * Field Members
	 *******************************************/

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage Details List|Designer", meta = (BindWidget))
	UOverlay* Container;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage Details List|Designer", meta = (BindWidget))
	UVerticalBox* DamageDetailsList;

public:
	UPROPERTY(BlueprintReadWrite, Category="Damage Details List")
	bool bIsMeleeAttack;

	UPROPERTY(BlueprintReadWrite, Category="Damage Details List")
	ACharacterBase* MainTarget;

	UPROPERTY(BlueprintReadWrite, Category="Damage Details List")
	ABattleController* BattleController;

	UPROPERTY(BlueprintReadWrite, Category="Damage Details List")
	ACharacterBase* AttackingCharacter;

	UPROPERTY(BlueprintReadWrite, Category="Damage Details List")
	TArray<ACharacterBase*> TargetCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage Details List|Spawn")
	TSubclassOf<UUW_DamageDetailsUI> WBP_DamageDetailsUIClass;
};
