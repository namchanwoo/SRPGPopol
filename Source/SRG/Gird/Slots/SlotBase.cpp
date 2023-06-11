#include "SlotBase.h"

#include "Components/TextRenderComponent.h"
#include "Kismet/KismetTextLibrary.h"
#include "Materials/MaterialInstanceConstant.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "SRG/Units/Obstacles/ObstacleBase.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "SRG/Abilities/Auras/AuraBase.h"
#include "SRGCore/Utilities/AssetTableRef.h"

#include "Paper2D/Classes/PaperSprite.h"


ASlotBase::ASlotBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	// 새 TextRenderComponent 생성
	TotalCostText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TotalCostText"));
	TotalCostText->SetText(FText::GetEmpty());
	TotalCostText->SetHorizontalAlignment(EHTA_Center);
	TotalCostText->SetRelativeLocation(FVector(-0.000000, 10.000002, 1.675277));
	TotalCostText->SetRelativeRotation(FRotator(90.000000, -90.000000, 180.000000));
	TotalCostText->SetupAttachment(DefaultSceneRoot);


	// 새 PaperSpriteComponent 생성
	PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));


	if (UPaperSprite* S_1x1DefaultSlot_Src = DT::FindObject<UPaperSprite>(
		DT_TEXTURES_PATH, FName(TEXT("S_1x1DefaultSlot"))))
	{
		PaperSprite->SetSprite(S_1x1DefaultSlot_Src);
	}
	PaperSprite->SetSpriteColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.1f));

	// Set the override material
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> OverrideMaterial(
		TEXT("MaterialInstanceConstant'/Paper2D/TranslucentUnlitSpriteMaterial.TranslucentUnlitSpriteMaterial'"));
	if (OverrideMaterial.Succeeded())
	{
		PaperSprite->SetMaterial(0, OverrideMaterial.Object);
	}

	// Set the collision profile name and responses
	PaperSprite->BodyInstance.SetCollisionProfileName("Custom");
	PaperSprite->BodyInstance.SetResponseToAllChannels(ECR_Ignore);
	PaperSprite->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f));
	PaperSprite->SetRelativeRotation(FRotator(0.0f, 0.0f, -89.999985f));
	PaperSprite->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.1f));
	PaperSprite->SetupAttachment(DefaultSceneRoot);

	SlotSetting.SlotSize = FVector(60.0f, 60.0f, 2.0f);

	
	SetSlotSprite(SlotSetting.SlotSprites1_1.DefaultSlotSprite, TEXT("S_1x1DefaultSlot"));
	SetSlotSprite(SlotSetting.SlotSprites1_1.MovableSlotSprite, TEXT("S_1x1MovableSlot"));
	SetSlotSprite(SlotSetting.SlotSprites1_1.ActiveSlotSprite, TEXT("S_1x1ActiveSlot"));
	SetSlotSprite(SlotSetting.SlotSprites1_1.EnemySlotSprite, TEXT("S_1x1EnemySlot"));
	SetSlotSprite(SlotSetting.SlotSprites1_1.AllySlotSprite, TEXT("S_1x1AllySlot"));
	SetSlotSprite(SlotSetting.SlotSprites1_1.MovementOverlaySlotSprite, TEXT("S_1x1MovementOverlaySlot"));
	SetSlotSprite(SlotSetting.SlotSprites1_1.SpellSlotSprite, TEXT("S_1x1SpellSlot"));

	SetSlotSprite(SlotSetting.SlotSprites2_2.DefaultSlotSprite, TEXT("S_2x2DefaultSlot"));
	SetSlotSprite(SlotSetting.SlotSprites2_2.MovableSlotSprite, TEXT("S_2x2MovementSlot"));
	SetSlotSprite(SlotSetting.SlotSprites2_2.ActiveSlotSprite, TEXT("S_2x2ActiveSlot"));
	SetSlotSprite(SlotSetting.SlotSprites2_2.EnemySlotSprite, TEXT("S_2x2EnemySlot"));
	SetSlotSprite(SlotSetting.SlotSprites2_2.AllySlotSprite, TEXT("S_2x2AllySlot"));
	SetSlotSprite(SlotSetting.SlotSprites2_2.MovementOverlaySlotSprite, TEXT("S_2x2MovementOverlaySlot"));
	SetSlotSprite(SlotSetting.SlotSprites2_2.SpellSlotSprite, TEXT("S_2x2SpellSlot"));

	SetSlotSprite(SlotSetting.SlotSprites3_3.DefaultSlotSprite, TEXT("S_3x3DefaultSlot"));
	SetSlotSprite(SlotSetting.SlotSprites3_3.MovableSlotSprite, TEXT("S_3x3MovableSlot"));
	SetSlotSprite(SlotSetting.SlotSprites3_3.ActiveSlotSprite, TEXT("S_3x3ActiveSlot"));
	SetSlotSprite(SlotSetting.SlotSprites3_3.EnemySlotSprite, TEXT("S_3x3EnemySlot"));
	SetSlotSprite(SlotSetting.SlotSprites3_3.AllySlotSprite, TEXT("S_3x3AllySlot"));
	SetSlotSprite(SlotSetting.SlotSprites3_3.MovementOverlaySlotSprite, TEXT("S_3x3MovementOverlaySlot"));
	SetSlotSprite(SlotSetting.SlotSprites3_3.SpellSlotSprite, TEXT("S_3x3SpellSlot"));

	SetSlotSprite(SlotSetting.SlotSprites4_4.DefaultSlotSprite, TEXT("S_4x4DefaultSlot"));
	SetSlotSprite(SlotSetting.SlotSprites4_4.MovableSlotSprite, TEXT("S_4x4MovableSlot"));
	SetSlotSprite(SlotSetting.SlotSprites4_4.ActiveSlotSprite, TEXT("S_4x4ActiveSlot"));
	SetSlotSprite(SlotSetting.SlotSprites4_4.EnemySlotSprite, TEXT("S_4x4EnemySlot"));
	SetSlotSprite(SlotSetting.SlotSprites4_4.AllySlotSprite, TEXT("S_4x4AllySlot"));
	SetSlotSprite(SlotSetting.SlotSprites4_4.MovementOverlaySlotSprite, TEXT("S_4x4MovementOverlaySlot"));
	SetSlotSprite(SlotSetting.SlotSprites4_4.SpellSlotSprite, TEXT("S_4x4SpellSlot"));
	
	SetSlotSprite(SlotSetting.SlotSprites5_5.DefaultSlotSprite, TEXT("S_5x5DefaultSlot"));
	SetSlotSprite(SlotSetting.SlotSprites5_5.MovableSlotSprite, TEXT("S_5x5MovableSlot"));
	SetSlotSprite(SlotSetting.SlotSprites5_5.ActiveSlotSprite, TEXT("S_5x5ActiveSlot"));
	SetSlotSprite(SlotSetting.SlotSprites5_5.EnemySlotSprite, TEXT("S_5x5EnemySlot"));
	SetSlotSprite(SlotSetting.SlotSprites5_5.AllySlotSprite, TEXT("S_5x5AllySlot"));
	SetSlotSprite(SlotSetting.SlotSprites5_5.MovementOverlaySlotSprite, TEXT("S_5x5MovementOverlaySlot"));
	SetSlotSprite(SlotSetting.SlotSprites5_5.SpellSlotSprite, TEXT("S_5x5SpellSlot"));

	SetSlotSprite(SlotSetting.SlotSprites6_6.DefaultSlotSprite, TEXT("S_6x6DefaultSlot"));
	SetSlotSprite(SlotSetting.SlotSprites6_6.MovableSlotSprite, TEXT("S_6x6MovableSlot"));
	SetSlotSprite(SlotSetting.SlotSprites6_6.ActiveSlotSprite, TEXT("S_6x6ActiveSlot"));
	SetSlotSprite(SlotSetting.SlotSprites6_6.EnemySlotSprite, TEXT("S_6x6EnemySlot"));
	SetSlotSprite(SlotSetting.SlotSprites6_6.AllySlotSprite, TEXT("S_6x6AllySlot"));
	SetSlotSprite(SlotSetting.SlotSprites6_6.MovementOverlaySlotSprite, TEXT("S_6x6MovementOverlaySlot"));
	SetSlotSprite(SlotSetting.SlotSprites6_6.SpellSlotSprite, TEXT("S_6x6SpellSlot"));

	SetSlotSprite(SlotSetting.SlotSprites7_7.DefaultSlotSprite, TEXT("S_7x7DefaultSlot"));
	SetSlotSprite(SlotSetting.SlotSprites7_7.MovableSlotSprite, TEXT("S_7x7MovableSlot"));
	SetSlotSprite(SlotSetting.SlotSprites7_7.ActiveSlotSprite, TEXT("S_7x7ActiveSlot"));
	SetSlotSprite(SlotSetting.SlotSprites7_7.EnemySlotSprite, TEXT("S_7x7EnemySlot"));
	SetSlotSprite(SlotSetting.SlotSprites7_7.AllySlotSprite, TEXT("S_7x7AllySlot"));
	SetSlotSprite(SlotSetting.SlotSprites7_7.MovementOverlaySlotSprite, TEXT("S_7x7MovementOverlaySlot"));
	SetSlotSprite(SlotSetting.SlotSprites7_7.SpellSlotSprite, TEXT("S_7x7SpellSlot"));

	SetSlotSprite(SlotSetting.SlotSprites8_8.DefaultSlotSprite, TEXT("S_8x8DefaultSlot"));
	SetSlotSprite(SlotSetting.SlotSprites8_8.MovableSlotSprite, TEXT("S_8x8MovableSlot"));
	SetSlotSprite(SlotSetting.SlotSprites8_8.ActiveSlotSprite, TEXT("S_8x8ActiveSlot"));
	SetSlotSprite(SlotSetting.SlotSprites8_8.EnemySlotSprite, TEXT("S_8x8EnemySlot"));
	SetSlotSprite(SlotSetting.SlotSprites8_8.AllySlotSprite, TEXT("S_8x8AllySlot"));
	SetSlotSprite(SlotSetting.SlotSprites8_8.MovementOverlaySlotSprite, TEXT("S_8x8MovementOverlaySlot"));
	SetSlotSprite(SlotSetting.SlotSprites8_8.SpellSlotSprite, TEXT("S_8x8SpellSlot"));
}

