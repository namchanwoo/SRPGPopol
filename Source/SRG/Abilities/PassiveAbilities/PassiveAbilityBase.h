#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/AbilityBase.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "PassiveAbilityBase.generated.h"


class ADeBuffBase;
class ABuffBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPassiveAbilityUseEnded);


/*
 * APassiveAbilityBase는 게임 캐릭터나 객체에 대해 지속적으로 작용하는 '패시브 능력'을 구현한 클래스입니다.
 * 이 클래스는 능력의 적용, 애니메이션 재생, 능력의 종료 등에 관한 메서드를 포함하고 있습니다.
 * 또한 능력이 적용되는 대상이나 그 결과에 관한 정보를 멤버 변수로 가지고 있습니다.
 * 이 클래스는 사용률, 애니메이션 지연, 사용 시점, 능력 타겟 등 다양한 능력의 속성을 설정하고 관리하는 역할을 합니다.
 * 특히, 각 능력에 따라 적용되는 버프나 디버프의 정보와 그 지속 시간을 관리하기 위한 메서드를 제공합니다.
 * 이 클래스를 사용하면 각 캐릭터나 객체에 특별한 패시브 능력을 쉽게 적용하고 관리할 수 있습니다.
 */
UCLASS()
class SRG_API APassiveAbilityBase : public AAbilityBase
{
	GENERATED_BODY()

public:
	APassiveAbilityBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category="Passive Ability|Animation")
	void PlayAbilityAnimation();


	UFUNCTION(BlueprintCallable, Category="Passive Ability|Animation")
	void OnAnimationPlayed();

	UFUNCTION(BlueprintCallable, Category="Passive Ability|Animation")
	void OnAbilityAnimationEnded();


	UFUNCTION(BlueprintCallable, Category="Passive Ability")
	void UsePassiveAbility(const TArray<ACharacterBase*> InHitCharacters);

	virtual void OnAbilityUsed_Implementation(const TArray<ASlotBase*>& InSlotsInRange,
	                                          const TArray<ASlotBase*>& InEmptySlotsInRange,
	                                          const TArray<ACharacterBase*>& InAlliesInRange,
	                                          const TArray<ACharacterBase*>& InEnemiesInRange) override;


	UFUNCTION(BlueprintCallable, Category="Passive Ability")
	void ClearPassiveAbilityUseCallback();


	void AddBuff(ACharacterBase* InAffectedCharacter, TSubclassOf<ABuffBase> InBuff, int InTurnDuration);

	void AddDeBuff(ACharacterBase* InAffectedCharacter, TSubclassOf<ADeBuffBase> InDeBuff, int InTurnDuration);

protected:
	UFUNCTION(BlueprintCallable, Category="Passive Ability")
	void OnAbilityUsedHandler();


	/*******************************************
	* Setting
	*******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passive Ability|Attribute")
	float UseChance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passive Ability|Attribute")
	float AbilityAnimationDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passive Ability|Attribute")
	EPassiveAbilityUseMoment UseMoment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passive Ability|Attribute")
	EPassiveAbilityTarget PassiveAbilityTarget;


	/*******************************************
	* Ability Data
	*******************************************/
public:
	UPROPERTY(BlueprintReadOnly, Category="Passive Ability|Ability Data")
	TArray<ACharacterBase*> HitCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Passive Ability|Ability Data")
	TMap<TSubclassOf<ACharacterBase>, UAnimMontage*> UseAbilityMontage;


	/*******************************************
	* Status Effects
	*******************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Passive Ability|Status Effects")
	TMap<TSubclassOf<ABuffBase>, int32> Buffs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Passive Ability|Status Effects")
	TMap<TSubclassOf<ADeBuffBase>, int32> DeBuffs;


	/*******************************************
	* Delegates
	*******************************************/
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Passive Ability|Delegate")
	FOnPassiveAbilityUseEnded OnPassiveAbilityUseEnded;
};
