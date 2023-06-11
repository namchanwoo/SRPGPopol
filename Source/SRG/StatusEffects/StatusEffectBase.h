#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "StatusEffectBase.generated.h"

class UNiagaraSystem;
class ACharacterBase;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnDurationUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDOTApplied, float, InDelay);

/**
 * AStatusEffectBase는 게임의 상태 효과에 대한 핵심 기능을 제공하는 클래스입니다.
 * 여기에는 다양한 방식으로 캐릭터 통계 또는 게임 플레이를 변경할 수 있는 오라, 버프, 디버프 및 기타 게임 효과가 포함될 수 있습니다.
 */
UCLASS(ClassGroup = ("SRPG"), meta = (BlueprintSpawnableComponent),
	HideCategories = ("HLOD","Rendering","Collision", "Physics", "WorldPartition","Input","Cooking","Replication",
		"DataLayers","Event"))
class SRG_API AStatusEffectBase : public AActor
{
	GENERATED_BODY()

public:
	AStatusEffectBase();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	/*******************************************
	 * EVENT HANDLING FUNCTIONS
	 *******************************************/
public:
	/**
	* @brief 필요한 속성 및 동작을 설정하여 상태 효과를 초기화합니다.
	*/
	virtual void InitializeEvent();

	/**
	* @brief 상태 효과가 적용될 때 동작을 정의합니다.
	*/
	virtual void OnStatusEffectStarted();

	/**
	* @brief 상태 효과 종료 시 동작을 정의합니다.
	*/
	virtual void OnStatusEffectEnded();

	/**
	* @brief 이 함수는 캐릭터의 사망 이벤트를 처리합니다. 일반적으로 죽음에 대한 상태 효과를 제거하는 데 사용됩니다.
	* @param InCharacter 사망한 캐릭터.
	*/
	UFUNCTION()
	void OnCharacterDies(ACharacterBase* InCharacter);

	/**
	* @brief 이 함수는 턴 기반 시스템에서 새로운 턴이 시작될 때 이벤트를 처리합니다.
	* @param NewTurn 새 차례의 번호입니다.
	*/
	UFUNCTION()
	void OnNewTurnHandler(int32 NewTurn);


	/*******************************************
 	* AURA MANAGEMENT FUNCTIONS
 	*******************************************/
public:
	/**
	* @brief 오라 로직을 설정합니다. 오라에서 온 상태 효과가 있는지 확인하고 필요한 델리게이트를 추가합니다.
	*/
	void SetAuraLogic();

	/**
	* @brief 오라 갱신 이벤트를 처리하는 함수입니다. 오라 상태 효과를 다시 적용하는 데 사용됩니다.
	*/
	UFUNCTION()
	void OnRefreshAurasHandler();


	/*******************************************
 	* STATUS EFFECT MANAGEMENT FUNCTIONS
 	*******************************************/
public:
	/**
	* @brief 이 상태 효과가 제거하는 다른 상태 효과를 제거합니다.
	*/
	void RemoveOtherStatusEffects();

	/**
	* @brief 이 상태 효과를 제거합니다. 캐릭터에서 제거하고 이벤트를 발생시킵니다.
	*/
	void RemoveStatusEffect();

	/**
	* @brief 지속 피해(DOT) 상태 효과가 적용될 때 호출되는 가상 함수입니다.
	*/
	virtual void OnDOT();

	/**
	* @brief 이 상태 효과를 캐릭터에서 제거합니다. 이 함수는 캐릭터의 상태 효과 목록에서 이 상태 효과를 제거합니다.
	*/
	virtual void RemoveFromCharacter();


	/*******************************************
 	* STATISTICS MODIFICATION FUNCTIONS
 	*******************************************/
public:
	/**
	* @brief 주어진 통계를 캐릭터에 적용합니다.
	* @param InStat 적용할 통계 
	*/
	void ApplyStats(const FCharacterStats& InStat);

	/**
	* @brief 주어진 요소 감소를 캐릭터에 적용합니다.
	* @param InElementReductions 적용할 요소 감소 
	*/
	void ApplyElementReductions(const FElementReductions& InElementReductions);

	/**
	* @brief 캐릭터에서 통계를 제거합니다.
	*/
	void RemoveStats();

	/**
	* @brief 캐릭터에서 요소 감소를 제거합니다.
	*/
	void RemoveElementReductions();

	/**
	* @brief 캐릭터의 통계에 곱해지는 통계를 계산하여 반환합니다.
	* @return 곱해진 통계
	*/
	FCharacterStats GetMultipliedStats();