void ASlotBase::SetSlotSprite(UPaperSprite*& SlotSprite, const FName& SpriteName)
{
	if (UPaperSprite* SpriteSrc = DT::FindObject<UPaperSprite>(DT_TEXTURES_PATH, SpriteName))
	{
		SlotSprite = SpriteSrc;
	}
}

void ASlotBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASlotBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlotBase::InitializeEvent(const FSlotSetting& InSlotSetting)
{
	SlotSetting = InSlotSetting;
	PaperSprite->SetWorldScale3D(SlotSetting.SlotSpriteSize);
}

void ASlotBase::SetSlotType(ESlotType NewSlotType, int32 InSize/*InSize=1*/)
{
	SlotType = NewSlotType;

	FSlotSprites spriteStruct = GetSlotSpriteStruct(InSize);

	UPaperSprite* newSlotPaperSprite = nullptr;
	FLinearColor newPaperSpriteColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (SlotType == ESlotType::DefaultSlot)
	{
		newSlotPaperSprite = spriteStruct.DefaultSlotSprite;
		newPaperSpriteColor.A = 0.1f;
	}
	else if (SlotType == ESlotType::MovableSlot)
	{
		newSlotPaperSprite = spriteStruct.MovableSlotSprite;
		newPaperSpriteColor.A = 0.5f;
	}
	else if (SlotType == ESlotType::ActiveSlot) { newSlotPaperSprite = spriteStruct.ActiveSlotSprite; }
	else if (SlotType == ESlotType::EnemySlot) { newSlotPaperSprite = spriteStruct.EnemySlotSprite; }
	else if (SlotType == ESlotType::AllySlot) { newSlotPaperSprite = spriteStruct.AllySlotSprite; }
	else if (SlotType == ESlotType::MovableSlot) { newSlotPaperSprite = spriteStruct.MovableSlotSprite; }
	else if (SlotType == ESlotType::SpellSlot) { newSlotPaperSprite = spriteStruct.SpellSlotSprite; }
	PaperSprite->SetSprite(newSlotPaperSprite);
	PaperSprite->SetSpriteColor(newPaperSpriteColor);
}

