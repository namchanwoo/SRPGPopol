#include "BattlePlayerController.h"

#include "BattleController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SRG/Gird/Grid.h"
#include "SRG/Widgets/UW_BattleCursor.h"
#include "SRG/Widgets/ExploreWidgets/UW_EmptyMouseCursor.h"
#include "SRGCore/Utilities/AssetTableRef.h"
#include "SRGCore/Utilities/SRGLog.h"


ABattlePlayerController::ABattlePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	if (const TSubclassOf<UUW_BattleCursor> WBP_BattleCursor_Src = DT::FindClass<UUW_BattleCursor>(
		DT_WIDGET_PATH,TEXT("WBP_BattleCursor")))
	{
		WBP_BattleCursorClass = WBP_BattleCursor_Src;
	}

	if (const TSubclassOf<UUW_EmptyMouseCursor> WBP_EmptyMouseCursor_Src = DT::FindClass<UUW_EmptyMouseCursor>(
		DT_WIDGET_PATH,TEXT("WBP_EmptyMouseCursor")))
	{
		WBP_EmptyMouseCursorClass = WBP_EmptyMouseCursor_Src;
	}
}

void ABattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SRPG_IF_CHECK(WBP_BattleCursorClass, TEXT("WBP_BattleCursorClass 레퍼런스를 설정해주세요."));
	SRPG_IF_CHECK(WBP_EmptyMouseCursorClass, TEXT("WBP_EmptyMouseCursorClass 레퍼런스를 설정해주세요."));


	AActor* worldGridActor = UGameplayStatics::GetActorOfClass(this, AGrid::StaticClass());
	SRPG_CHECK(worldGridActor);
	Grid = Cast<AGrid>(worldGridActor);

	Grid->OnCursorChanged.AddDynamic(this, &ABattlePlayerController::OnCursorChangedHandler);
	Grid->OnCursorAngleChanged.AddDynamic(this, &ABattlePlayerController::OnCursorAngleChangedHandler);

	UW_BattleCursor = CreateWidget<UUW_BattleCursor>(GetWorld(), WBP_BattleCursorClass);
	UW_BattleCursor->AddToViewport(999);

	UW_EmptyMouseCursor = CreateWidget<UUW_EmptyMouseCursor>(GetWorld(), WBP_EmptyMouseCursorClass);
	SetMouseCursorWidget(EMouseCursor::Default, UW_EmptyMouseCursor);
}

void ABattlePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//마우스 커서 위치를 조정합니다.
	if (IsValid(UW_BattleCursor))
	{
		FVector2D NewPosition;
		UWidgetLayoutLibrary::GetMousePositionScaledByDPI(this, NewPosition.X, NewPosition.Y);
		UW_BattleCursor->SetPositionInViewport(NewPosition);
	}
}

void ABattlePlayerController::OnCursorChangedHandler(EBattleCursorType InCursorType,
                                                     AActiveAbilityBase* InActiveAbility, ASpellBase* InSpell,
                                                     TSubclassOf<ACharacterBase> InCharacter)
{
	if (IsValid(UW_BattleCursor))
	{
		UW_BattleCursor->ChangeCursor(InCursorType, InActiveAbility, InSpell, InCharacter);
	}
}

void ABattlePlayerController::OnCursorAngleChangedHandler(float InAngle)
{
	if (IsValid(UW_BattleCursor))
	{
		UW_BattleCursor->SetCursorAngle(InAngle);
	}
}
