#include "GridOverlaySlot.h"

#include "PaperSpriteComponent.h"


AGridOverlaySlot::AGridOverlaySlot()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다. 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PaperSprite->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f));
	PaperSprite->SetSpriteColor(FLinearColor(1.0f,1.0f,1.0f,1.0f));
	PaperSprite->SetSprite(nullptr);
}

// Called when the game starts or when spawned
void AGridOverlaySlot::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGridOverlaySlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
