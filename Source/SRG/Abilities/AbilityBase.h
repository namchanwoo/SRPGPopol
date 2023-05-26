#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityBase.generated.h"

class ABattleController;
class ACharacterBase;

/*
 * AAbilityBase는 SRG 게임의 능력에 대한 기본 클래스입니다.
 * 게임의 모든 능력에 공통적인 속성과 메서드를 캡슐화합니다.
 * 또한 능력의 수명을 관리하고 그에 따라 이벤트를 바인딩 해제합니다.
 * 능력에는 소유자(ACharacterBase 유형)가 있으며 전투 컨트롤러(ABattleController 유형)에도 연결되어 있습니다.
 * 능력은 소유자가 누구인지 알고 소유자 캐릭터의 죽음 이벤트는 이 클래스의 메서드에 바인딩되어 소유자가 사망할 때 적절한 작업을 수행할 수 있습니다.
 * 이 클래스에는 이름, 설명, 아이콘 및 범위와 같은 능력의 다양한 측면을 정의하는 속성도 있습니다. 언리얼 에디터에서 편집할 수 있습니다.
 * 게임의 특정 기능은 이 클래스의 하위 클래스로 구현하여 이러한 속성과 메서드를 상속하고 필요에 따라 추가하거나 재정의하는 것이 좋습니다.
 */
UCLASS(ClassGroup = ("SRPG"), meta = (BlueprintSpawnableComponent),
	HideCategories = ("HLOD","Rendering","Collision", "Physics", "WorldPartition","Input","Cooking","Replication",
		"DataLayers","Event"))
class SRG_API AAbilityBase : public AActor
{
	GENERATED_BODY()

public:
	AAbilityBase();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnCharacterDies(ACharacterBase* InCharacter);


	/*******************************************
	 * Field Members
	 *******************************************/

	/*---	      	    Setting    	      	---*/
public:
	// 이 능력을 소유한 캐릭터. 능력 인스턴스를 생성할 때 설정될 것으로 예상됩니다.
	UPROPERTY(BlueprintReadWrite, Category="Ability", meta=(ExposeOnSpawn="true"))
	ACharacterBase* AbilityOwner;

	// 이 능력과 관련된 전투 컨트롤러입니다. 능력 인스턴스를 생성할 때 설정될 것으로 예상됩니다.
	UPROPERTY(BlueprintReadWrite, Category="Ability", meta=(ExposeOnSpawn="true"))
	ABattleController* BattleController;

	// 이 능력의 이름입니다. Unreal 편집기에서 설정하여 플레이어에 표시하는 데 사용할 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	FText Name;

	// 이 능력의 기능에 대한 설명입니다. Unreal 편집기에서 설정하여 플레이어에 표시하는 데 사용할 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	FText Description;

	// 이 능력을 나타내는 아이콘입니다. Unreal 편집기에서 설정하여 플레이어에 표시하는 데 사용할 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	UTexture2D* Icon;

	// 이 능력의 범위, 즉 영향을 받는 타일 수입니다. 언리얼 에디터에서 설정할 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	int32 Range = 1;

	// 이 액터 구성 요소 계층 구조의 루트로 사용할 기본 장면 구성 요소입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability|Component")
	USceneComponent* DefaultSceneRoot;
};
