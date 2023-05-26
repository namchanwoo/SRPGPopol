#include "SRG/Widgets/ExploreWidgets/UW_EnemyInfoDialogue.h"

#include "UW_MenuButton.h"
#include "UW_RewardItem.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

#include "SRG/Units/Characters/CharacterBase.h"
#include "SRGCore/AssetTableRef.h"

UUW_EnemyInfoDialogue::UUW_EnemyInfoDialogue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (TSubclassOf<UUW_RewardItem> WBP_RewardItem = DT::FindClass<UUW_RewardItem>(
		DT_BLUEPRINT_PATH, FName(TEXT("WBP_RewardItem"))))
	{
		WBP_RewardItemClass = WBP_RewardItem;
	}
}

void UUW_EnemyInfoDialogue::NativeConstruct()
{
    Super::NativeConstruct();

    // 이벤트 핸들러 등록
    RegisterEventHandlers();

    // 적 목록 구성
    ConstructEnemyList();
}

void UUW_EnemyInfoDialogue::RegisterEventHandlers()
{
    ContinueButton->OnButtonClicked.AddDynamic(this, &UUW_EnemyInfoDialogue::OnButtonClicked_ContinueButton);
	
    Background->OnMouseButtonDownEvent.BindUFunction(this, FName("OnMouseButtonDown_BackGround"));
}

void UUW_EnemyInfoDialogue::ConstructEnemyList()
{
    TMap<TSubclassOf<ACharacterBase>, FVector2D> EnemyInfos = GetEnemyInfo();

    for (TTuple<TSubclassOf<ACharacterBase>, UE::Math::TVector2<double>> EnemyInfo : EnemyInfos)
    {
        AddEnemyToWidget(EnemyInfo);
    }
}

void UUW_EnemyInfoDialogue::AddEnemyToWidget(TTuple<TSubclassOf<ACharacterBase>, FVector2D> EnemyInfo)
{
    ACharacterBase* CharacterBaseCDO = EnemyInfo.Key->GetDefaultObject<ACharacterBase>();

    UUW_RewardItem* NewRewardItem = CreateWidget<UUW_RewardItem>(GetWorld(), WBP_RewardItemClass);

    NewRewardItem->ItemName = CharacterBaseCDO->Name;
    NewRewardItem->ItemIcon = CharacterBaseCDO->Image;

    FString ItemAmountString = FormatItemAmountString(EnemyInfo.Value);

    const FText NewItemAmount = FText::FromString(ItemAmountString);
    NewRewardItem->ItemAmount = NewItemAmount;

    EnemyList->AddChild(NewRewardItem);
}

FString UUW_EnemyInfoDialogue::FormatItemAmountString(FVector2D AmountRange)
{
    FString ItemAmountString;

    if (AmountRange.X == AmountRange.Y)
    {
    	ItemAmountString = FString::Printf(TEXT("%d"), static_cast<int32>(AmountRange.Y));
       
    }
    else
    {
    	ItemAmountString = FString::Printf(TEXT("%d - %d"), static_cast<int32>(AmountRange.X), static_cast<int32>(AmountRange.Y));
    }

    return ItemAmountString;
}

TMap<TSubclassOf<ACharacterBase>, FVector2D> UUW_EnemyInfoDialogue::GetEnemyInfo()
{
	TMap<TSubclassOf<ACharacterBase>, FVector2D> CurrentEnemyInfo;
	// 적 캐릭터 목록에 대해 반복
	for (const FEnemyCharacterData& CharacterList : EnemyCharacterList)
	{
		// 적 캐릭터가 유효한지 검사
		if (!IsValid(CharacterList.Character))
		{
			continue;
		}

		// 캐릭터의 MinStack과 MaxStack 범위를 벡터로 표현
		FVector2D CharacterListAmountRange = FVector2D(CharacterList.MinStack, CharacterList.MaxStack);

		// 적 캐릭터가 이미 현재 적 정보에 포함되어 있는지 검사
		if (CurrentEnemyInfo.Contains(CharacterList.Character))
		{
			// 존재하는 경우, 기존 범위에 새 범위를 추가
			FVector2D& ExistingAmountRange = CurrentEnemyInfo[CharacterList.Character];
			ExistingAmountRange += CharacterListAmountRange;
		}
		else
		{
			// 존재하지 않는 경우, 새로운 범위를 맵에 추가
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
	Reply.NativeReply = FReply::Handled(); // 이벤트를 사용하려면 FReply::Handled()를 사용하십시오.
	return Reply;
}
