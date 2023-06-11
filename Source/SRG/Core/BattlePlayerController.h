#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "SRG/Spells/SpellBase.h"
#include "SRGCore/EnumStruct/SRGEnumStruct.h"
#include "BattlePlayerController.generated.h"

class UUW_EmptyMouseCursor;
class UUW_BattleCursor;
class AGrid;

UCLASS()
class SRG_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABattlePlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnCursorChangedHandler(EBattleCursorType InCursorType, AActiveAbilityBase* InActiveAbility,
								ASpellBase* InSpell, TSubclassOf<ACharacterBase> InCharacter);
	UFUNCTION()
	void OnCursorAngleChangedHandler(float InAngle);
	
	/*******************************************
	 * Battle Player Controller
	 *******************************************/
public:
	UPROPERTY(BlueprintReadWrite, Category="Battle Player Controller")
	AGrid* Grid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Player Controller|Spawn")
	TSubclassOf<UUW_BattleCursor> WBP_BattleCursorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Player Controller|Spawn")
	TSubclassOf<UUW_EmptyMouseCursor> WBP_EmptyMouseCursorClass;

	UPROPERTY(BlueprintReadWrite, Category="Battle Player Controller")
	UUW_BattleCursor* UW_BattleCursor;

	UPROPERTY(BlueprintReadWrite, Category="Battle Player Controller")
	UUW_EmptyMouseCursor* UW_EmptyMouseCursor;
};
