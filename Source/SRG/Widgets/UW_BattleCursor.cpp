#include "UW_BattleCursor.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "SRG/Spells/SpellBase.h"
#include "SRG/Units/Characters/CharacterBase.h"

UUW_BattleCursor::UUW_BattleCursor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_BattleCursor::ChangeCursor(EBattleCursorType InCursorType, AActiveAbilityBase* InActiveAbility,
                                    ASpellBase* InSpell, TSubclassOf<ACharacterBase> InCharacter)
{
	SetCursorImage(InActiveAbility, InSpell, InCharacter);

	if (!Cursors.Contains(InCursorType)) { return; }

	CursorImage->SetBrushFromTexture(*Cursors.Find(InCursorType));
	CursorImage->SetRenderTransformAngle(0.0f);

	FVector2D containerTranslation;
	FVector2D cursorScale = FVector2D(1.0f, 1.0f);
	switch (InCursorType)
	{
	case EBattleCursorType::Default:
		break;
	case EBattleCursorType::Walk:
		containerTranslation = CursorImage->Brush.ImageSize * -0.5f;
		break;
	case EBattleCursorType::Fly:
		containerTranslation = CursorImage->Brush.ImageSize * -0.5f;
		cursorScale = FVector2D(1.3f, 1.3f);
		break;
	case EBattleCursorType::MeleeAttack:
		break;
	case EBattleCursorType::RangeAttack:
		containerTranslation = CursorImage->Brush.ImageSize * -0.5f;
		break;
	case EBattleCursorType::BrokenRangeAttack:
		containerTranslation = CursorImage->Brush.ImageSize * -0.5f;
		cursorScale = FVector2D(1.2f, 1.2f);
		break;
	case EBattleCursorType::Spell:
		containerTranslation = CursorImage->Brush.ImageSize * -0.5f;
		break;
	case EBattleCursorType::Wait:
		containerTranslation = CursorImage->Brush.ImageSize * -0.5f;
		cursorScale = FVector2D(0.9f, 0.9f);
		break;
	default: break;
	}

	Container->SetRenderTranslation(containerTranslation);
	CursorImage->SetRenderScale(cursorScale);
	Image->SetRenderTranslation(FVector2D(0.0f,0.0f));
}

void UUW_BattleCursor::SetCursorAngle(float InAngle)
{
	CursorImage->SetRenderTransformAngle(InAngle - 225.0f);

	FVector2D falseVector2D = UKismetMathLibrary::InRange_FloatFloat(InAngle, -22.5f, 22.5f) ||
	                          (InAngle < -157.5f || InAngle > 157.5f)
		                          ? FVector2D(0.0f, -17.0f)
		                          : FVector2D(0.0f, -32.0f);

	FVector2D NewTranslation = UKismetMathLibrary::InRange_FloatFloat(InAngle, -157.5f, -22.5f)
		                           ? FVector2D(0.0f, 0.0f)
		                           : falseVector2D;

	Image->SetRenderTranslation(NewTranslation);
}

void UUW_BattleCursor::SetCursorImage(AActiveAbilityBase* InActiveAbility, ASpellBase* InSpell,
                                      TSubclassOf<ACharacterBase> InCharacter)
{
	if (IsValid(InActiveAbility))
	{
		Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Image->SetBrushFromTexture(InActiveAbility->Icon);
		return;
	}

	if (IsValid(InSpell))
	{
		Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Image->SetBrushFromTexture(InSpell->CursorIcon);
		return;
	}

	if (IsValid(InCharacter))
	{
		ACharacterBase* CharacterCDO = InCharacter->GetDefaultObject<ACharacterBase>();
		Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Image->SetBrushFromTexture(CharacterCDO->DragImage);
		return;
	}

	Image->SetVisibility(ESlateVisibility::Collapsed);
}
