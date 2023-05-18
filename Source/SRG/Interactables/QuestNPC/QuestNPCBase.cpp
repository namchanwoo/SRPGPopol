#include "QuestNPCBase.h"

#include "Kismet/GameplayStatics.h"
#include "SRG/Interactables/InteractionDetector.h"
#include "SRG/Quests/QuestBase.h"
#include "SRG/Quests/QuestLogic.h"
#include "SRG/Widgets/ExploreWidgets/UW_InfoDialogue.h"
#include "SRGCore/AssetTableRef.h"


AQuestNPCBase::AQuestNPCBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Create the ChildActorComponent and set its ChildActorClass
	InteractionActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("InteractionActor"));
	InteractionActor->SetChildActorClass(AInteractionDetector::StaticClass());
	InteractionActor->SetupAttachment(RootComponent);

	// Create the ChildActorComponent and set its ChildActorClass
	QuestActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("QuestActor"));
	QuestActor->SetChildActorClass(AQuestLogic::StaticClass());
	QuestActor->SetupAttachment(RootComponent);

	auto WBP_InfoDialogue = DT::FindClass<UUW_InfoDialogue>(DT_BLUEPRINT_PATH, FName(TEXT("WBP_InfoDialogue")));
	if (WBP_InfoDialogue)
	{
		InfoDialogueClass = WBP_InfoDialogue;
	}
}

void AQuestNPCBase::BeginPlay()
{
	Super::BeginPlay();

	InteractionDetector = Cast<AInteractionDetector>(InteractionActor->GetChildActor());

	InteractionDetector->OnInteracted.AddDynamic(this, &AQuestNPCBase::OnInteracted_QuestNPC);

	QuestLogic = Cast<AQuestLogic>(QuestActor->GetChildActor());

	InteractionDetector->OnLoaded.AddDynamic(this, &AQuestNPCBase::OnLoaded_Handler);
	QuestLogic->OnQuestUnavailable.AddDynamic(this, &AQuestNPCBase::OnQuestUnavailable_Handle);
	QuestLogic->OnNewQuestReceived.AddDynamic(this, &AQuestNPCBase::OnNewQuestReceived_Handle);
	QuestLogic->OnQuestInProgress.AddDynamic(this, &AQuestNPCBase::OnQuestInProgress_Handle);
	QuestLogic->OnQuestReadyToDeliver.AddDynamic(this, &AQuestNPCBase::OnQuestReadyToDeliver_Handle);
	QuestLogic->OnQuestAlreadyCompleted.AddDynamic(this, &AQuestNPCBase::OnQuestAlreadyCompleted_Handle);
	
}

void AQuestNPCBase::OnInteracted_QuestNPC(int32 InButtonIndex)
{
	InteractionDetector->EnableInteraction();
	QuestLogic->CheckQuestStatus(InteractionDetector->ExploreHero, Quests);
}

void AQuestNPCBase::OnLoaded_Handler(bool bBShouldLoadEnemyPawns)
{
	InteractionDetector->EnableInteraction();
}

void AQuestNPCBase::OnQuestUnavailable_Handle(AQuestBase* InQuest)
{
	if (!InfoDialogueClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("스폰할려는 InfoDialogueClass 클래스가 없습니다."))
		return;
	}

	InfoDialogueWidget = CreateWidget<UUW_InfoDialogue>(GetWorld(), InfoDialogueClass);
	InfoDialogueWidget->DialogueTitle = FText::FromString(TEXT("Quest"));
	InfoDialogueWidget->YesButtonText = FText::FromString(TEXT("Continue"));
	InfoDialogueWidget->DialogueText = InQuest->QuestUnavailableText;

	InfoDialogueWidget->OnOkClicked.AddDynamic(this, &AQuestNPCBase::OnQuestUnavailableConfirmed);
	InfoDialogueWidget->AddToViewport();
}

void AQuestNPCBase::OnQuestUnavailableConfirmed()
{
	UGameplayStatics::SetGamePaused(this, false);
	InfoDialogueWidget->RemoveFromParent();
}

void AQuestNPCBase::OnNewQuestReceivedConfirmed()
{
	InfoDialogueWidget->RemoveFromParent();

	QuestLogic->ExploreHero->AddQuest(ProcessingQuest);
	UGameplayStatics::SetGamePaused(this, false);
}

