
#include "ExplorePlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "SRG/Widgets/ExploreWidgets/UW_ExploreCursor.h"

AExplorePlayerController::AExplorePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultClickTraceChannel = ECollisionChannel::ECC_Camera;
}

void AExplorePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	WBP_ExploreCursor = CreateWidget<UUW_ExploreCursor>(this, WBP_ExploreCursorClass);
	SetMouseCursorWidget(EMouseCursor::Default, WBP_ExploreCursor);
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, nullptr, EMouseLockMode::DoNotLock, false);

	
}

void AExplorePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/*UStrategyEnhancedInputComponent* StrategyEnhancedInputComponent = Cast<UStrategyEnhancedInputComponent>(InputComponent);
	SRPG_CHECK(StrategyEnhancedInputComponent);
	StrategyEnhancedInputComponent->BindActionByTag(DefaultInputConfig,SRPGTAGS::Input,ETriggerEvent::Started,this,)*/
}


