#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "UW_DamageDetailsUI.generated.h"

class APassiveAbilityBase;
class UImage;
class UTextBlock;
class ACharacterBase;
class ABattleController;
class UW_DamageDetailsListUI;

/**
 * UUW_DamageDetailsUI는 게임에서 손상 세부 정보를 표시하기 위한 UI를 나타내는 UserWidget 클래스입니다.
 * 피해 종류, 대상, 피해 범위, 피해 수정치, 사살, 보복 상태 등의 정보를 제공한다.
 * UI에는 다양한 정보를 표시하기 위한 여러 텍스트 블록과 손상과 관련된 요소의 이미지가 포함되어 있습니다.
 * 이 클래스는 또한 공격 캐릭터, 대상 캐릭터, 전투 컨트롤러 및 주요 대상과 같은 다양한 게임 개체에 대한 참조를 보유합니다.
 * 이 클래스는 UI의 다른 부분을 설정하고 업데이트하기 위한 여러 메서드를 제공합니다.
 */
UCLASS()
class SRG_API UUW_DamageDetailsUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_DamageDetailsUI(const FObjectInitializer& ObjectInitializer);


	virtual void NativeConstruct() override;

	void SetElementData();
	
	EElement GetDamageType(FText& OutDamageType);


	void SetupDamageType();
	void SetupTargetText();
	void SetupDamageRangeText();
	void SetupDamageModifierText();
	void SetupKillText();
	void SetupRetaliateText();
	
	/*******************************************
	 * Field Members
	 *******************************************/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage Details|Designer", meta = (BindWidget))
	UTextBlock* DamageTypeText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage Details|Designer", meta = (BindWidget))
	UTextBlock* ElementText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage Details|Designer", meta = (BindWidget))
	UTextBlock* TargetText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage Details|Designer", meta = (BindWidget))
	UTextBlock* DamageText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage Details|Designer", meta = (BindWidget))
	UTextBlock* DamageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage Details|Designer", meta = (BindWidget))
	UTextBlock* KillsText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage Details|Designer", meta = (BindWidget))
	UTextBlock* RetaliateText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage Details|Designer", meta = (BindWidget))
	UImage* ElementImage;

public:
	UPROPERTY(BlueprintReadWrite, Category="Damage Details")
	ACharacterBase* AttackingCharacter;

	UPROPERTY(BlueprintReadWrite, Category="Damage Details")
	ACharacterBase* TargetCharacter;

	UPROPERTY(BlueprintReadWrite, Category="Damage Details")
	bool bIsMeleeAttack;

	UPROPERTY(BlueprintReadWrite, Category="Damage Details")
	ABattleController* BattleController;

	UPROPERTY(BlueprintReadWrite, Category="Damage Details")
	FDamageData DamageData;

	UPROPERTY(BlueprintReadWrite, Category="Damage Details")
	ACharacterBase* MainTarget;
	
	UPROPERTY(BlueprintReadWrite, Category="Damage Details")
	TSubclassOf<APassiveAbilityBase > BP_NoEnemyRetaliationAbility;
};
