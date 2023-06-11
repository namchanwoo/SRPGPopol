#include "Slot.h"

#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

ASlot::ASlot()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다.
	// 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetBoxExtent(FVector(50.0f, 50.0f, 2.0f));
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionProfileName(TEXT("Custom"));
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Box->SetVisibility(false);
	Box->SetupAttachment(DefaultSceneRoot);

	SlotSetting.SlotSpriteSize = FVector(1.1f, 1.1f, 1.1f);
}

void ASlot::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnClicked.AddDynamic(this, &ASlot::OnClickedHandler);
	Box->OnBeginCursorOver.AddDynamic(this, &ASlot::OnBeginCursorOverHandler);
	Box->OnEndCursorOver.AddDynamic(this, &ASlot::OnEndCursorOverHandler);
}

void ASlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlot::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Box->SetBoxExtent(SlotSetting.SlotSize);
	PaperSprite->SetWorldScale3D(SlotSetting.SlotSpriteSize);
}

void ASlot::ClearAllCallbacks()
{
	OnMouseOverSlot.RemoveAll(this);
	OnMouseLeaveSlot.RemoveAll(this);
	OnSlotClicked.RemoveAll(this);
}

void ASlot::MouseOverSlot()
{
	if (OnMouseOverSlot.IsBound())
	{
		OnMouseOverSlot.Broadcast(this, bIsInnerSlot);
	}
}

void ASlot::OnClickedHandler(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (OnSlotClicked.IsBound())
	{
		OnSlotClicked.Broadcast(this, ButtonPressed);
	}
}

void ASlot::OnBeginCursorOverHandler(UPrimitiveComponent* TouchedComponent)
{
	MouseOverSlot();
}

void ASlot::OnEndCursorOverHandler(UPrimitiveComponent* TouchedComponent)
{
	if (OnMouseLeaveSlot.IsBound())
	{
		OnMouseLeaveSlot.Broadcast(this);
	}
}
