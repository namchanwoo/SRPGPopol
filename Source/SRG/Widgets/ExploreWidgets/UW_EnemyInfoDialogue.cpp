#include "SRG/Widgets/ExploreWidgets/UW_EnemyInfoDialogue.h"

#include "UW_MenuButton.h"
#include "UW_RewardItem.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "SRG/Units/CharacterBase.h"
#include "SRGCore/AssetTableRef.h"

UUW_EnemyInfoDialogue::UUW_EnemyInfoDialogue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TSubclassOf<UUW_RewardItem> WBP_RewardItem = DT::FindClass<UUW_RewardItem>(DT_BLUEPRINT_PATH, FName(TEXT("WBP_RewardItem")));
	if (WBP_RewardItem)
	{
		WBP_RewardItemClass = WBP_RewardItem;
	}
}

void UUW_EnemyInfoDialogue::NativeConstruct()
{
	Super::NativeConstruct();

	ContinueButton->OnButtonClicked.AddDynamic(this, &UUW_EnemyInfoDialogue::OnButtonClicked_ContinueButton);
	Background->OnMouseButtonDownEvent.BindUFunction(this, FName("OnMouseButtonDown_BackGround"));

	TMap<TSubclassOf<ACharacterBase>, FVector2D> EnemyInfos = GetEnemyInfo();

	for (TTuple<TSubclassOf<ACharacterBase>, UE::Math::TVector2<double>> EnemyInfo : EnemyInfos)
	{
		ACharacterBase* CharacterBaseCDO = EnemyInfo.Key->GetDefaultObject<ACharacterBase>();

		UUW_RewardItem* NewRewardItem = CreateWidget<UUW_RewardItem>(GetWorld(), WBP_RewardItemClass);

		NewRewardItem->ItemName = CharacterBaseCDO->Name;
		NewRewardItem->ItemIcon = CharacterBaseCDO->Image;


		FString ItemAmountString;
		if (EnemyInfo.Value.X == EnemyInfo.Value.Y)
		{
			ItemAmountString = FString::Printf(TEXT("%d - %d"), static_cast<int32>(EnemyInfo.Value.X), static_cast<int32>(EnemyInfo.Value.Y));
		}
		else
		{
			ItemAmountString = FString::Printf(TEXT("%d"), static_cast<int32>(EnemyInfo.Value.Y));
		}

		const FText NewItemAmount = FText::FromString(ItemAmountString);
		NewRewardItem->ItemAmount = NewItemAmount;
		EnemyList->AddChild(NewRewardItem);
	}
}

TMap<TSubclassOf<ACharacterBase>, FVector2D> UUW_EnemyInfoDialogue::GetEnemyInfo()
{
	TMap<TSubclassOf<ACharacterBase>, FVector2D> CurrentEnemyInfo;
	for (const FEnemyCharacterData& CharacterList : EnemyCharacterList)
	{
		if (!IsValid(CharacterList.Character))
		{
			continue;
		}

		FVector2D CharacterListAmountRange = FVector2D(CharacterList.MinStack, CharacterList.MaxStack);
		if (CurrentEnemyInfo.Contains(CharacterList.Character))
		{
			FVector2D NewAmountRange = *CurrentEnemyInfo.Find(CharacterList.Character) + CharacterListAmountRange;
			CurrentEnemyInfo.Add(CharacterList.Character, NewAmountRange);
		}
		else
		{
			CurrentEnemyInfo.Add(CharacterList.Character, CharacterListAmountRange);
		}
	}

	return CurrentEnemyInfo;
}

void UUW_EnemyInfoDialogue::OnButtonClicked_ContinueButton()
{
	if (OnContinueClicked.IsBound())
		OnContinueClicked.Broadcast();
}

FEventReply UUW_EnemyInfoDialogue::OnMouseButtonDown_BackGround(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = FReply::Handled(); // Use FReply::Handled() if you want to consume the event.
	return Reply;
}
