#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SRG/Widgets/ExploreWidgets/UW_ExploreCursor.h"
#include "ExplorePlayerController.generated.h"

class UUW_ExploreCursor;
class UInputMappingContext;

/**
 * AExplorePlayerController는 게임의 탐색 모드에서 플레이어 입력 및 작업을 관리하는 클래스입니다.
 * Main Features:
 * 1. 탐색 모드에서 플레이어의 커서를 나타내는 것으로 보이는 'UUW_ExploreCursor' 개체에 대한 참조를 포함합니다.
 * 2. 마우스 커서의 가시성과 클릭 가능성을 제어합니다.
 * 3. 이 게임 모드에서 플레이어 입력이 해석되는 방식을 정의하는 특정 입력 매핑 컨텍스트를 설정합니다.
 * 4. 'BeginPlay' 기능에서는 탐색 커서에 대한 위젯을 생성하여 기본 마우스 커서로 설정하고 게임의 입력 모드를 게임 및 UI 입력의 하이브리드로 설정합니다.
 *
 * Note: 이 클래스는 탐색 모드에서 플레이어 입력 및 작업을 처리하는 데 사용됩니다. 다른 게임 모드의 경우 유사한 플레이어 컨트롤러 클래스가 필요합니다.
 */
UCLASS()
class SRG_API AExplorePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AExplorePlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;



public:
	UUW_ExploreCursor* GetExploreCursor(){return WBP_ExploreCursor;}
	
	/*******************************************
	 * Field Members
	 *******************************************/

	/*---	      	    Input    	      	---*/
private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explore Player Controller|Input",
		meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/*---	      	    Reference    	      	---*/
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Explore Player Controller|Reference",
		meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UUW_ExploreCursor> WBP_ExploreCursorClass;

	UPROPERTY(BlueprintReadWrite, Category="Explore Player Controller|Reference", meta=(AllowPrivateAccess = "true"))
	UUW_ExploreCursor* WBP_ExploreCursor;
};
