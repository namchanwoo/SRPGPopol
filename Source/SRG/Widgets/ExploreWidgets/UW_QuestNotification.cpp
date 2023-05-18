#include "SRG/Widgets/ExploreWidgets/UW_QuestNotification.h"

#include "Animation/WidgetAnimation.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SRG/Quests/QuestBase.h"

UUW_QuestNotification::UUW_QuestNotification(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_QuestNotification::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAppearAnimation();

	AQuestBase* QuestCDO = Quest->GetDefaultObject<AQuestBase>();
	QuestName->SetText(QuestCDO->Name);

	SetQuestStatus();


	const float InPercent = QuestStepData.CurrentStep / QuestStepData.TotalSteps;
	QuestBar->SetPercent(InPercent);

	const FString FormulaString = FString::Printf(TEXT("%d / %d"), QuestStepData.CurrentStep, QuestStepData.TotalSteps);
	const FText InText = FText::FromString(FormulaString);
	ProgressDetailsText->SetText(InText);

	FTimerHandle WaitTimerHandle;
	float WaitTime = 5.0f;
	GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		PlayDisappearAnimation();
		GetWorld()->GetTimerManager().ClearTimer(WaitTimerHandle);
	}), WaitTime, false); // 반복하려면 false를 true로 변경
}

void UUW_QuestNotification::PlayAppearAnimation()
{
	PlayAnimation(Appear);
}

void UUW_QuestNotification::PlayDisappearAnimation()
{
	PlayAnimation(Disappear);

	FTimerHandle WaitTimerHandle;
	const float WaitTime = Disappear->GetEndTime();
	GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		RemoveFromParent();

		GetWorld()->GetTimerManager().ClearTimer(WaitTimerHandle);
	}), WaitTime, false); // 반복하려면 false를 true로 변경
}

void UUW_QuestNotification::PlayQuestSound(USoundBase* InSound)
{
	if (IsValid(InSound))
	{
		UGameplayStatics::PlaySound2D(this, InSound);
	}
}

void UUW_QuestNotification::SetQuestStatus()
{
	if (QuestStepData.CurrentStep == QuestStepData.TotalSteps)
	{
		CompletedQuest();
		return;
	}

	if (QuestStepData.CurrentStep == 0)
	{
		NewQuest();
	}
	else
	{
		UpdateQuest();
	}
}

void UUW_QuestNotification::NewQuest()
{
	QuestStatusText->SetText(FText::FromString(TEXT("New Quest !")));
	AQuestBase* QuestCDO = Quest->GetDefaultObject<AQuestBase>();
	PlayQuestSound(QuestCDO->QuestSounds.NewQuestSound);
}

void UUW_QuestNotification::UpdateQuest()
{
	QuestStatusText->SetText(FText::FromString(TEXT("Quest Updated !")));
	AQuestBase* QuestCDO = Quest->GetDefaultObject<AQuestBase>();
	PlayQuestSound(QuestCDO->QuestSounds.QuestUpdateSound);
}

void UUW_QuestNotification::CompletedQuest()
{
	QuestStatusText->SetText(FText::FromString(TEXT("Quest Completed !")));
	AQuestBase* QuestCDO = Quest->GetDefaultObject<AQuestBase>();
	PlayQuestSound(QuestCDO->QuestSounds.QuestCompletedSound);
}