void ASlotBase::SetUnit(AUnitBase* InUnit)
{
	ContainedUnit = InUnit;
}

void ASlotBase::SetPlayerSpawnSlot()
{
	bCanPlayerCharacterSpawn = true;
	SetSlotType(ESlotType::MovableSlot);
}

void ASlotBase::ApplyAuraToContainedUnit()
{
	TArray<AAuraBase*> AurasKeys;
	Auras.GetKeys(AurasKeys);

	for (int i = AurasKeys.Num() - 1; i >= 0; --i)
	{
		ACharacterBase* auraCharacter = *Auras.Find(AurasKeys[i]);

		ACharacterBase* outAllyCharacter;
		if (IsAllySlot(auraCharacter, outAllyCharacter))
		{
			AurasKeys[i]->AddBuffsToCharacter(outAllyCharacter);
			continue;
		}

		ACharacterBase* outEnemyCharacter;
		if (IsEnemySlot(auraCharacter, outEnemyCharacter))
		{
			AurasKeys[i]->AddDeBuffsToCharacter(outEnemyCharacter);
		}
	}
}

void ASlotBase::AddAura(AAuraBase* InAura, ACharacterBase* InAuraOwner)
{
	Auras.Add(InAura, InAuraOwner);

	InAuraOwner->OnCharacterRemovedFromSlot.AddDynamic(this, &ASlotBase::OnCharacterRemovedFromSlotHandler);
}

