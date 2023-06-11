#include "UW_DamageDetailsUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "SRG/Abilities/MultiTargetPassiveAbilities/MultiTargetPassiveAbilityBase.h"
#include "SRG/Core/BattleController.h"
#include "SRG/Gird/Grid.h"
#include "SRG/Libraries/SRPGFunctionLibrary.h"
#include "SRG/Spells/SpellBase.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "SRGCore/Libraries/SRPGUtilityLibrary.h"
#include "SRGCore/Utilities/ColorNames.h"
#include "SRG/Abilities/PassiveAbilities/PassiveAbilityBase.h"


UUW_DamageDetailsUI::UUW_DamageDetailsUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_DamageDetailsUI::NativeConstruct()
{
	Super::NativeConstruct();

	SetupDamageType();
	SetElementData();
	SetupTargetText();
	SetupDamageRangeText();
	SetupDamageModifierText();
	SetupKillText();
	SetupRetaliateText();
}

void UUW_DamageDetailsUI::SetupDamageType()
{
	FText L_DamageType;
	const EElement L_DamageTypeElement = GetDamageType(L_DamageType);
	DamageTypeText->SetText(L_DamageType);

	auto NewElementText = FText::FromString(USRPGUtilityLibrary::EnumToString(L_DamageTypeElement));
	ElementText->SetText(NewElementText);

	FLinearColor OutColor;
	UTexture2D* OutImage;
	UTexture2D* OutResistImage;

	USRPGFunctionLibrary::GetElementColorAndImage(this, L_DamageTypeElement, OutColor, OutImage, OutResistImage);
	FSlateColor NewColorAndOpacity{OutColor};
	ElementText->SetColorAndOpacity(NewColorAndOpacity);
}

void UUW_DamageDetailsUI::SetupTargetText()
{
	auto NewTargetText = FText::FromString(FString::Printf(TEXT("Target : %s }"), *TargetCharacter->Name.ToString()));
	TargetText->SetText(NewTargetText);
}

void UUW_DamageDetailsUI::SetupDamageRangeText()
{
	FText NewDamageRangeText = FText::Format(FText::FromString("Damage: {0} - {1}"),
	                                         UKismetTextLibrary::Conv_IntToText(DamageData.MinDamage),
	                                         UKismetTextLibrary::Conv_IntToText(DamageData.MaxDamage));
	DamageText->SetText(NewDamageRangeText);
}

void UUW_DamageDetailsUI::SetupDamageModifierText()
{
	FText NewDamageModifierText = FText::Format(FText::FromString(" * {0}"),
	                                            UKismetTextLibrary::Conv_DoubleToText(
		                                            DamageData.DamageModifier, HalfToEven));

	DamageModifier->SetText(NewDamageModifierText);
	auto FalseModifierColor = DamageData.DamageModifier < 1.0f ? FLinearColor::Red : FLinearColor::Green;
	auto SelectModifierColor = FMath::IsNearlyEqual(DamageData.DamageModifier, 1.0f, 1.e-6)
		                           ? SRPG_COLOR_BLACK
		                           : FalseModifierColor;
	DamageModifier->SetColorAndOpacity(FSlateColor(SelectModifierColor));
}

void UUW_DamageDetailsUI::SetupKillText()
{
	int32 MinDiedStack, MaxDiedStack;
	int32 MinRemainingHealth, MaxRemainingHealth;
	TargetCharacter->CalculateDyingStack(DamageData.MinDamage, MinDiedStack, MinRemainingHealth);
	TargetCharacter->CalculateDyingStack(DamageData.MaxDamage, MaxDiedStack, MaxRemainingHealth);

	if (MinDiedStack >= TargetCharacter->Stack)
	{
		FText NewKillText = FText::Format(FText::FromString("Kills: {1}"),
		                                  UKismetTextLibrary::Conv_IntToText(TargetCharacter->Stack));
		KillsText->SetText(NewKillText);
		return;
	}

	FText NewKillText = MinDiedStack == MaxDiedStack
		                    ? FText::Format(FText::FromString("Kills: {1}"),
		                                    UKismetTextLibrary::Conv_IntToText(MaxDiedStack))
		                    : FText::Format(FText::FromString("Kills: {0} - {1}"),
		                                    UKismetTextLibrary::Conv_IntToText(MinDiedStack),
		                                    UKismetTextLibrary::Conv_IntToText(MaxDiedStack));
	KillsText->SetText(NewKillText);
}

void UUW_DamageDetailsUI::SetupRetaliateText()
{
	if (BattleController->GetGrid()->bAreaTargetAbilityEnabled || IsValid(BattleController->CurrentSpell))
	{
		RetaliateText->SetText(FText::FromString(TEXT("Retaliates: No")));
	}
	else
	{
		bool IsAttack = bIsMeleeAttack && TargetCharacter->CurrentRetaliationCount > 0 &&
			!AttackingCharacter->PassiveAbilities.Contains(BP_NoEnemyRetaliationAbility) &&
			TargetCharacter == MainTarget;

		FText SelectText = IsAttack ? FText::FromString("Yes") : FText::FromString("No");

		FText NewRetaliateText = FText::Format(FText::FromString("Retaliates: {1}"), SelectText);
		RetaliateText->SetText(NewRetaliateText);;
	}
}

void UUW_DamageDetailsUI::SetElementData()
{
	FText OutDamageTypeText;
	auto L_DamageElement = GetDamageType(OutDamageTypeText);
	FLinearColor L_ElementColor;
	UTexture2D* L_ElementImage;
	UTexture2D* L_ElementResistImage;
	USRPGFunctionLibrary::GetElementColorAndImage(this, L_DamageElement, L_ElementColor, L_ElementImage,
	                                              L_ElementResistImage);

	if (IsValid(ElementImage))
		ElementImage->SetBrushFromTexture(L_ElementImage);
	else
		ElementImage->SetVisibility(ESlateVisibility::Collapsed);
}

EElement UUW_DamageDetailsUI::GetDamageType(FText& OutDamageType)
{
	if (IsValid(BattleController->CurrentActiveAbility))
	{
		OutDamageType = BattleController->CurrentActiveAbility->Name;
		return BattleController->CurrentActiveAbility->Element;
	}

	if (BattleController->CurrentSpell)
	{
		OutDamageType = BattleController->CurrentSpell->Name;
		return BattleController->CurrentSpell->Element;
	}

	if (IsValid(AttackingCharacter->CurrentMultiTargetPassiveAbility))
	{
		if (AttackingCharacter->CurrentMultiTargetPassiveAbility->bIsMelee == bIsMeleeAttack)
		{
			OutDamageType = AttackingCharacter->CurrentMultiTargetPassiveAbility->Name;
			return EElement::Normal;
		}
	}

	OutDamageType = bIsMeleeAttack ? FText::FromString(TEXT("Range Attack")) : FText::FromString(TEXT("Melee Attack"));
	return EElement::Normal;
}
