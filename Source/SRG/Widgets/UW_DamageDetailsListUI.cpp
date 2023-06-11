#include "UW_DamageDetailsListUI.h"

#include "UW_DamageDetailsUI.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/VerticalBox.h"
#include "SRG/Core/BattleController.h"
#include "SRGCore/Utilities/AssetTableRef.h"
#include "SRGCore/Utilities/SRGLog.h"

UUW_DamageDetailsListUI::UUW_DamageDetailsListUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (const TSubclassOf<UUW_DamageDetailsUI> WBP_DamageDetailsUI_Src = DT::FindClass<UUW_DamageDetailsUI>(
		DT_WIDGET_PATH, FName(TEXT("WBP_DamageDetailsUI"))))
	{
		WBP_DamageDetailsUIClass = WBP_DamageDetailsUI_Src;
	}
}

void UUW_DamageDetailsListUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 마우스의 뷰포트 상 위치를 가져옴
	const FVector2D MousePositionOnViewport = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// 뷰포트의 크기와 스케일을 가져옴
	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
	const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);

	// 뷰포트의 크기와 스케일을 고려한 비율 계산
	const double ViewportRatioX = ViewportSize.X / ViewportScale;
	const double ViewportRatioY = ViewportSize.Y / ViewportScale;

	// Container의 X 위치 계산
	const float ContainerPosX = Container->GetCachedGeometry().GetLocalSize().X +
		UWidgetLayoutLibrary::SlotAsCanvasSlot(Container)->GetPosition().X + 50.0f;

	// X 위치 계산
	const float PositionX = (MousePositionOnViewport.X + ContainerPosX) - ViewportRatioX;
	const float NewViewportX = MousePositionOnViewport.X - FMath::Max(PositionX, 0.0f);

	// Container의 Y 위치 계산
	const float ContainerPosY = (Container->GetCachedGeometry().GetLocalSize().Y * 0.5f) +
		UWidgetLayoutLibrary::SlotAsCanvasSlot(Container)->GetPosition().Y + 50.0f;

	// Y 위치 계산
	const float PositionY = (MousePositionOnViewport.Y + ContainerPosY) - ViewportRatioY;
	const float ClampPositionY = MousePositionOnViewport.Y - FMath::Max(PositionY, 0.0f);

	// Container와 마우스의 Y 위치에 따라 뷰포트 Y 위치 계산
	const float ContainerMouseDiffY = MousePositionOnViewport.Y - ContainerPosY;
	const float NewViewportY = ContainerMouseDiffY < 0.0f
		                           ? MousePositionOnViewport.Y - (MousePositionOnViewport.Y - ContainerPosY)
		                           : MousePositionOnViewport.Y - ClampPositionY;

	// 새로운 뷰포트 위치 설정
	const FVector2D NewViewportPosition(NewViewportX, NewViewportY);
	SetPositionInViewport(NewViewportPosition, false);
}

void UUW_DamageDetailsListUI::ShowDamageDetailsUI()
{
	SRPG_IF_CHECK(WBP_DamageDetailsUIClass, TEXT("WBP_DamageDetailsUIClass 클래스가 설정되지 않았습니다."));

	int32 TotalDamage;
	TMap<ACharacterBase*, FDamageData> DamageData;
	BattleController->CalculateDamage(AttackingCharacter, TargetCharacters, bIsMeleeAttack, DamageData, TotalDamage);

	for (const TTuple<ACharacterBase*, FDamageData> InDamageData : DamageData)
	{
		ACharacterBase* InDamageCharacter = InDamageData.Key;
		UUW_DamageDetailsUI* NewDamageDetailUI =
			CreateWidget<UUW_DamageDetailsUI>(GetWorld(), WBP_DamageDetailsUIClass);

		NewDamageDetailUI->AttackingCharacter = AttackingCharacter;
		NewDamageDetailUI->TargetCharacter = InDamageCharacter;
		NewDamageDetailUI->bIsMeleeAttack = bIsMeleeAttack;
		NewDamageDetailUI->BattleController = BattleController;
		NewDamageDetailUI->DamageData = InDamageData.Value;
		NewDamageDetailUI->MainTarget = MainTarget;

		DamageDetailsList->AddChild(NewDamageDetailUI);
	}
}

void UUW_DamageDetailsListUI::ShowDamageDetailsUIForAbility(const TMap<ACharacterBase*, FDamageData>& InDamageData)
{
	SRPG_IF_CHECK(WBP_DamageDetailsUIClass, TEXT("WBP_DamageDetailsUIClass 클래스가 설정되지 않았습니다."));

	for (const TTuple<ACharacterBase*, FDamageData> DamageData : InDamageData)
	{
		ACharacterBase* InDamageCharacter = DamageData.Key;

		UUW_DamageDetailsUI* NewDamageDetailUI =
			CreateWidget<UUW_DamageDetailsUI>(GetWorld(), WBP_DamageDetailsUIClass);

		NewDamageDetailUI->AttackingCharacter = AttackingCharacter;
		NewDamageDetailUI->TargetCharacter = InDamageCharacter;
		NewDamageDetailUI->bIsMeleeAttack = false;
		NewDamageDetailUI->BattleController = BattleController;
		NewDamageDetailUI->DamageData = DamageData.Value;
		NewDamageDetailUI->MainTarget = nullptr;

		DamageDetailsList->AddChild(NewDamageDetailUI);
	}
}
