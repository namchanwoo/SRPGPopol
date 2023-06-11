#include "InnerSlot.h"

#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

AInnerSlot::AInnerSlot()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	PaperSprite->SetSprite(nullptr);
	Box->SetRelativeLocation(FVector(0.0f, 0.0f, -1.0f));

	bIsInnerSlot = true;
	SlotSetting.SlotSpriteSize = FVector(0.0f, 0.0f, 0.0f);
}

void AInnerSlot::BeginPlay()
{
	Super::BeginPlay();
}

void AInnerSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInnerSlot::MouseOverSlot()
{
	if (OnMouseOverSlot.IsBound())
	{
		OnMouseOverSlot.Broadcast(this, bIsInnerSlot);
	}
}
