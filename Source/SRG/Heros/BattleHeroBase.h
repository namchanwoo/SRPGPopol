#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "BattleHeroBase.generated.h"

class UBoxComponent;
class AEquipmentBase;
class ABattleController;
class AExploreHeroBase;
class ASpellBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaSet, int32, InMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpellCasted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClicked);

/*
 * ABattleHeroBase는 AActor 클래스의 하위 클래스로, 게임에서 Hero 캐릭터의 기본 클래스 역할을 하도록 특별히 설계되었습니다.
 * 여기에는 영웅 통계 관리, 전투 상호 작용 처리, 애니메이션 및 영웅의 마나, 골드, 레벨 및 장비와 같은 관련 상태 유지를 포함하되 이에 국한되지 않는 다양한 기능이 포함됩니다.
 * 이 클래스는 또한 클릭과 같은 사용자 상호 작용을 위한 유틸리티 메서드 및 대리자뿐만 아니라 새로운 턴과 같은 게임 이벤트에 대한 응답도 제공합니다.
 * 그래픽 표현을 위한 SkeletalMesh 및 클릭 감지를 위한 BoxComponent와 같은 다양한 구성 요소를 사용합니다.
 */
UCLASS()
class SRG_API ABattleHeroBase : public AActor
{
	GENERATED_BODY()

public:
	ABattleHeroBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


	/*
	 * =======================	 * Initialization Functions	 * =======================
	 */
public:
	void InitializeEvent();

	/*
	 * =======================	 * Stats and State Management Functions	 * =======================
	 */
	void SetMana(int32 NewMana);
	void SetSpells();
	void OnSpellUsed(int32 InManaCost);
	void AddDrops(int32 NewGold, const TArray<TSubclassOf<AEquipmentBase>>& NewDrops);

	/*
	 * =======================	 * Animation Functions	 * =======================
	 */
	void PlayAnimationMontage(UAnimMontage* MontageToPlay);
	void OnPlayMontageNotifyBeginHandler(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	void OnMontageEndedHandler(UAnimMontage* Montage, bool bInterrupted);

	/*
	 * =======================	 * Callback and Delegate Functions	 * =======================
	 */
	void ClearSpellCallback();

	/*
	 * =======================	 * Utility Functions	 * =======================
	 */
	FVector GetProjectileSpawnLocation();


	/*
	 * =======================	 * Event Handler Functions	 * =======================
	 */
protected:
	UFUNCTION()
	void OnNewTurnHandler(int32 NewTurn);

	UFUNCTION()
	void OnClickedHandler_RightClickDetector(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);


	/*
 	* =======================================
 	* Hero State & Statistics Properties
 	* =======================================
 	*/
public:
	// 영웅의 현재 마나
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Stats")
	int32 CurrentMana;

	// 영웅의 최대 마나
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Stats")
	int32 MaxMana;

	// 건강, 손상 등과 같은 영웅의 일반 통계
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Stats")
	FHeroStats HeroStats;

	// 영웅의 현재 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Stats")
	int32 Level;

	// 마법을 사용하는 영웅의 능력
	UPROPERTY(BlueprintReadWrite, Category="Battle Hero|Stats")
	bool bCanUseSpell;

	/*
	 * =======================================
	 * Hero Identification Properties
	 * =======================================
	 */
public:
	// 영웅의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Identity")
	FText Name;

	// 영웅의 골드 카운트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Identity")
	int32 Gold;

	/*
	 * =======================================
	 * Hero Ability & Equipment Properties
	 * =======================================
	 */
public:
	// 영웅이 현재 가지고 있는 주문
	UPROPERTY(BlueprintReadOnly, Category="Battle Hero|Ability")
	TArray<ASpellBase*> CurrentSpells;

	// 가능한 모든 주문 및 개수
	UPROPERTY(BlueprintReadWrite, Category="Battle Hero|Ability")
	TMap<TSubclassOf<ASpellBase>, int32> Spells;

	// 영웅이 현재 가지고 있는 장비
	UPROPERTY(BlueprintReadOnly, Category="Battle Hero|Equipment")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;

	// 영웅의 배낭에 있는 장비
	UPROPERTY(BlueprintReadOnly, Category="Battle Hero|Equipment")
	TArray<TSubclassOf<AEquipmentBase>> BackPack;

	/*
	 * =======================================
	 * Hero Visual Properties
	 * =======================================
	 */
public:
	// 영웅을 나타내는 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Visual")
	UTexture2D* HeroImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Visual")
	UTexture2D* HeroDetailsImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Visual")
	UAnimSequence* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Visual")
	UAnimMontage* SpellAnimation;

	// 발사체를 생성할 소켓 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Visual")
	FName ProjectileSpawnSocket;
	
	/*
	 * =======================================
	 * Hero Components
	 * =======================================
	 */
public:
	// Main skeletal mesh of the hero
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Hero|Component")
	USkeletalMeshComponent* SkeletalMesh;

	// Main scene component (root)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Hero|Component")
	USceneComponent* DefaultSceneRoot;

	// 영웅에 대한 오른쪽 클릭을 감지하는 상자 구성 요소
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Hero|Interaction")
	UBoxComponent* RightClickDetector;
	
	/*
	 * =======================================
	 * Hero Event Delegates
	 * =======================================
	 */
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnManaSet OnManaSet;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnAnimationEnded OnAnimationEnded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnSpellCasted OnSpellCasted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnRightClicked OnRightClicked;

	/*
	 * =======================================
	 * Hero Controllers
	 * =======================================
	 */
public:
	// 영웅의 전투 행위를 관장하는 컨트롤러
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero|Controller")
	ABattleController* BattleController;
};


/*

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	ABattleController* BattleController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	int32 CurrentMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	int32 MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	FHeroStats HeroStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	FName ProjectileSpawnSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	UTexture2D* HeroImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	UTexture2D* HeroDetailsImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	int32 Level;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	bool bCanUseSpell;

	UPROPERTY(BlueprintReadOnly, Category="Battle Hero")
	TArray<ASpellBase*> CurrentSpells;

	UPROPERTY(BlueprintReadWrite, Category="Battle Hero")
	TMap<TSubclassOf<ASpellBase>, int32> Spells;

	UPROPERTY(BlueprintReadOnly, Category="Battle Hero")
	TArray<TSubclassOf<AEquipmentBase>> BackPack;

	UPROPERTY(BlueprintReadOnly, Category="Battle Hero")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	UAnimSequence* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Hero")
	UAnimMontage* SpellAnimation;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Hero|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Hero|Component")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Hero|Component")
	UBoxComponent* RightClickDetector;



public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnManaSet OnManaSet;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnAnimationEnded OnAnimationEnded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnSpellCasted OnSpellCasted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle Hero|Delegate")
	FOnRightClicked OnRightClicked;
  
 */