	/**
	* @brief 모든 상태 효과를 가져와서 반환합니다.
	* @return 현재 적용된 모든 상태 효과
	*/
	TArray<AStatusEffectBase*> GetAllStatusEffects();


	/*******************************************
 	* CHARACTER INTERACTION FUNCTIONS
 	*******************************************/
public:
	/**
	* @brief 캐스터 캐릭터가 슬롯에서 제거될 때 실행되는 함수입니다.
	* @param InCharacter 슬롯에서 제거되는 캐스터 캐릭터
	*/
	UFUNCTION()
	void OnCasterCharacterRemovedFromSlot(ACharacterBase* InCharacter);


	/*******************************************
 	* VISUAL AND AUDIO EFFECT FUNCTIONS
 	*******************************************/
public:
	/**
	* @brief 상태 효과에 연관된 사운드를 재생합니다.
	*/
	void PlaySound();

	/**
	* @brief 상태 효과에 연관된 파티클 효과를 재생합니다.
	*/
	void PlayParticleEffect();


	/*******************************************
	 * Field Members
	 *******************************************/

	/*---	      	    STATUS EFFECT CONFIGURATION   	      	---*/
public:
	// 이 상태 효과가 오라에서 오는지 여부를 나타내는 부울 플래그입니다.
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Status Effect", meta=(ExposeOnSpawn="true"))
	bool bIsFromAura;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Status Effect")
	bool bIsDOT;

	// 이 상태 효과가 지속되는 턴 수입니다.
	UPROPERTY(BlueprintReadWrite, Category="Status Effect", meta=(ExposeOnSpawn="true"))
	int32 TurnDuration;

	// 공격 후 이 상태 효과를 제거해야 하는지 여부를 나타내는 부울 플래그입니다.
	UPROPERTY(BlueprintReadWrite, Category="Status Effect", meta=(ExposeOnSpawn="true"))
	bool bRemoveAfterAttack;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Status Effect")
	FText Name;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Status Effect")
	FText Description;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Status Effect")
	UTexture2D* Icon;
	
	// 이 상태 효과를 시전하는 캐릭터에 대한 포인터입니다.
	UPROPERTY(BlueprintReadWrite, Category="Status Effect", meta=(ExposeOnSpawn="true"))
	ACharacterBase* CasterCharacter;

	// 이 상태 효과가 적용될 때 제거될 상태 효과 클래스의 배열입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Status Effect")
	TArray<TSubclassOf<AStatusEffectBase>> RemovesStatusEffects;

	/*---	      	    CHARACTER STATISTICS 	      	---*/
public:
	// 이 상태 효과로 인해 영향을 받는 캐릭터에 적용되는 추가 통계입니다.
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Status Effect")
	FCharacterStats StatAdditions;

	// 이 상태 효과로 인해 영향을 받는 캐릭터에 적용될 요소 감소입니다.
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Status Effect")
	FElementReductions ElementReductions;

	// 이 상태 효과로 인해 영향을 받는 캐릭터의 캐릭터 통계에 대한 승수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effect")
	FCharacterStatsBuff StatMultiplications;

	/*---	      	    CHARACTER REFERENCE 	      	---*/
public:
	// 이 상태 효과가 적용될 캐릭터에 대한 포인터입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effect")
	ACharacterBase* AffectedCharacter;

	/*---	      	    SOUND AND VISUAL EFFECTS 	      	---*/
public:
	// 이 상태 효과가 적용될 때 재생할 사운드입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effect")
	USoundBase* Sound;

	// 이 상태 효과가 적용될 때 재생할 입자 효과입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effect")
	UParticleSystem* ParticleEffect;

	// 이 상태 효과가 적용될 때 재생되는 나이아가라 파티클 시스템입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effect")
	UNiagaraSystem* NiagaraParticleEffect;

	/*---	      	    DELEGATES 	      	---*/
public:
	// 이 상태 효과의 턴 지속 시간이 업데이트될 때 호출될 델리게이트입니다.
	UPROPERTY(BlueprintAssignable, BlueprintAssignable, Category="Status Effect|Delegate")
	FOnTurnDurationUpdated OnTurnDurationUpdated;

	// DOT가 적용될 때 호출될 델리게이트입니다.
	UPROPERTY(BlueprintAssignable, BlueprintAssignable, Category="Status Effect|Delegate")
	FOnDOTApplied OnDOTApplied;


	/*---	      	    COMPONENTS 	      	---*/
public:
	// 이 상태 효과의 루트 구성 요소입니다.
	UPROPERTY(BlueprintReadWrite, Category="Status Effect|Component")
	USceneComponent* DefaultSceneRoot;
};
