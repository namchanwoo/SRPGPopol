
#pragma once

#include "CoreMinimal.h"
#include "SRG/Abilities/AbilityBase.h"
#include "AuraBase.generated.h"

class ADeBuffBase;
class ABuffBase;

/*
 * AAuraBase는 SRG 게임에서 오라 능력을 생성하는 데 사용되는 `AAbilityBase`의 하위 클래스입니다.
 * 오라는 범위 내의 캐릭터에게 버프와 디버프를 적용합니다. 
 * 효과를 정의하기 위해 `ABuffBase` 및 `ADeBuffBase` 클래스를 사용합니다.
 * 언리얼 엔진의 블루프린트 베이스 클래스로 설계되었습니다.
 */
UCLASS()
class SRG_API AAuraBase : public AAbilityBase
{
	GENERATED_BODY()

public:
	AAuraBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 아우라 적용 함수
	 */
	void ApplyAura();

	/**
	 * @brief 캐릭터에게 버프 추가 함수
	 * @param AffectedCharacter 버프를 받을 캐릭터
	 * @param InBuff 추가할 버프
	 */
	void AddBuff(ACharacterBase* AffectedCharacter, TSubclassOf<ABuffBase> InBuff);

	/**
	 * @brief 캐릭터에게 모든 버프를 추가하는 함수
	 * @param AffectedCharacter 버프를 받을 캐릭터
	 */
	void AddBuffsToCharacter(ACharacterBase* AffectedCharacter);

	/**
	 * @brief 캐릭터에게 디버프 추가 함수
	 * @param AffectedCharacter 디버프를 받을 캐릭터
	 * @param InDeBuff 추가할 디버프
	 */
	void AddDeBuff(ACharacterBase* AffectedCharacter, TSubclassOf<ADeBuffBase> InDeBuff);

	/**
	 * @brief 캐릭터에게 모든 디버프를 추가하는 함수
	 * @param AffectedCharacter 디버프를 받을 캐릭터
	 */
	void AddDeBuffsToCharacter(ACharacterBase* AffectedCharacter);


protected:
	ABuffBase* SpawnBuff(TSubclassOf<ABuffBase> InBuff);
	
	ADeBuffBase* SpawnDeBuff(TSubclassOf<ADeBuffBase> InDeBuff);
	
	/*******************************************
	 * Field Members
	 *******************************************/
public:
	// "Buffs"는 이 오라에서 적용할 버프의 종류를 포함하는 배열입니다.
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Aura")
	TArray<TSubclassOf<ABuffBase>> Buffs;

	// "DeBuffs"는 이 오라에서 적용할 디버프의 종류를 포함하는 배열입니다.
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Aura")
	TArray<TSubclassOf<ADeBuffBase>> DeBuffs;

};
