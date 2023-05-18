#include "InteractionDetector.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "SRG/Core/SRGGameInstance.h"
#include "SRG/Widgets/ExploreWidgets/UW_InteractionUI.h"
#include "SRGCore/AssetTableRef.h"

AInteractionDetector::AInteractionDetector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	// SphereComponent 생성
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	// AreaClass 속성 설정
	InteractionSphere->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
	InteractionSphere->SetupAttachment(RootComponent);


	// WidgetComponent 생성
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	// 공간 속성 설정
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//Widget 클래스 설정
	TSubclassOf<UUW_InteractionUI> WBP_InteractionUI = DT::FindClass<UUW_InteractionUI>(DT_BLUEPRINT_PATH,TEXT("WBP_InteractionUI"));
	if (WBP_InteractionUI)
	{
		InteractionWidget->SetWidgetClass(WBP_InteractionUI);
	}
	// DrawSize 속성 설정	
	InteractionWidget->SetDrawSize(FVector2D(200.f, 50.f));
	// bDrawAtDesiredSize 속성 설정
	InteractionWidget->SetDrawAtDesiredSize(true);
	InteractionWidget->SetupAttachment(RootComponent);

	InteractionUILocation = FVector(0.0f, 0.0f, 50.0f);

	InteractionTexts.Add(FText::FromString(TEXT("Interact")));
}

void AInteractionDetector::OnConstruction(const FTransform& Transform)
{
	InteractionSphere->SetSphereRadius(InteractionRadius);
	InteractionWidget->SetRelativeLocation(InteractionUILocation);
}

void AInteractionDetector::BeginPlay()
{
	Super::BeginPlay();

	InteractionUI = Cast<UUW_InteractionUI>(InteractionWidget->GetUserWidgetObject());
	if (!InteractionUI)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("InteractionUI로 캐스팅에 실패했습니다"));
		return;
	}

	InteractionUI->SetInteractionTexts(InteractionTexts);
	InteractionUI->InstantHide();
	InteractionUI->OnInteractClicked.AddDynamic(this, &AInteractionDetector::OnInteractClicked_InteractButton);
}


void AInteractionDetector::SetExploreHero(AExploreHeroBase* InExploreHero)
{
	ExploreHero = InExploreHero;
}

void AInteractionDetector::EnableInteraction()
{
	bCanInteract = true;
	if (IsValid(ExploreHero))
	{
		ExploreHero->RefreshInteractions();
	}
}

void AInteractionDetector::DisableInteraction()
{
	bCanInteract = false;
}

void AInteractionDetector::Load(bool IsShouldLoadEnemyPawns)
{
	DisableInteraction();

	if (OnLoaded.IsBound())
		OnLoaded.Broadcast(IsShouldLoadEnemyPawns);
}

void AInteractionDetector::ShowInteractionUI()
{
	InteractionUI->ShowInteractionUI();
	if (OnInteractionAppeared.IsBound())
		OnInteractionAppeared.Broadcast();
}

void AInteractionDetector::HideInteractionUI()
{
	InteractionUI->HideInteractionUI();
	if (OnInteractionDisappeared.IsBound())
		OnInteractionDisappeared.Broadcast();
}

void AInteractionDetector::OnInteractClicked_InteractButton(int32 ButtonIndex)
{
	bCanInteract = false;
	InteractionUI->InstantHide();

	USRGGameInstance* SRGGameInstance = Cast<USRGGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!SRGGameInstance)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("USRGGameInstance 캐스팅에 실패했습니다"));
		return;
	}

	SRGGameInstance->AddInteractions(this);
	ExploreHero->UpdateInteractionQuest(GetParentActor());

	if (OnInteracted.IsBound())
		OnInteracted.Broadcast(ButtonIndex);
}
