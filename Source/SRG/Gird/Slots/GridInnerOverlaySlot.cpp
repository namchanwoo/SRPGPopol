
#include "SRG/Gird/Slots/GridInnerOverlaySlot.h"
#include "PaperSpriteComponent.h"

AGridInnerOverlaySlot::AGridInnerOverlaySlot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PaperSprite->SetRelativeLocation(FVector(0.0f, 0.0f, 1.1f));
	PaperSprite->SetSprite(nullptr);
}

void AGridInnerOverlaySlot::BeginPlay()
{
	Super::BeginPlay();
}

void AGridInnerOverlaySlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
