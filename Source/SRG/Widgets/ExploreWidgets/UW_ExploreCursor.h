#pragma once

#include "CoreMinimal.h"
#include "UW_ExploreWidget.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "UW_ExploreCursor.generated.h"


class UImage;

/**
 * 이 클래스는 'UUW_ExploreWidget'를 상속받아 탐색 커서의 기능을 구현합니다.
 * 'UUW_ExploreCursor'는 다양한 커서 이미지를 관리하고 필요에 따라 이미지를 변경하는 역할을 합니다.
 */
UCLASS()
class SRG_API UUW_ExploreCursor : public UUW_ExploreWidget
{
	GENERATED_BODY()

public:
	UUW_ExploreCursor(const FObjectInitializer& ObjectInitializer);

public:
	/*커서의 타입 유형을 변경합니다*/
	UFUNCTION(BlueprintCallable, Category="ExploreCursor")
	void ChangeCursor(EExploreCursorType ExploreCursorType);


	/*******************************************
	 * Field Members
	 *******************************************/

	/*---	      	    Designer    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Designer", meta = (BindWidget))
	UImage* CursorImage;


	/*---	      	    Properties    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExploreCursor")
	UTexture2D* DefaultCursorImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExploreCursor")
	UTexture2D* InventoryCursorImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExploreCursor")
	UTexture2D* EnemyCursorImage;
};
