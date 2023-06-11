#include "QuestNPCBase.h"

#include "Kismet/GameplayStatics.h"
#include "SRG/Interactables/InteractionDetector.h"
#include "SRG/Quests/QuestBase.h"
#include "SRG/Quests/QuestLogic.h"
#include "SRG/Widgets/ExploreWidgets/UW_InfoDialogue.h"
#include "SRGCore/Utilities/AssetTableRef.h"


AQuestNPCBase::AQuestNPCBase()
{
	// 설정이 필요한 기본적인 변수들을 초기화합니다.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// 이 클래스에 필요한 컴포넌트들을 초기화합니다.
	InitializeCustomComponents();

	// 이 클래스에 필요한 정보 팝업 위젯 클래스를 초기화합니다.
	InitializeInfoDialogueClass();
}

void AQuestNPCBase::InitializeCustomComponents()
{
	// DefaultSceneRoot 컴포넌트를 초기화하고 RootComponent로 설정합니다.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	// InteractionActor 컴포넌트를 초기화하고 RootComponent에 부착합니다.
	InteractionActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("InteractionActor"));
	InteractionActor->SetChildActorClass(AInteractionDetector::StaticClass());
	InteractionActor->SetupAttachment(RootComponent);

	// QuestActor 컴포넌트를 초기화하고 RootComponent에 부착합니다.
	QuestActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("QuestActor"));
	QuestActor->SetChildActorClass(AQuestLogic::StaticClass());
	QuestActor->SetupAttachment(RootComponent);
}

void AQuestNPCBase::InitializeInfoDialogueClass()
{
	// WBP_InfoDialogue 위젯 클래스를 가져옵니다.
	if (TSubclassOf<UUW_InfoDialogue> WBP_InfoDialogueClassSrc = DT::FindClass<UUW_InfoDialogue>(
		DT_WIDGET_PATH, FName(TEXT("WBP_InfoDialogue"))))
	{
		WBP_InfoDialogueClass = WBP_InfoDialogueClassSrc;
	}
}

void AQuestNPCBase::BeginPlay()
{
	Super::BeginPlay();

	SetupInteractionDetector();

	SetupQuestLogic();
}

void AQuestNPCBase::SetupInteractionDetector()
{
	// InteractionDetector에 Actor의 인스턴스를 가져와 초기화합니다.
	InteractionDetector = Cast<AInteractionDetector>(InteractionActor->GetChildActor());

	SRPG_CHECK(InteractionDetector);

	// InteractionDetector의 이벤트에 함수를 바인딩합니다.
	InteractionDetector->OnInteracted.AddDynamic(this, &AQuestNPCBase::OnInteracted_QuestNPC);

	// OnLoaded 이벤트에 핸들러를 바인딩합니다.
	InteractionDetector->OnLoaded.AddDynamic(this, &AQuestNPCBase::OnLoaded_Handler);
}

void AQuestNPCBase::SetupQuestLogic()
{
	// QuestLogic에 Actor의 인스턴스를 가져와 초기화합니다.
	QuestLogic = Cast<AQuestLogic>(QuestActor->GetChildActor());
	SRPG_CHECK(QuestLogic);

	// QuestLogic의 각 이벤트에 함수를 바인딩합니다.
	QuestLogic->OnQuestUnavailable.AddDynamic(this, &AQuestNPCBase::OnQuestUnavailable_Handle);
	QuestLogic->OnNewQuestReceived.AddDynamic(this, &AQuestNPCBase::OnNewQuestReceived_Handle);
	QuestLogic->OnQuestInProgress.AddDynamic(this, &AQuestNPCBase::OnQuestInProgress_Handle);
	QuestLogic->OnQuestReadyToDeliver.AddDynamic(this, &AQuestNPCBase::OnQuestReadyToDeliver_Handle);
	QuestLogic->OnQuestAlreadyCompleted.AddDynamic(this, &AQuestNPCBase::OnQuestAlreadyCompleted_Handle);
}

UUW_InfoDialogue* AQuestNPCBase::CreateAndSetupDialogueWidget(FString Title, FString YesButtonText,
                                                              FString DialogueText)
{
	if (!WBP_InfoDialogueClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("The InfoDialogueClass class you are trying to spawn does not exist."))
		return nullptr;
	}

	UUW_InfoDialogue* Widget = CreateWidget<UUW_InfoDialogue>(GetWorld(), WBP_InfoDialogueClass);
	Widget->DialogueTitle = FText::FromString(Title);
	Widget->YesButtonText = FText::FromString(YesButtonText);
	Widget->DialogueText = FText::FromString(DialogueText);
	Widget->AddToViewport();
	return Widget;
}


