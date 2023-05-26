// Fill out your copyright notice in the Description page of Project Settings.


#include "SRG/Widgets/ExploreWidgets/UW_InfoDialogue.h"

#include "UW_MenuButton.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UUW_InfoDialogue::UUW_InfoDialogue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_InfoDialogue::NativePreConstruct()
{
	Super::NativePreConstruct();

	Description->SetText(DialogueText);
	Title->SetText(DialogueTitle);
	OkButton->SetButtonText(YesButtonText);
}

void UUW_InfoDialogue::NativeConstruct()
{
	Super::NativeConstruct();

	Background->OnMouseButtonDownEvent.BindUFunction(this, FName("OnBackgroundMouseButtonDown"));
	OkButton->OnButtonClicked.AddDynamic(this, &UUW_InfoDialogue::OnButtonClicked);
}

void UUW_InfoDialogue::SetUpInfoDialogue()
{
	Description->SetText(DialogueText);
	Title->SetText(DialogueTitle);
	OkButton->SetButtonText(YesButtonText);
}

void UUW_InfoDialogue::OnButtonClicked()
{
	if (OnOkClicked.IsBound())
		OnOkClicked.Broadcast();
}

FEventReply UUW_InfoDialogue::OnBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	// Your logic for handling the mouse button down event goes here.
	// UE_LOG(LogTemp, Warning, TEXT("Background OnMouseButtonDown triggered"));

	FEventReply Reply;
	Reply.NativeReply = FReply::Handled(); // Use FReply::Handled() if you want to consume the event.
	return Reply;
}
