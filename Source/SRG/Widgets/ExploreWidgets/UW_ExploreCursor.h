#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRGCore/SRGEnumStruct.h"
#include "UW_ExploreCursor.generated.h"


class UImage;

/**
 * 
 */
UCLASS()
class SRG_API UUW_ExploreCursor : public UUserWidget
{
	GENERATED_BODY()

public:
	/*커서의 타입 유형을 변경합니다*/
	void ChangeCursor(EExploreCursorType ExploreCursorType);


	/*---	Field Members(Variable)	---*/
#pragma region               Designer

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Designer", meta = (BindWidget))
	UImage* CursorImage;

#pragma endregion            Designer

#pragma region               ExploreCursor Members
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExploreCursor")
	UTexture2D* DefaultCursorImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExploreCursor")
	UTexture2D* InventoryCursorImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExploreCursor")
	UTexture2D* EnemyCursorImage;
#pragma endregion            ExploreCursor Members
};