void AQuestNPCBase::OnQuestInProgressConfirmed()
{
	UGameplayStatics::SetGamePaused(this, false);
	InfoDialogueWidget->RemoveFromParent();
}

void AQuestNPCBase::OnQuestDeliveredConfirmed()
{
	QuestLogic->ExploreHero->DeliverQuest(ProcessingQuest);
	InfoDialogueWidget->RemoveFromParent();
}

void AQuestNPCBase::OnQuestAlreadyCompletedConfirmed()
{
	UGameplayStatics::SetGamePaused(this, false);
	InfoDialogueWidget->RemoveFromParent();
}

void AQuestNPCBase::OnNewQuestReceived_Handle(AQuestBase* InQuest)
{
	if (!InfoDialogueClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("스폰할려는 InfoDialogueClass 클래스가 없습니다."))
		return;
	}

	ProcessingQuest = InQuest;

	InfoDialogueWidget = CreateWidget<UUW_InfoDialogue>(GetWorld(), InfoDialogueClass);
	InfoDialogueWidget->DialogueTitle = FText::FromString(TEXT("New Quest"));
	InfoDialogueWidget->YesButtonText = FText::FromString(TEXT("Continue"));
	InfoDialogueWidget->DialogueText = InQuest->NewQuestText;

	InfoDialogueWidget->OnOkClicked.AddDynamic(this, &AQuestNPCBase::OnNewQuestReceivedConfirmed);
	InfoDialogueWidget->AddToViewport();
}

void AQuestNPCBase::OnQuestInProgress_Handle(AQuestBase* InQuest)
{
	if (!InfoDialogueClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("스폰할려는 InfoDialogueClass 클래스가 없습니다."))
		return;
	}

	ProcessingQuest = InQuest;

	InfoDialogueWidget = CreateWidget<UUW_InfoDialogue>(GetWorld(), InfoDialogueClass);
	InfoDialogueWidget->DialogueTitle = FText::FromString(TEXT("Quest"));
	InfoDialogueWidget->YesButtonText = FText::FromString(TEXT("Continue"));
	InfoDialogueWidget->DialogueText = InQuest->QuestInProgressText;

	InfoDialogueWidget->OnOkClicked.AddDynamic(this, &AQuestNPCBase::OnQuestInProgressConfirmed);
	InfoDialogueWidget->AddToViewport();
}

void AQuestNPCBase::OnQuestReadyToDeliver_Handle(AQuestBase* InQuest)
{
	if (!InfoDialogueClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("스폰할려는 InfoDialogueClass 클래스가 없습니다."))
		return;
	}

	ProcessingQuest = InQuest;

	InfoDialogueWidget = CreateWidget<UUW_InfoDialogue>(GetWorld(), InfoDialogueClass);
	InfoDialogueWidget->DialogueTitle = FText::FromString(TEXT("Quest"));
	InfoDialogueWidget->YesButtonText = FText::FromString(TEXT("Continue"));
	InfoDialogueWidget->DialogueText = InQuest->QuestReadyToDeliverText;

	InfoDialogueWidget->OnOkClicked.AddDynamic(this, &AQuestNPCBase::OnQuestDeliveredConfirmed);
	InfoDialogueWidget->AddToViewport();
}

void AQuestNPCBase::OnQuestAlreadyCompleted_Handle(AQuestBase* InQuest)
{
	if (!InfoDialogueClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("스폰할려는 InfoDialogueClass 클래스가 없습니다."))
		return;
	}

	ProcessingQuest = InQuest;

	InfoDialogueWidget = CreateWidget<UUW_InfoDialogue>(GetWorld(), InfoDialogueClass);
	InfoDialogueWidget->DialogueTitle = FText::FromString(TEXT("Quest"));
	InfoDialogueWidget->YesButtonText = FText::FromString(TEXT("Continue"));
	InfoDialogueWidget->DialogueText = InQuest->QuestAlreadyCompletedText;

	InfoDialogueWidget->OnOkClicked.AddDynamic(this, &AQuestNPCBase::OnQuestAlreadyCompletedConfirmed);
	InfoDialogueWidget->AddToViewport();
}

