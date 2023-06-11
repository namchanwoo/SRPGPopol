#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CharacterHealth.generated.h"

class ACharacterBase;

/**
 * 캐릭터의 체력바를 표시하는 위젯입니다.
 * 블루프린트에서 기능을 정의 합니다.
 */
UCLASS()
class SRG_API UUW_CharacterHealth : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="UW Character Health")
	void InitializeEvent(ACharacterBase* NewCharacterBase);

	UFUNCTION(BlueprintNativeEvent, Category="UW Character Health")
	void OnHealthSetHandler(int32 InHp, bool IsHeal);

	UFUNCTION(BlueprintNativeEvent, Category="UW Character Health")
	void SetStackAmount();

	/*******************************************
	 * Field Members
	 *******************************************/
public:
	UPROPERTY(BlueprintReadWrite, Category="UW Character Health")
	ACharacterBase* Character;
};