void ASlotBase::RemoveAura(ACharacterBase* InCharacter)
{
	TArray<AAuraBase*> AurasKeys;
	Auras.GetKeys(AurasKeys);

	for (int i = AurasKeys.Num() - 1; i >= 0; --i)
	{
		const ACharacterBase* auraCharacter = *Auras.Find(AurasKeys[i]);

		if (auraCharacter != InCharacter) { continue; }

		Auras.Remove(AurasKeys[i]);
	}
}

bool ASlotBase::IsEmpty()
{
	return ContainedUnit == nullptr;
}

bool ASlotBase::IsEnemySlot(ACharacterBase* InCharacter, ACharacterBase*& OutEnemyCharacter)
{
	if (ACharacterBase* CastContainedCharacter = Cast<ACharacterBase>(ContainedUnit))
	{
		if (!CastContainedCharacter->IsDead())
		{
			OutEnemyCharacter = CastContainedCharacter;
			return CastContainedCharacter->bIsAI != InCharacter->bIsAI;
		}
	}

	OutEnemyCharacter = nullptr;
	return false;
}

bool ASlotBase::IsAllySlot(ACharacterBase* InCharacter, ACharacterBase*& OutAllyCharacter)
{
	if (ACharacterBase* CastContainedCharacter = Cast<ACharacterBase>(ContainedUnit))
	{
		if (!CastContainedCharacter->IsDead())
		{
			OutAllyCharacter = CastContainedCharacter;
			return CastContainedCharacter->bIsAI == InCharacter->bIsAI;
		}
	}

	OutAllyCharacter = nullptr;
	return false;
}

bool ASlotBase::ContainsObstacle()
{
	AObstacleBase* ContainedObstacle = Cast<AObstacleBase>(ContainedUnit);
	return ContainedObstacle != nullptr;
}

FSlotSprites ASlotBase::GetSlotSpriteStruct(int32 InSize)
{
	if (InSize == 1) { return SlotSetting.SlotSprites1_1; }
	if (InSize == 2) { return SlotSetting.SlotSprites2_2; }
	if (InSize == 3) { return SlotSetting.SlotSprites3_3; }
	if (InSize == 4) { return SlotSetting.SlotSprites4_4; }
	if (InSize == 5) { return SlotSetting.SlotSprites5_5; }
	if (InSize == 6) { return SlotSetting.SlotSprites6_6; }
	if (InSize == 7) { return SlotSetting.SlotSprites7_7; }
	if (InSize == 8) { return SlotSetting.SlotSprites8_8; }
	return {};
}

void ASlotBase::ShowTotalCost()
{
	TotalCostText->SetText(UKismetTextLibrary::Conv_IntToText(TotalCost));
	TotalCostText->SetVisibility(true);
}

void ASlotBase::HideSlot()
{
	SetSlotType(ESlotType::NoSlot);
}

void ASlotBase::Movable(bool IsOn)
{
	bIsMovable = IsOn;
	SetSlotType(bIsMovable ? ESlotType::MovableSlot : ESlotType::DefaultSlot);
}

void ASlotBase::Highlight(bool IsOn, int32 InSize)
{
	if (IsOn)
		SetSlotType(ESlotType::ActiveSlot, InSize);
	else
		SetSlotType(PreviousSlotType);
}

void ASlotBase::SetEnemySlot(int32 InSize)
{
	SetSlotType(ESlotType::EnemySlot, InSize);
}

void ASlotBase::SetAllySlot(int32 InSize)
{
	SetSlotType(ESlotType::AllySlot, InSize);
}

void ASlotBase::SetSpellSlot()
{
	SetSlotType(ESlotType::SpellSlot);
}

void ASlotBase::ResetMovementData(bool IsOverlayData)
{
	TotalCost = 0;
	Parent = nullptr;

	if (IsOverlayData)
		OverlayPath.Empty();
	else
		Path.Empty();
}

void ASlotBase::SaveSlotType()
{
	PreviousSlotType = SlotType;
}

void ASlotBase::AddSlotToCanAttackFromSlots(ASlotBase* InSlot)
{
	CanAttackFromSlots.AddUnique(InSlot);
}

void ASlotBase::ClearCanAttackFromSlots()
{
	CanAttackFromSlots.Empty();
}

void ASlotBase::OnSpellDisabled()
{
}

void ASlotBase::ClearAllCallbacks()
{
}

void ASlotBase::OnCharacterRemovedFromSlotHandler(ACharacterBase* InCharacter)
{
	InCharacter->OnCharacterRemovedFromSlot.RemoveDynamic(this, &ASlotBase::OnCharacterRemovedFromSlotHandler);
	RemoveAura(InCharacter);
}