void AQuestNPCBase::OnInteracted_QuestNPC(int32 InButtonIndex)
{
	// NPC와의 상호작용이 감지되면 InteractionDetector를 활성화하고 퀘스트 상태를 확인합니다.
	if (InteractionDetector && QuestLogic)
	{
		InteractionDetector->EnableInteraction();
		QuestLogic->CheckQuestStatus(InteractionDetector->ExploreHero, Quests);
	}
	else
	{
		// 여기에 적절한 오류 처리 또는 로깅
		SRPG_LOG_ERROR(TEXT("InteractionDetector 또는 QuestLogic이 초기화되지 않았습니다."));
	}
}

void AQuestNPCBase::OnLoaded_Handler(bool bBShouldLoadEnemyPawns)
{
	// 게임 상태가 로드되면 InteractionDetector를 활성화합니다.
	if (InteractionDetector)
	{
		InteractionDetector->EnableInteraction();
	}
	else
	{
		// 여기에 적절한 오류 처리 또는 로깅
		SRPG_LOG_ERROR(TEXT("InteractionDetector가 초기화되지 않았습니다."));
	}
}

void AQuestNPCBase::OnQuestUnavailable_Handle(AQuestBase* InQuest)
{
	QuestUnavailableDialogue = CreateAndSetupDialogueWidget(
		TEXT("Quest"), TEXT("Continue"), InQuest->QuestUnavailableText.ToString());
	QuestUnavailableDialogue->OnOkClicked.AddDynamic(this, &AQuestNPCBase::OnQuestUnavailableConfirmed);
}


void AQuestNPCBase::OnNewQuestReceived_Handle(AQuestBase* InQuest)
{
	ProcessingQuest = InQuest;
	NewQuestReceivedDialogue = CreateAndSetupDialogueWidget(
		TEXT("New Quest가아니라 바보다"), TEXT("Continue도 될가?"), InQuest->NewQuestText.ToString());
	NewQuestReceivedDialogue->OnOkClicked.AddDynamic(this, &AQuestNPCBase::OnNewQuestReceivedConfirmed);
}

void AQuestNPCBase::OnQuestInProgress_Handle(AQuestBase* InQuest)
{
	ProcessingQuest = InQuest;
	QuestInProgressDialogue = CreateAndSetupDialogueWidget(TEXT("Quest"), TEXT("Continue"),
	                                                       InQuest->QuestInProgressText.ToString());
	QuestInProgressDialogue->OnOkClicked.AddDynamic(this, &AQuestNPCBase::OnQuestInProgressConfirmed);
}

void AQuestNPCBase::OnQuestReadyToDeliver_Handle(AQuestBase* InQuest)
{
	ProcessingQuest = InQuest;
	QuestReadyToDeliverDialogue = CreateAndSetupDialogueWidget(
		TEXT("Quest"), TEXT("Continue"), InQuest->QuestReadyToDeliverText.ToString());
	QuestReadyToDeliverDialogue->OnOkClicked.AddDynamic(this, &AQuestNPCBase::OnQuestDeliveredConfirmed);
}

void AQuestNPCBase::OnQuestAlreadyCompleted_Handle(AQuestBase* InQuest)
{
	ProcessingQuest = InQuest;
	QuestAlreadyCompletedDialogue = CreateAndSetupDialogueWidget(
		TEXT("Quest"), TEXT("Continue"), InQuest->QuestAlreadyCompletedText.ToString());
	QuestAlreadyCompletedDialogue->OnOkClicked.AddDynamic(this, &AQuestNPCBase::OnQuestAlreadyCompletedConfirmed);
}

void AQuestNPCBase::OnQuestUnavailableConfirmed()
{
	UGameplayStatics::SetGamePaused(this, false);
	QuestUnavailableDialogue->RemoveFromParent();
}

void AQuestNPCBase::OnNewQuestReceivedConfirmed()
{
	QuestLogic->ExploreHero->AddQuest(ProcessingQuest);
	UGameplayStatics::SetGamePaused(this, false);
	NewQuestReceivedDialogue->RemoveFromParent();
}

void AQuestNPCBase::OnQuestInProgressConfirmed()
{
	UGameplayStatics::SetGamePaused(this, false);
	QuestInProgressDialogue->RemoveFromParent();
}

void AQuestNPCBase::OnQuestDeliveredConfirmed()
{
	QuestLogic->ExploreHero->DeliverQuest(ProcessingQuest);
	QuestReadyToDeliverDialogue->RemoveFromParent();
}

void AQuestNPCBase::OnQuestAlreadyCompletedConfirmed()
{
	UGameplayStatics::SetGamePaused(this, false);
	QuestAlreadyCompletedDialogue->RemoveFromParent();
}
