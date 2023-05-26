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
	PrimaryActorTick.bCanEverTick = true;

	SetupDefaultSceneRoot();
	SetupInteractionSphere();
	SetupInteractionWidget();
}

void AInteractionDetector::SetupDefaultSceneRoot()
{
	// 기본 씬 루트 생성 및 설정
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
}

void AInteractionDetector::SetupInteractionSphere()
{
	// 상호작용 구체 생성 및 설정
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
	InteractionSphere->SetupAttachment(RootComponent);
}

void AInteractionDetector::SetupInteractionWidget()
{
	// 상호작용 위젯 생성 및 설정
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);

	// 상호작용 UI 설정
	if (TSubclassOf<UUW_InteractionUI> WBP_InteractionUIClass =
		DT::FindClass<UUW_InteractionUI>(DT_BLUEPRINT_PATH,TEXT("WBP_InteractionUI")))
	{
		InteractionWidget->SetWidgetClass(WBP_InteractionUIClass);
	}

	InteractionWidget->SetDrawSize(FVector2D(200.f, 50.f));
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
	InteractionUI->OnInteractClicked.AddDynamic(this, &AInteractionDetector::OnInteractClicked_InteractionUI);
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

void AInteractionDetector::OnInteractClicked_InteractionUI(int32 ButtonIndex)
{
	// 상호작용 가능성을 비활성화 합니다.
	bCanInteract = false;

	// 상호작용 UI를 즉시 숨깁니다.
	InteractionUI->InstantHide();

	// 게임 인스턴스를 얻고 SRGGameInstance로 캐스팅합니다.
	USRGGameInstance* SRGGameInstance = Cast<USRGGameInstance>(UGameplayStatics::GetGameInstance(this));

	// 캐스팅에 실패하면 오류 로그를 출력하고 함수를 종료합니다.
	if (!SRGGameInstance)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("Failed to cast USRGGameInstance"));
		return;
	}

	// 상호작용을 추가합니다.
	SRGGameInstance->AddInteractions(this);

	// ExploreHero의 상호작용 퀘스트를 업데이트합니다.
	ExploreHero->UpdateInteractionQuest(GetParentActor());

	// OnInteracted 이벤트가 바인딩 되어 있으면 이를 방송합니다.
	if (OnInteracted.IsBound())
	{
		OnInteracted.Broadcast(ButtonIndex);
	}
}
