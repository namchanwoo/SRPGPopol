#include "Grid.h"

#include "EnemySpawnLocation.h"
#include "HeroSpawnLocation.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "SRG/Core/BattleController.h"

#include "Slots/GridOverlaySlot.h"
#include "Slots/GridInnerOverlaySlot.h"
#include "Slots/InnerSlot.h"
#include "Slots/Slot.h"
#include "Slots/SlotBase.h"
#include "SRG/Abilities/ActiveAbilities/AreaTargetActiveAbilityBase.h"

#include "SRG/Gird/Slots/SlotBase.h"
#include "SRG/Spells/SpellBase.h"
#include "SRG/Spells/AreaTargetSpells/AreaTargetSpellBase.h"
#include "SRG/Units/Characters/CharacterBase.h"
#include "SRG/Units/Characters/RangeCharacters/RangeCharacterBase.h"
#include "SRG/Units/Obstacles/ObstacleBase.h"

#include "SRG/Widgets/UW_DamageDetailsListUI.h"

#include "SRGCore/Libraries/SRPGTypes.h"
#include "SRGCore/Libraries/SRPGUtilityLibrary.h"
#include "SRGCore/Utilities/SRGLog.h"

AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
	PathSpline->EditorUnselectedSplineSegmentColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
	PathSpline->EditorSelectedSplineSegmentColor = FLinearColor(0.728f, 0.364f, 0.003f, 1.0f);
	PathSpline->EditorTangentColor = FLinearColor(1.0f, 0.030547f, 0.0f, 1.0f);
	PathSpline->SetupAttachment(DefaultSceneRoot);

	PathMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PathMesh"));
	PathMesh->SetGenerateOverlapEvents(false);
	PathMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PathMesh->SetCollisionProfileName(TEXT("Custom"));
	PathMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Set the static mesh for the component
	// 블루프린트에서 설정했음 귀찮아서
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/TurnBasedStrategyRPGTemplate/Meshes/SM_TinySphere.SM_TinySphere"));
	// if (MeshAsset.Succeeded())
	// {
	// 	PathMesh->SetStaticMesh(MeshAsset.Object);
	// }
	PathMesh->SetCastShadow(false);
	PathMesh->SetupAttachment(DefaultSceneRoot);

	SlotClass = ASlot::StaticClass();
	OverlaySlotClass = AGridOverlaySlot::StaticClass();
	InnerSlotClass = AInnerSlot::StaticClass();
	InnerOverlaySlotClass = AGridInnerOverlaySlot::StaticClass();
}

void AGrid::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineEvent DetectEvent;
	DetectEvent.BindUFunction(this, FName("DetectClosestMovableSlotTimeline_Detect"));
	DetectClosestMovableSlotTimeline.AddEvent(0.0f, DetectEvent);
	DetectClosestMovableSlotTimeline.SetTimelineLength(0.1f);
	DetectClosestMovableSlotTimeline.SetLooping(true);
}

void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DetectClosestMovableSlotTimeline.TickTimeline(DeltaTime);
}

void AGrid::InitializeEvent_Implementation()
{
	// 레벨에 BP_BattleController 추가되었는지 확인
	SRPG_IF_CHECK(BattleControllerRef, TEXT("레벨에 BP_BattleController 추가해주세요"));

	// OnBattleStateChanged 이벤트에 대한 디나믹 바인딩 추가
	BattleControllerRef->OnBattleStateChanged.AddDynamic(this, &AGrid::OnBattleStateChangedHandler);
}

void AGrid::ReCreateSlots()
{
	SRPG_IF_CHECK(SlotClass, TEXT("SlotClass가 설정되지 않았습니다. 설정해주세요."));
	SRPG_IF_CHECK(OverlaySlotClass, TEXT("OverlaySlotClass 설정되지 않았습니다. 설정해주세요."));
	SRPG_IF_CHECK(InnerSlotClass, TEXT("InnerSlotClass가 설정되지 않았습니다. 설정해주세요."));
	SRPG_IF_CHECK(InnerOverlaySlotClass, TEXT("InnerOverlaySlotClass 설정되지 않았습니다. 설정해주세요."));

	UWorld* World = GetWorld();
	SRPG_CHECK(World);

	RemoveAllSlots();

	CreateOuterSlots(World);
	CreateInnerSlots(World);
}

void AGrid::RemoveAllSlots()
{
	// 슬롯 제거
	DestroySlots(Slots, true);

	// 오버레이 슬롯 제거
	DestroySlots(OverlaySlots, false);

	// 이너 슬롯 제거
	DestroySlots(InnerSlots, true);

	// 이너 오버레이 슬롯 제거
	DestroySlots(InnerOverlaySlots, false);

	// 히어로 스폰 지점 제거
	if (HeroSpawnLocation)
	{
		HeroSpawnLocation->Destroy();
		HeroSpawnLocation = nullptr;
	}

	// 이 그리드에 연결된 모든 액터 제거
	TArray<AActor*> OutActors;
	GetAttachedActors(OutActors);
	for (int i = OutActors.Num() - 1; i >= 0; --i)
	{
		OutActors[i]->Destroy();
	}
}

void AGrid::DestroySlots(TMap<FVector2D, ASlotBase*>& SlotMap, bool bClearCallbacks)
{
	TArray<ASlotBase*> SlotValues;
	SlotMap.GenerateValueArray(SlotValues);
	for (int i = SlotValues.Num() - 1; i >= 0; --i)
	{
		if (bClearCallbacks)
		{
			SlotValues[i]->ClearAllCallbacks();
		}
		SlotValues[i]->Destroy();
	}
	SlotMap.Empty();
}

void AGrid::Refresh()
{
	// 모든 슬롯을 순회하면서 슬롯을 새로 고침
	for (TTuple<UE::Math::TVector2<double>, ASlotBase*> L_SlotPair : Slots)
	{
		ASlotBase* L_SlotBase = L_SlotPair.Value;

		// 슬롯의 좌표가 PlayerSpawnColumn보다 작은 경우
		if (L_SlotBase->Coordinates.X < PlayerSpawnColumn)
		{
			// 슬롯을 플레이어 스폰 슬롯으로 설정
			L_SlotBase->SetPlayerSpawnSlot();
		}
		else
		{
			// 슬롯이 비어있는 경우 DefaultSlot으로, 그렇지 않은 경우 NoSlot으로 슬롯 타입 설정
			const ESlotType L_NewSlotType = L_SlotBase->IsEmpty() ? ESlotType::DefaultSlot : ESlotType::NoSlot;
			L_SlotBase->SetSlotType(L_NewSlotType, 1);
		}
	}
}

void AGrid::CreateOuterSlots(UWorld* World)
{
	// 그리드의 행과 열을 반복합니다.
	for (int32 Row = 0; Row < NumberOfRows; ++Row)
	{
		for (int32 Column = 0; Column < NumberOfColumns; ++Column)
		{
			// 위치에 따라 슬롯의 변환을 계산합니다.
			FTransform SlotTransform = CalculateSlotTransform(Row, Column);

			// 슬롯이 그리드의 왼쪽 하단 모서리에 있는 경우 영웅 스폰 지점으로 설정합니다.
			if (Column == 0 && (Row == (NumberOfRows - 1)))
			{
				SetHeroSpawnPoint(SlotTransform);
			}

			// 이 위치에서 새 슬롯을 만들고 초기화합니다.
			ASlot* NewSlot = CreateAndInitializeSlot(World, SlotTransform, Column, Row);

			// 그리드의 슬롯 맵에 새 슬롯을 추가합니다.
			Slots.Add(FVector2D(Column, Row), NewSlot);

			// 이 위치에서 새 오버레이 슬롯을 만들고 초기화합니다.
			AGridOverlaySlot* NewOverlaySlot = CreateAndInitializeOverlaySlot(World, SlotTransform, Column, Row);

			// 그리드의 오버레이 슬롯 맵에 새 오버레이 슬롯을 추가합니다.
			OverlaySlots.Add(FVector2D(Column, Row), NewOverlaySlot);
		}
	}
}

void AGrid::CreateInnerSlots(UWorld* World)
{
	// 마지막 행과 열을 제외하고 그리드의 행과 열을 반복합니다.
	for (int32 Row = 0; Row < NumberOfRows - 1; ++Row)
	{
		for (int32 Column = 0; Column < NumberOfColumns - 1; ++Column)
		{
			// 위치에 따라 내부 슬롯의 변환을 계산합니다.
			FTransform InnerSlotTransform = CalculateInnerSlotTransform(Row, Column);

			// 이 위치에서 새 내부 슬롯을 생성하고 초기화합니다.
			AInnerSlot* NewInnerSlot = CreateAndInitializeInnerSlot(World, InnerSlotTransform, Column, Row);

			// 그리드의 내부 슬롯 맵에 새 내부 슬롯을 추가합니다.
			InnerSlots.Add(FVector2D(Column, Row), NewInnerSlot);

			// 이 위치에서 새 내부 오버레이 슬롯을 만들고 초기화합니다.
			AGridInnerOverlaySlot* NewInnerOverlaySlot = CreateAndInitializeInnerOverlaySlot(
				World, InnerSlotTransform, Column, Row);

			// 그리드의 내부 오버레이 슬롯 맵에 새 내부 오버레이 슬롯을 추가합니다.
			InnerOverlaySlots.Add(FVector2D(Column, Row), NewInnerOverlaySlot);
		}
	}
}

FTransform AGrid::CalculateSlotTransform(int32 Row, int32 Column)
{
	// 슬롯 크기 가져오기
	const ASlot* L_SlotCDO = SlotClass->GetDefaultObject<ASlot>();

	// 행, 열 및 슬롯 크기를 기반으로 스폰 위치 계산
	const FVector L_SpawnLocation = FVector(Column * (L_SlotCDO->SlotSetting.SlotSize.X * 2.0f),
	                                        Row * (L_SlotCDO->SlotSetting.SlotSize.Y * -2.0f),
	                                        0.0f);
	// 슬롯의 변환을 반환합니다.
	return FTransform(FRotator::ZeroRotator, L_SpawnLocation + GetActorTransform().GetLocation());
}

FTransform AGrid::CalculateInnerSlotTransform(int32 Row, int32 Column)
{
	// 슬롯의 기본 속성을 가져옵니다.
	const ASlot* SlotCDO = SlotClass->GetDefaultObject<ASlot>();

	// 슬롯의 위치와 크기를 기반으로 내부 슬롯의 스폰 위치를 계산합니다.
	FVector L_SpawnLocation = FVector(Column * (SlotCDO->SlotSetting.SlotSize.X * 2.0f),
	                                  Row * (SlotCDO->SlotSetting.SlotSize.Y * -2.0f),
	                                  0.0f);

	// 이것이 내부 슬롯이라는 사실을 고려하여 스폰 위치를 조정합니다.
	L_SpawnLocation += FVector(SlotCDO->SlotSetting.SlotSize.X, SlotCDO->SlotSetting.SlotSize.Y * -1.0f, 0.0f);

	// 스폰 위치를 그리드의 현재 위치와 결합하여 내부 슬롯에 대한 최종 변형을 얻습니다.
	return FTransform(FRotator::ZeroRotator, L_SpawnLocation + GetActorTransform().GetLocation());
}

ASlot* AGrid::CreateAndInitializeSlot(UWorld* World, const FTransform& SlotTransform, int32 Column, int32 Row)
{
	// 새 슬롯 생성
	ASlot* L_NewSlot = World->SpawnActor<ASlot>(SlotClass, SlotTransform, FActorSpawnParameters());

	// 새 슬롯의 속성 설정
	L_NewSlot->Coordinates = FVector2D(Column, Row);
	L_NewSlot->Grid = this;
	L_NewSlot->InitializeEvent(L_NewSlot->SlotSetting);

	// 슬롯의 이벤트를 그리드의 메서드에 바인딩
	L_NewSlot->OnMouseOverSlot.AddDynamic(this, &AGrid::OnMouseOverSlot);
	L_NewSlot->OnSlotClicked.AddDynamic(this, &AGrid::OnSlotClicked);
	L_NewSlot->OnMouseLeaveSlot.AddDynamic(this, &AGrid::OnMouseLeaveSlot);

	if (L_NewSlot->Coordinates.X < PlayerSpawnColumn)
	{
		L_NewSlot->SetPlayerSpawnSlot();
	}

	// 그리드 액터에 새 슬롯을 연결합니다.
	L_NewSlot->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

	return L_NewSlot;
}

AGridOverlaySlot* AGrid::CreateAndInitializeOverlaySlot(UWorld* World, FTransform& SlotTransform, int32 Column,
                                                        int32 Row)
{
	// 새 오버레이 슬롯 생성
	AGridOverlaySlot* L_NewSlot = World->SpawnActor<AGridOverlaySlot>(OverlaySlotClass, SlotTransform,
	                                                                  FActorSpawnParameters());

	// 새 오버레이 슬롯의 속성 설정
	L_NewSlot->Coordinates = FVector2D(Column, Row);
	L_NewSlot->Grid = this;
	L_NewSlot->InitializeEvent(L_NewSlot->SlotSetting);

	// 새 오버레이 슬롯을 그리드 액터에 연결합니다.
	L_NewSlot->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

	return L_NewSlot;
}

AInnerSlot* AGrid::CreateAndInitializeInnerSlot(UWorld* World, FTransform& InnerSlotTransform, int32 Column, int32 Row)
{
	// 월드에 새로운 내부 슬롯 액터를 스폰합니다.
	AInnerSlot* NewInnerSlot = World->SpawnActor<AInnerSlot>(InnerSlotClass, InnerSlotTransform,
	                                                         FActorSpawnParameters());

	// 새 내부 슬롯의 좌표와 그리드 참조를 설정합니다.
	NewInnerSlot->Coordinates = FVector2D(Column, Row);
	NewInnerSlot->Grid = this;

	// 새 내부 슬롯을 초기화합니다.
	NewInnerSlot->InitializeEvent(NewInnerSlot->SlotSetting);

	// 새 내부 슬롯에 그리드의 마우스 오버 이벤트를 등록합니다.
	NewInnerSlot->OnMouseOverSlot.AddDynamic(this, &AGrid::OnMouseOverSlot);

	// 새 내부 슬롯을 그리드 액터에 연결합니다.
	NewInnerSlot->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

	return NewInnerSlot;
}

AGridInnerOverlaySlot* AGrid::CreateAndInitializeInnerOverlaySlot(UWorld* World, FTransform& InnerSlotTransform,
                                                                  int32 Column, int32 Row)
{
	// 월드에 새로운 내부 오버레이 슬롯 액터를 생성합니다.
	AGridInnerOverlaySlot* NewInnerOverlaySlot = World->SpawnActor<AGridInnerOverlaySlot>(
		InnerOverlaySlotClass, InnerSlotTransform, FActorSpawnParameters());

	// 새 내부 오버레이 슬롯의 좌표와 그리드 참조를 설정합니다.
	NewInnerOverlaySlot->Coordinates = FVector2D(Column, Row);
	NewInnerOverlaySlot->Grid = this;

	// 새 내부 오버레이 슬롯을 초기화합니다.
	NewInnerOverlaySlot->InitializeEvent(NewInnerOverlaySlot->SlotSetting);

	// 새 내부 오버레이 슬롯을 그리드 액터에 연결합니다.
	NewInnerOverlaySlot->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

	return NewInnerOverlaySlot;
}

void AGrid::OnBattleStateChangedHandler(EBattleState NewBattleState)
{
	// 새로운 전투 상태가 PlayerIsCastingSpell 또는 PlayerIsPlaying인 경우
	if (NewBattleState == EBattleState::PlayerIsCastingSpell || NewBattleState == EBattleState::PlayerIsPlaying)
	{
		// 오버레이 데이터를 초기화
		ResetOverlayData();

		// 캐릭터 움직임 오버레이 슬롯을 비움
		CharacterMovementOverlaySlots.Empty();

		// 데미지 상세 UI를 숨김
		HideDamageDetailsUI();

		// OnCursorChanged가 바인딩되어 있는 경우, Wait 커서 상태를 브로드캐스트
		if (OnCursorChanged.IsBound())
		{
			OnCursorChanged.Broadcast(EBattleCursorType::Wait, nullptr, nullptr, nullptr);
		}
	}
}

void AGrid::OnMovableSlotTimeline_Detect()
{
	// 마우스와 가장 가까운 슬롯을 찾습니다.
	ASlotBase* L_ClosestSlotToMouse =
		GetClosestSlotToMouseImproved(m_CurrentMouseOverSlot, m_CurrentMouseOverSlot->CanAttackFromSlots);

	// 마우스 아래의 현재 슬롯이 가장 가까운 슬롯과 동일하면 함수를 종료합니다.
	if (CurrentSlotUnderMouse == L_ClosestSlotToMouse) { return; }

	ResetCurrentSlotUnderMouse();
	CurrentSlotUnderMouse = L_ClosestSlotToMouse;

	// 현재 슬롯이 유효하지 않으면 함수를 종료합니다.
	if (!IsValid(CurrentSlotUnderMouse)) { return; }

	ShowDamageDetailsUI(m_CurrentMouseOverSlot, CurrentSlotUnderMouse);

	if (bIsHexGrid)
	{
		// 커서 변경 이벤트가 바인딩되어 있으면 이를 브로드캐스트합니다.
		if (OnCursorChanged.IsBound())
		{
			OnCursorChanged.Broadcast(EBattleCursorType::MeleeAttack, BattleControllerRef->CurrentActiveAbility,
			                          nullptr, nullptr);
		}

		bool L_AllFree;
		TArray<ASlotBase*> L_OutSlotsInRange;
		GetSlotsInRange(CurrentSlotUnderMouse, BattleControllerRef->PlayingCharacter->UnitSize,
		                false, L_OutSlotsInRange, L_AllFree);
		const ASlotBase* L_ClosestSlot = GetClosestSlotInRange(m_CurrentMouseOverSlot, L_OutSlotsInRange);

		// 커서 각도 변경 이벤트가 바인딩되어 있으면 이를 브로드캐스트합니다.
		if (OnCursorAngleChanged.IsBound())
		{
			const FRotator L_NewRotation = UKismetMathLibrary::FindLookAtRotation(
				L_ClosestSlot->GetActorLocation(), m_CurrentMouseOverSlot->GetActorLocation());

			const TObjectPtr<APlayerCameraManager> L_PlayerCameraManager =
				UGameplayStatics::GetPlayerController(this, 0)->PlayerCameraManager;

			const float L_NewAngle = L_NewRotation.Yaw - (L_PlayerCameraManager->GetCameraRotation().Yaw + 90.0f);
			OnCursorAngleChanged.Broadcast(L_NewAngle);
		}
	}
	else // bIsHexGrid==false
	{
		// 커서 변경 이벤트가 바인딩되어 있으면 이를 브로드캐스트합니다.
		if (OnCursorChanged.IsBound())
		{
			OnCursorChanged.Broadcast(EBattleCursorType::MeleeAttack, BattleControllerRef->CurrentActiveAbility,
			                          nullptr, nullptr);
		}

		bool L_AllFree;
		TArray<ASlotBase*> L_OutSlotsInRange;
		GetSlotsInRange(m_CurrentMouseOverSlot->ContainedUnit->Slot,
		                m_CurrentMouseOverSlot->ContainedUnit->UnitSize, false,
		                L_OutSlotsInRange, L_AllFree);

		const auto L_ClosestSlot = GetClosestSlotInRange(CurrentSlotUnderMouse, L_OutSlotsInRange);

		// 커서 각도 변경 이벤트가 바인딩되어 있으면 이를 브로드캐스트합니다.
		if (OnCursorAngleChanged.IsBound())
		{
			const float NewYaw = UKismetMathLibrary::FindLookAtRotation(CurrentSlotUnderMouse->GetActorLocation(),
			                                                            L_ClosestSlot->GetActorLocation()).Yaw;

			const TObjectPtr<APlayerCameraManager> L_PlayerCameraManager =
				UGameplayStatics::GetPlayerController(this, 0)->PlayerCameraManager;

			const float NewAngle = NewYaw - (L_PlayerCameraManager->GetCameraRotation().Yaw + 90.0f);
			OnCursorAngleChanged.Broadcast(NewAngle);
		}
	}

	// 플레이어 캐릭터의 슬롯이 현재 마우스 아래의 슬롯과 동일하다면 함수를 종료합니다.
	if (BattleControllerRef->PlayingCharacter->Slot == CurrentSlotUnderMouse)
	{
		return;
	}

	ShowMovementPath(BattleControllerRef->PlayingCharacter, CurrentSlotUnderMouse);
	HighlightSlot(CurrentSlotUnderMouse, BattleControllerRef->PlayingCharacter->UnitSize, true);
}

void AGrid::ResetCurrentSlotUnderMouse()
{
	// 전투 컨트롤러의 전투 상태가 배치 단계인지 확인합니다.
	if (BattleControllerRef->BattleState == EBattleState::DeploymentPhase)
	{
		// 마우스가 그리드 밖으로 나갔을 때 이벤트가 바인딩되어 있는지 확인하고, 있다면 브로드캐스트합니다.
		if (OnMouseOutOfGrid.IsBound())
		{
			OnMouseOutOfGrid.Broadcast();
		}
		return;
	}

	// 전투 컨트롤러의 전투 상태에 따라 새로운 커서 타입을 설정합니다.
	const EBattleCursorType L_NewCursorType = BattleControllerRef->BattleState == EBattleState::WaitingForPlayerAction
		                                          ? EBattleCursorType::Default
		                                          : EBattleCursorType::Wait;

	// 커서 변경 이벤트가 바인딩되어 있는지 확인하고, 있다면 브로드캐스트합니다.
	if (OnCursorChanged.IsBound())
	{
		OnCursorChanged.Broadcast(L_NewCursorType, nullptr, nullptr, nullptr);
	}

	// 현재 마우스 아래의 슬롯이 유효한지 확인하고, 그렇지 않다면 함수를 종료합니다.
	if (!IsValid(CurrentSlotUnderMouse))
	{
		return;
	}

	// 현재 마우스 아래의 슬롯이 플레이어 캐릭터의 슬롯이 아닌지 확인합니다.
	if (CurrentSlotUnderMouse != BattleControllerRef->PlayingCharacter->Slot)
	{
		// 현재 슬롯을 강조 표시 해제하고 이동 경로를 지웁니다.
		HighlightSlot(CurrentSlotUnderMouse, BattleControllerRef->PlayingCharacter->UnitSize, false);
		ClearMovementPath();
	}

	// 현재 마우스 아래의 슬롯을 null로 설정합니다.
	CurrentSlotUnderMouse = nullptr;
}

void AGrid::CheckSlotUnderMouse()
{
	// 현재 마우스 아래의 슬롯을 리셋합니다.
	ResetCurrentSlotUnderMouse();

	// 플레이어 컨트롤러를 가져옵니다.
	const APlayerController* L_PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// 커서 아래의 히트 결과를 저장할 변수를 생성합니다.
	FHitResult L_UnderCursorHitResult;

	// 시각적 트레이스 타입을 설정합니다.
	const ETraceTypeQuery ETQ_Visibility = USRPGTypes::ConvertToTraceTypeVisibility();

	// 커서 아래의 히트 결과를 가져옵니다.
	if (L_PlayerController->GetHitResultUnderCursorByChannel(ETQ_Visibility, false, L_UnderCursorHitResult))
	{
		// 히트 결과의 액터를 슬롯 베이스로 변환하고 결과를 저장합니다.
		if (ASlotBase* L_CursorHitSlot = Cast<ASlotBase>(L_UnderCursorHitResult.GetActor()))
		{
			// 마우스가 슬롯 위에 있음을 통지합니다.
			OnMouseOverSlot(L_CursorHitSlot, false);
		}
	}
}

void AGrid::ResetSlots()
{
	// 모든 슬롯을 반복하면서 슬롯을 리셋합니다.
	for (const TTuple<UE::Math::TVector2<double>, ASlotBase*> L_SlotPair : Slots)
	{
		// 장애물이 있는지 확인하고 슬롯 타입을 결정합니다.
		const ESlotType L_NewSlotType = L_SlotPair.Value->ContainsObstacle()
			                                ? ESlotType::NoSlot
			                                : ESlotType::DefaultSlot;

		// 슬롯 타입을 설정합니다.
		L_SlotPair.Value->SetSlotType(L_NewSlotType, 1);
	}

	// 모든 내부 슬롯을 반복하면서 슬롯을 숨깁니다.
	for (const TTuple<UE::Math::TVector2<double>, ASlotBase*> L_InnerSlotPair : InnerSlots)
	{
		L_InnerSlotPair.Value->HideSlot();
	}
}

void AGrid::SetHeroSpawnPoint(const FTransform& InSlotTransform)
{
	// 슬롯 클래스가 유효한지 확인합니다.
	SRPG_CHECK(SlotClass);

	// 현재 월드를 가져옵니다.
	UWorld* L_World = GetWorld();
	SRPG_CHECK(L_World);

	// HeroSpawnLocationClass가 유효한지 확인합니다.
	SRPG_CHECK(HeroSpawnLocationClass);

	// 슬롯 클래스의 기본 객체를 가져옵니다.
	const auto L_SlotCDO = SlotClass->GetDefaultObject<ASlotBase>();

	// 새로운 위치를 계산합니다. 슬롯 사이즈에 따라 X 위치를 조정합니다.
	const FVector L_NewLocation = InSlotTransform.GetLocation() -
		FVector(L_SlotCDO->SlotSetting.SlotSize.X + 100.0f, 0.0f, 0.0f);

	// 액터 생성 파라미터를 정의합니다.
	const FActorSpawnParameters L_SpawnParams;

	// 스폰 위치를 정의합니다.
	const FTransform L_SpawnTransform = FTransform(FRotator::ZeroRotator, L_NewLocation);

	// 히어로 스폰 위치 액터를 월드에 생성합니다.
	HeroSpawnLocation = L_World->SpawnActor<
		AHeroSpawnLocation>(HeroSpawnLocationClass, L_SpawnTransform, L_SpawnParams);

	// 생성된 히어로 스폰 위치를 이 액터에 부착합니다.
	HeroSpawnLocation->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
}

void AGrid::OnMouseOverSlot(ASlotBase* InSlot, bool bIsInnerSlot)
{
	// 현재 마우스가 위치한 슬롯을 할당합니다.
	m_CurrentMouseOverSlot = InSlot;

	// 현재 전투 상태에 따라 다른 메서드가 호출됩니다.
	switch (BattleControllerRef->BattleState)
	{
	case EBattleState::DeploymentPhase:
		HandleDeploymentPhase(InSlot);
		break;

	case EBattleState::WaitingForPlayerAction:
		HandleWaitingForPlayerActionPhase(InSlot, bIsInnerSlot);
		break;

	// 그 외 필요한 경우를 처리합니다.	
	default: break;
	}
}

void AGrid::OnSlotClicked(ASlotBase* InSlot, FKey InButton)
{
	// 마우스 오른쪽 버튼 클릭을 처리합니다.
	if (InButton == EKeys::RightMouseButton)
	{
		if (ACharacterBase* CastCharacterUnit = Cast<ACharacterBase>(InSlot->ContainedUnit))
		{
			BattleControllerRef->ShowCharacterDetails(CastCharacterUnit);
		}
		return;
	}

	// 왼쪽 마우스 버튼 클릭을 처리합니다.
	if (InButton == EKeys::LeftMouseButton)
	{
		if (BattleControllerRef->BattleState == EBattleState::DeploymentPhase)
		{
			if (SlotClicked.IsBound())
			{
				SlotClicked.Broadcast(InSlot);
			}
			return;
		}
		if (BattleControllerRef->BattleState == EBattleState::WaitingForPlayerAction)
		{
			HandleBattleClick(InSlot);
			return;
		}
	}
}

void AGrid::HandleBattleClick(ASlotBase* InSlot)
{
	StopDetectingClosestMovableSlot();

	// 슬롯이 유효하고 영역 대상 능력이 활성화되어 있으면 능력을 활성화합니다.
	if (IsValid(InSlot) && bAreaTargetAbilityEnabled && !InSlot->ContainsObstacle())
	{
		ActivateAbility(InSlot);
		return;
	}

	// 동맹 대상 능력이 활성화되고 슬롯이 동맹 슬롯인 경우 능력을 활성화합니다.
	ACharacterBase* OutAllyCharacter;
	if (bAllyTargetAbilityEnabled && AllySlots.Contains(InSlot) &&
		InSlot->IsAllySlot(BattleControllerRef->PlayingCharacter, OutAllyCharacter))
	{
		ActivateAbility(InSlot);
		return;
	}

	// BattleController가 유효하면 슬롯에 주문을 시전합니다.
	if (IsValid(BattleControllerRef))
	{
		CastSpellOnSlot(InSlot);
		return;
	}

	// 공격 유형에 따라 적의 공격이나 움직임의 논리를 처리합니다.
	HandleEnemyAttackOrMovement(InSlot);
}

void AGrid::ActivateAbility(ASlotBase* InSlot)
{
	BattleControllerRef->ActiveAbility(InSlot);
	ResetSlots();
	CurrentSlotUnderMouse = nullptr;
	BattleControllerRef->ChangeBattleState(EBattleState::PlayerIsPlaying);
}

void AGrid::HandleEnemyAttackOrMovement(ASlotBase* InSlot)
{
	ACharacterBase* OutEnemyCharacter;
	const bool IsEnemySlot = EnemySlots.Contains(InSlot) &&
		InSlot->IsEnemySlot(BattleControllerRef->PlayingCharacter, OutEnemyCharacter);

	// 근접 공격이 아니고 슬롯이 적 슬롯이면 범위 공격을 수행합니다.
	if (!bIsMeleeAttack && IsEnemySlot)
	{
		RangeAttack(InSlot);
		return;
	}

	// 마우스 아래의 현재 슬롯이 유효한 경우 근접 공격 또는 이동을 처리합니다.
	if (IsValid(CurrentSlotUnderMouse))
	{
		HandleMeleeAttackOrMovement(InSlot, IsEnemySlot);
	}
}

void AGrid::RangeAttack(ASlotBase* InSlot)
{
	BattleControllerRef->RangeAttack(InSlot);
	ResetSlots();
	CurrentSlotUnderMouse = nullptr;
	BattleControllerRef->ChangeBattleState(EBattleState::PlayerIsPlaying);
}

void AGrid::HandleMeleeAttackOrMovement(ASlotBase* InSlot, bool IsEnemySlot)
{
	if (IsEnemySlot)
	{
		MeleeAttackOrMoveEnemy(InSlot);
	}
	else
	{
		MoveAction(InSlot);
	}
}

void AGrid::MeleeAttackOrMoveEnemy(ASlotBase* InSlot)
{
	if (!Slots.Contains(CurrentSlotUnderMouse->Coordinates)) { return; }

	if (*Slots.Find(CurrentSlotUnderMouse->Coordinates) == BattleControllerRef->PlayingCharacter->Slot)
	{
		BattleControllerRef->MeleeAttackAction(InSlot);
		FinishAction();
	}
	else
	{
		BattleControllerRef->MoveAction(CurrentSlotUnderMouse, InSlot);
		FinishAction();
	}
}

void AGrid::MoveAction(ASlotBase* InSlot)
{
	if (!Slots.Contains(CurrentSlotUnderMouse->Coordinates)) { return; }
	if (!MovableSlots.Contains(*Slots.Find(CurrentSlotUnderMouse->Coordinates))) { return; }
	BattleControllerRef->MoveAction(CurrentSlotUnderMouse, nullptr);
	FinishAction();
}

void AGrid::FinishAction()
{
	ResetSlots();
	CurrentSlotUnderMouse = nullptr;
	BattleControllerRef->ChangeBattleState(EBattleState::PlayerIsPlaying);
}

void AGrid::HandleDeploymentPhase(ASlotBase* InSlot)
{
	// MouseOverSlot이 연결되어 있다면 InSlot을 브로드캐스트합니다.
	if (MouseOverSlot.IsBound())
	{
		MouseOverSlot.Broadcast(InSlot);
	}
}

void AGrid::HandleWaitingForPlayerActionPhase(ASlotBase* InSlot, bool L_bIsInnerSlot)
{
	// 현재 마우스 아래의 슬롯을 재설정하고 가장 가까운 이동 가능한 슬롯 감지를 중단합니다.
	StopDetectingClosestMovableSlot();
	ResetCurrentSlotUnderMouse();

	// 영역 대상 능력을 처리합니다.
	if (bAreaTargetAbilityEnabled)
	{
		ShowAreaTargetAbilitySlots(InSlot);
		return;
	}

	// 현재 주문이 유효하다면 주문 슬롯을 표시합니다.
	if (IsValid(BattleControllerRef->CurrentSpell))
	{
		ShowSpellSlots(InSlot);
		return;
	}


	// 아군 대상 능력이 활성화되어 있고 슬롯에 아군이 있다면 아군 대상 능력을 처리합니다.
	ACharacterBase* L_OutAllyCharacter;
	if (InSlot->IsAllySlot(BattleControllerRef->PlayingCharacter, L_OutAllyCharacter) &&
		bAllyTargetAbilityEnabled && AllySlots.Contains(InSlot))
	{
		HandleAllyTargetAbility(InSlot);
		return;
	}


	// InSlot이 SlotsInRange 내에 있다면 범위 내의 슬롯을 처리합니다.
	if (SlotsInRange.Contains(InSlot))
	{
		HandleSlotInRange(InSlot);
		return;
	}


	// 아군 대상 능력이 활성화되어 있지 않고 현재 유효한 주문이 없다면 적 대상 능력을 처리합니다.
	ACharacterBase* L_OutEnemyCharacter;
	if (InSlot->IsEnemySlot(BattleControllerRef->PlayingCharacter, L_OutEnemyCharacter) &&
		(!bAllyTargetAbilityEnabled && !IsValid(BattleControllerRef->CurrentSpell) &&
			EnemySlots.Contains(InSlot)))
	{
		HandleEnemyTargetAbility(InSlot, L_bIsInnerSlot);
	}
	else
	{
		HideDamageDetailsUI();
	}
}

void AGrid::HandleAllyTargetAbility(ASlotBase* InSlot)
{
	RemoveHighlightOnCharacters();
	if (OnCursorChanged.IsBound())
	{
		OnCursorChanged.Broadcast(EBattleCursorType::Spell, BattleControllerRef->CurrentActiveAbility,
		                          nullptr, nullptr);
	}

	// 현재 활성화된 능력의 대상 슬롯을 설정합니다.
	BattleControllerRef->CurrentActiveAbility->SetTargetSlot(InSlot);

	// 능력 슬롯을 강조 표시합니다.
	HighlightAbilitySlots();
}

void AGrid::HandleSlotInRange(ASlotBase* InSlot)
{
	// 데미지 세부 정보 UI를 숨깁니다.
	HideDamageDetailsUI();

	// BattleControllerRef가 유효하지 않다면 함수를 종료합니다.
	if (!IsValid(BattleControllerRef)) { return; }

	// 가장 가까운 이동 가능한 슬롯을 가져옵니다.
	ASlotBase* L_ClosestMovableSlot = GetClosestMovableSlot(
		InSlot, BattleControllerRef->PlayingCharacter->UnitSize, MovableSlots);

	// 가장 가까운 이동 가능한 슬롯이 유효하지 않다면 함수를 종료합니다.
	if (!IsValid(L_ClosestMovableSlot)) { return; }

	// 현재 마우스 아래의 슬롯을 업데이트합니다.
	CurrentSlotUnderMouse = L_ClosestMovableSlot;

	// 이동 경로를 표시합니다.
	ShowMovementPath(BattleControllerRef->PlayingCharacter, CurrentSlotUnderMouse);

	// 해당 슬롯을 강조 표시합니다.
	HighlightSlot(CurrentSlotUnderMouse, BattleControllerRef->PlayingCharacter->UnitSize, true);

	// 커서 타입을 업데이트합니다.
	UpdateCursorType();
}

void AGrid::HandleEnemyTargetAbility(ASlotBase* InSlot, bool L_bIsInnerSlot)
{
	if (bIsMeleeAttack)
	{
		DetectClosestMovableSlotTimeline.PlayFromStart();
		return;
	}

	// 범위 내 공격 유형을 결정합니다.
	DetermineRangeAttackType(InSlot);

	// 데미지 세부 정보 UI를 표시합니다.
	ShowDamageDetailsUI(InSlot, InSlot);
}

void AGrid::HighlightAbilitySlots()
{
	// 범위 내의 슬롯, 빈 슬롯, 적 캐릭터를 담을 변수들입니다.
	TArray<ASlotBase*> L_OutSlotsInRange, L_OutEmptySlotsInRange;
	TArray<ACharacterBase*> L_OutEnemiesInRange;

	// 현재 활성화된 능력의 슬롯을 가져옵니다.
	BattleControllerRef->CurrentActiveAbility->GetAbilitySlots(L_OutSlotsInRange, L_OutEmptySlotsInRange,
	                                                           HighlightedCharacters, L_OutEnemiesInRange);

	// 강조 표시할 캐릭터들을 강조 표시합니다.
	for (ACharacterBase* InHighlightedCharacter : HighlightedCharacters)
	{
		InHighlightedCharacter->Highlight(true);
	}
}

void AGrid::UpdateCursorType()
{
	if (OnCursorChanged.IsBound())
	{
		EBattleCursorType SelectCursorType = {};
		if (BattleControllerRef->PlayingCharacter->MovementType == EMovementType::Walk)
			SelectCursorType = EBattleCursorType::Walk;
		else if (BattleControllerRef->PlayingCharacter->MovementType == EMovementType::Fly)
			SelectCursorType = EBattleCursorType::Fly;
		else if (BattleControllerRef->PlayingCharacter->MovementType == EMovementType::Immobilie)
			SelectCursorType = EBattleCursorType::Default;

		// 커서 변경 이벤트를 방송합니다.
		OnCursorChanged.Broadcast(SelectCursorType, nullptr, nullptr, nullptr);
	}
}

void AGrid::DetermineRangeAttackType(ASlotBase* InSlot)
{
	// 슬롯 간의 거리가 범위 패널티 거리보다 크다면
	const float L_SlotToDistance = GetDistanceToSlot(BattleControllerRef->PlayingCharacter->Slot,
	                                                 InSlot->ContainedUnit->Slot);
	if (L_SlotToDistance > BattleControllerRef->RangePenaltyDistance)
	{
		// BrokenRangeAttack으로 커서 타입을 변경합니다.
		if (OnCursorChanged.IsBound())
		{
			OnCursorChanged.Broadcast(EBattleCursorType::BrokenRangeAttack,
			                          BattleControllerRef->CurrentActiveAbility,
			                          nullptr, nullptr);
		}
	}
	else
	{
		// RangeAttack으로 커서 타입을 변경합니다.
		if (OnCursorChanged.IsBound())
		{
			OnCursorChanged.Broadcast(EBattleCursorType::RangeAttack,
			                          BattleControllerRef->CurrentActiveAbility,
			                          nullptr, nullptr);
		}
	}
}

void AGrid::OnMouseLeaveSlot(ASlotBase* InSlot)
{
	// 가장 가까운 이동 가능한 슬롯 탐지를 중지합니다.
	StopDetectingClosestMovableSlot();

	const auto L_PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	FHitResult L_HitResult; // 로컬 변수명 변경 및 L_ 접두사 추가

	// 마우스 커서 아래에 있는 Actor를 감지합니다.
	const bool L_IsHit = L_PlayerController->GetHitResultUnderCursorByChannel(
		USRPGTypes::ConvertToTraceTypeVisibility(),
		false, L_HitResult);

	if (L_IsHit)
	{
		if (L_HitResult.GetActor() && L_HitResult.GetActor()->IsA<ASlotBase>())
		{
			return;
		}
	}

	// 마우스 아래의 현재 슬롯을 재설정하고 피해 정보 UI를 숨깁니다.
	ResetCurrentSlotUnderMouse();
	HideDamageDetailsUI();
}

void AGrid::ResetMovementData()
{
	// 모든 슬롯 반복
	for (const TTuple<UE::Math::TVector2<double>, ASlotBase*> L_SlotTuple : Slots)
	{
		// 현재 슬롯의 이동 데이터 재설정
		ASlotBase* L_CurrentSlot = L_SlotTuple.Value;
		L_CurrentSlot->ResetMovementData(false);

		// InnerSlots에 현재 슬롯의 좌표가 없으면 나머지 루프를 건너뜁니다.
		if (!InnerSlots.Contains(L_CurrentSlot->Coordinates))
		{
			continue;
		}

		// 내부 슬롯의 이동 데이터 재설정
		ASlotBase* L_InnerSlot = *InnerSlots.Find(L_CurrentSlot->Coordinates);
		L_InnerSlot->ResetMovementData(false);
	}
}

void AGrid::ShowMovementForPlayerCharacter(ACharacterBase* InCharacter)
{
	// 이동 가능한 슬롯을 계산합니다.
	bIsMeleeAttack = CalculateMovableSlots(InCharacter, false, MovableSlots, SlotsInRange, EnemySlots, AllySlots);

	// 각 슬롯을 순회하며 필요한 하이라이트를 적용합니다.
	for (TTuple<UE::Math::TVector2<double>, ASlotBase*>& L_SlotPair : Slots)
	{
		ASlotBase* L_CurrentSlot = L_SlotPair.Value;
		// 현재 플레이어 캐릭터의 슬롯인 경우
		if (BattleControllerRef->PlayingCharacter->Slot == L_CurrentSlot)
		{
			// 플레이어 캐릭터의 슬롯을 하이라이트합니다.
			HighlightPlayingCharacterSlot(L_CurrentSlot);
			continue;
		}

		if (L_CurrentSlot->IsEmpty())
		{
			L_CurrentSlot->Movable(SlotsInRange.Contains(L_CurrentSlot));
		}
		else
		{
			HighlightEnemySlots(L_CurrentSlot);
		}
	}

	// 마우스 아래의 슬롯을 확인합니다.
	CheckSlotUnderMouse();
}

void AGrid::ShowMovementPath(ACharacterBase* InCharacter, ASlotBase* InTargetSlot)
{
	// 이전 스플라인 포인트를 모두 삭제합니다.
	PathSpline->ClearSplinePoints(true);
	if (InTargetSlot->Path.IsEmpty()) return;

	TMap<FVector2D, ASlotBase*> AllSlots = GetAllSlots(InCharacter->UnitSize);
	if (!AllSlots.Contains(InCharacter->Slot->Coordinates)) return;

	// 경로에 대한 스플라인 포인트를 준비합니다.
	PrepareSplinePoints(InCharacter, InTargetSlot, AllSlots);

	//플레이중인 캐릭터가 AI이거나 또는 오토플레이중이라면 함수를 미리 종료합니다. 
	if ((!BattleControllerRef->PlayingCharacter->bIsAI && !BattleControllerRef->bIsAutoPlay) == false) return;

	// 경로 메시를 렌더링합니다.
	RenderPathMesh();
}

void AGrid::PrepareSplinePoints(ACharacterBase* InCharacter, ASlotBase* InTargetSlot,
                                const TMap<FVector2D, ASlotBase*>& AllSlots)
{
	// 캐릭터의 현재 위치를 스플라인 포인트에 추가합니다.
	const ASlotBase* CharacterCoordSlot = *AllSlots.Find(InCharacter->Slot->Coordinates);
	PathSpline->AddSplinePoint(CharacterCoordSlot->GetActorLocation(), ESplineCoordinateSpace::World);

	// 목표 슬롯의 경로를 따라 추가 스플라인 포인트를 설정합니다.
	for (int i = 0; i < InTargetSlot->Path.Num(); i++)
	{
		const ASlotBase* InTargetPathSlot = InTargetSlot->Path[i];
		if (!AllSlots.Contains(InTargetPathSlot->Coordinates)) continue;

		const ASlotBase* FindTargetCoordSlot = *AllSlots.Find(InTargetPathSlot->Coordinates);
		PathSpline->AddSplinePoint(FindTargetCoordSlot->GetActorLocation(), ESplineCoordinateSpace::World);

		// 스플라인 접선을 조정합니다.
		AdjustSplineTangents(i);
	}
}

void AGrid::AdjustSplineTangents(int PointIndex)
{
	// 접선 벡터의 크기를 반으로 조정합니다.
	FVector ArriveTangent = PathSpline->GetArriveTangentAtSplinePoint(PointIndex, ESplineCoordinateSpace::Local) * 0.5f;
	FVector LeaveTangent = PathSpline->GetLeaveTangentAtSplinePoint(PointIndex, ESplineCoordinateSpace::Local) * 0.5f;
	PathSpline->SetTangentsAtSplinePoint(PointIndex, ArriveTangent, LeaveTangent, ESplineCoordinateSpace::Local, true);
}

void AGrid::RenderPathMesh()
{
	// 이전 인스턴스를 모두 삭제합니다.
	PathMesh->ClearInstances();

	const int32 LastIndex = FMath::Floor((PathSpline->GetSplineLength() / 35.0f));

	// 스플라인을 따라 새 인스턴스를 추가합니다.
	for (int32 i = 0; i <= LastIndex; i++)
	{
		AddPathMeshInstance(i);

		if (i > 0)
		{
			// 이전 인스턴스의 회전을 업데이트합니다.
			UpdatePathMeshInstanceRotation(i);
		}
	}

	// 마지막 인스턴스의 회전을 업데이트합니다.
	UpdateLastPathMeshInstanceRotation();
}

void AGrid::AddPathMeshInstance(int Index)
{
	// 스플라인을 따라 적절한 위치에 메시 인스턴스를 추가합니다.
	const FVector DistanceSpline = PathSpline->GetLocationAtDistanceAlongSpline(
		Index * 35.0f, ESplineCoordinateSpace::World);
	PathMesh->AddInstance(FTransform(FRotator::ZeroRotator, DistanceSpline,
	                                 FVector(0.75f, 0.75f, 0.75f)), true);
}

void AGrid::UpdatePathMeshInstanceRotation(int Index)
{
	FTransform OutInstanceTransform;
	PathMesh->GetInstanceTransform(Index - 1, OutInstanceTransform, true);
	FVector DistanceSpline = PathSpline->GetLocationAtDistanceAlongSpline(Index * 35.0f, ESplineCoordinateSpace::World);
	OutInstanceTransform.SetRotation(
		UKismetMathLibrary::FindLookAtRotation(OutInstanceTransform.GetLocation(), DistanceSpline).Quaternion());
	PathMesh->UpdateInstanceTransform(Index - 1, OutInstanceTransform, true);
}

void AGrid::UpdateLastPathMeshInstanceRotation()
{
	int32 PathInstanceCount = PathMesh->GetInstanceCount() - 1;
	FTransform OutInstanceTransform;
	PathMesh->GetInstanceTransform(PathInstanceCount, OutInstanceTransform, true);
	UE::Math::TQuat<double> CloseRotation = PathSpline->FindRotationClosestToWorldLocation(
		OutInstanceTransform.GetLocation(), ESplineCoordinateSpace::World).Quaternion();
	OutInstanceTransform.SetRotation(CloseRotation);
	PathMesh->UpdateInstanceTransform(PathInstanceCount, OutInstanceTransform, true);
}

void AGrid::ClearMovementPath()
{
	PathSpline->ClearSplinePoints(true);
	PathMesh->ClearInstances();
}

void AGrid::HighlightMovementSlots(ASlotBase* InSlot, int32 InSize)
{
	InSlot->SetSlotType(ESlotType::MovementOverlaySlot, InSize);
}

void AGrid::ShowCharacterMovementOverlay(ACharacterBase* InCharacter)
{
	// 캐릭터가 유효하지 않거나 현재 재생 중이면 기능을 종료합니다.
	if ((IsValid(InCharacter) && BattleControllerRef->PlayingCharacter != InCharacter) == false) return;

	// 캐릭터에 대한 이동 오버레이가 이미 존재하는 경우 이를 사용하십시오.
	if (CharacterMovementOverlaySlots.Contains(InCharacter))
	{
		MovementOverlaySlotsInRange = CharacterMovementOverlaySlots.Find(InCharacter)->OverlaySlots;
	}
	// Else, 캐릭터의 이동 오버레이를 계산하고 저장합니다.
	else
	{
		MovementOverlaySlotsInRange = CalculateAndStoreOverlaySlots(InCharacter);
	}

	// 이동 오버레이의 모든 슬롯을 강조 표시합니다.
	HighlightMovementOverlaySlots(InCharacter);
}

TArray<ASlotBase*> AGrid::CalculateAndStoreOverlaySlots(ACharacterBase* InCharacter)
{
	TArray<ASlotBase*> StoreOverSlots, OutMoveAbleSlots, OutSlotsInRange, OutEnemySlots, OutAllySlot;
	CalculateMovableSlots(InCharacter, true, OutMoveAbleSlots, OutSlotsInRange, OutEnemySlots, OutAllySlot);
	StoreOverSlots.Append(OutSlotsInRange);
	StoreOverSlots.Append(OutEnemySlots);
	CharacterMovementOverlaySlots.Add(InCharacter, FListOfSlots(OutSlotsInRange));
	return OutSlotsInRange;
}

void AGrid::HighlightMovementOverlaySlots(ACharacterBase* InCharacter)
{
	for (ASlotBase* InSlot : MovementOverlaySlotsInRange)
	{
		// 슬롯이 비어 있으면 강조 표시합니다.
		if (InSlot->IsEmpty())
		{
			HighlightEmptySlot(InSlot);
		}
		// 슬롯에 적이 포함되어 있으면 강조 표시합니다.
		else
		{
			HighlightEnemySlot(InCharacter, InSlot);
		}
	}
}

void AGrid::HighlightEmptySlot(ASlotBase* InSlot)
{
	if (!OverlaySlots.Contains(InSlot->Coordinates))
	{
		return;
	}

	ASlotBase* FindOverlaySlot = *OverlaySlots.Find(InSlot->Coordinates);
	FindOverlaySlot->SetSlotType(ESlotType::MovementOverlaySlot, 1);
}

void AGrid::HighlightEnemySlot(ACharacterBase* InCharacter, ASlotBase* InSlot)
{
	ACharacterBase* OutEnemyCharacter;
	if (InSlot->IsEnemySlot(InCharacter, OutEnemyCharacter) && InSlot->ContainedUnit->Slot == InSlot)
	{
		MovementSlotsForEnemy(InCharacter, InSlot);
	}
}

void AGrid::MovementSlotsForEnemy(ACharacterBase* InCharacter, ASlotBase* InSlot)
{
	TMap<FVector2D, ASlotBase*> AllSlots = GetAllOverlaySlots(InSlot->ContainedUnit->UnitSize);

	if (!OverlaySlots.Contains(InSlot->Coordinates)) { return; }

	const ASlotBase* FindOverlaySlot = *OverlaySlots.Find(InSlot->Coordinates);

	if (AllSlots.Contains(FindOverlaySlot->Coordinates)) { return; }

	ASlotBase* FindToAllSlot = *AllSlots.Find(FindOverlaySlot->Coordinates);
	HighlightMovementSlots(FindToAllSlot, InSlot->ContainedUnit->UnitSize);
}

void AGrid::ClearAttackSlots()
{
	for (ASlotBase* EnemySlot : EnemySlots)
	{
		EnemySlot->ClearCanAttackFromSlots();
	}
}

void AGrid::RemoveHighlightOnCharacters()
{
	for (ACharacterBase* InHighlightedCharacter : HighlightedCharacters)
	{
		InHighlightedCharacter->Highlight(false);
	}
	HighlightedCharacters.Empty();
}

void AGrid::HighlightSlot(ASlotBase* InSlot, int32 InSize, bool IsOn)
{
	// 입력 슬롯 유효성 검사
	if (!Slots.Contains(InSlot->Coordinates)) return;

	// 현재 슬롯 및 크기에 대한 변수 초기화
	ASlotBase* CurrentSlot = InSlot;
	int32 CurrentSize = InSize;

	// 크기가 짝수이고 현재 슬롯이 플레이 중인 캐릭터의 슬롯이 아닌 경우 숨기기
	HideEvenSizedNonPlayerSlots(CurrentSlot, CurrentSize);

	// 범위 내 모든 슬롯 가져오기
	bool AllFree;
	TArray<ASlotBase*> OutSlotsInRange;
	GetSlotsInRange(CurrentSlot, CurrentSize, false, OutSlotsInRange, AllFree);

	// 'IsOn' 플래그를 기반으로 범위 내 슬롯의 가시성을 조정합니다.
	AdjustSlotsInRangeVisibility(OutSlotsInRange, IsOn, CurrentSlot);

	// 'IsOn'이 참이면 현재 슬롯을 강조 표시합니다.
	if (IsOn)
	{
		CurrentSlot->Highlight(true, CurrentSize);
	}
}

void AGrid::HideEvenSizedNonPlayerSlots(ASlotBase* CurrentSlot, int32 CurrentSize)
{
	if (CurrentSize % 2 == 0 && *Slots.Find(CurrentSlot->Coordinates) != BattleControllerRef->PlayingCharacter->Slot)
	{
		CurrentSlot->HideSlot();
	}
}

void AGrid::AdjustSlotsInRangeVisibility(const TArray<ASlotBase*>& OutSlotsInRange, bool IsOn, ASlotBase* CurrentSlot)
{
	// 범위의 각 슬롯에 대해 반복
	for (ASlotBase* InSlotsInRange : OutSlotsInRange)
	{
		// IsOn이 참이면 현재 슬롯 유형을 저장하고 비 플레이어 슬롯을 숨깁니다.
		if (IsOn)
		{
			InSlotsInRange->SaveSlotType();
			// 범위 내 현재 슬롯이 플레이어의 캐릭터에 의해 점유되지 않은 경우 슬롯을 숨깁니다.
			if (InSlotsInRange != BattleControllerRef->PlayingCharacter->Slot)
			{
				InSlotsInRange->HideSlot();
			}
		}
		// 그렇지 않으면 비 플레이어 슬롯을 강조 표시합니다.
		else
		{
			// 범위 내 현재 슬롯이 플레이어의 캐릭터에 의해 점유되지 않은 경우 슬롯을 강조 표시합니다.
			if (InSlotsInRange != BattleControllerRef->PlayingCharacter->Slot)
			{
				InSlotsInRange->Highlight(false, 1);
			}
		}
	}
}

void AGrid::HighlightPlayingCharacterSlot(ASlotBase* InSlot)
{
	InSlot->HideSlot();
	TMap<FVector2D, ASlotBase*> AllSlots = GetAllSlots(BattleControllerRef->PlayingCharacter->UnitSize);
	if (!AllSlots.Contains(InSlot->Coordinates)) { return; }
	InSlot->Highlight(true, BattleControllerRef->PlayingCharacter->UnitSize);
}

void AGrid::HighlightEnemySlots(ASlotBase* InSlot)
{
	InSlot->HideSlot();

	if ((EnemySlots.Contains(InSlot) && (InSlot == InSlot->ContainedUnit->Slot)) == false) { return; }

	TMap<FVector2D, ASlotBase*> AllSlots = GetAllSlots(InSlot->ContainedUnit->UnitSize);

	if (!AllSlots.Contains(InSlot->Coordinates)) { return; }
	ASlotBase* FindCoordSlot = *AllSlots.Find(InSlot->Coordinates);
	FindCoordSlot->SetEnemySlot(InSlot->ContainedUnit->UnitSize);
}

bool AGrid::CalculateMovableSlots(ACharacterBase* InCharacter, bool OnlyVisual, TArray<ASlotBase*>& OutMoveAbleSlots,
                                  TArray<ASlotBase*>& OutSlotsInRange, TArray<ASlotBase*>& OutEnemySlots,
                                  TArray<ASlotBase*>& OutAllySlots)
{
	auto CurrentCharacter = InCharacter;
	auto CurrentOnlyVisual = OnlyVisual;
	const bool CurrentIsMeleeAttack = DetermineMeleeAttack(CurrentCharacter);

	TArray<ASlotBase*> OpenSlots, ClosedSlots;
	TArray<ASlotBase*> CurrentSlotsInRange, CurrentMovableSlots, CurrentAllySlots, CurrentEnemySlots;

	ResetDataBasedOnVisual(CurrentOnlyVisual, CurrentCharacter);

	for (const TTuple<UE::Math::TVector2<double>, ASlotBase*> InSlotPair : Slots)
	{
		ASlotBase* InSlot = InSlotPair.Value;

		HandleSlotBasedOnMeleeAttack(CurrentIsMeleeAttack, CurrentCharacter, InSlot, CurrentAllySlots,
		                             CurrentEnemySlots);

		const float CoordDistance = GetDistanceBetweenCoordinates(CurrentCharacter->Slot->Coordinates,
		                                                          InSlot->Coordinates);
		if ((CoordDistance <= CurrentCharacter->CurrentMovementRange &&
			!InSlot->ContainsObstacle() && CurrentCharacter->Slot != InSlot) == false)
		{
			continue;
		}

		HandleMovableSlot(CurrentCharacter, CurrentOnlyVisual, InSlot, CurrentMovableSlots);
	}

	switch (CurrentCharacter->MovementType)
	{
	case EMovementType::Walk:
		HandleWalkMovementType(CurrentCharacter, CurrentOnlyVisual, OpenSlots, ClosedSlots,
		                       CurrentSlotsInRange, CurrentMovableSlots, CurrentAllySlots, CurrentEnemySlots);
		OutMoveAbleSlots = ClosedSlots;
		OutSlotsInRange = CurrentSlotsInRange;
		OutEnemySlots = CurrentEnemySlots;
		OutAllySlots = CurrentAllySlots;
		return CurrentIsMeleeAttack;

	case EMovementType::Fly:
		HandleFlyMovementType(CurrentCharacter, CurrentOnlyVisual, CurrentMovableSlots, CurrentSlotsInRange,
		                      CurrentAllySlots, CurrentEnemySlots);
		OutMoveAbleSlots = CurrentMovableSlots;
		OutSlotsInRange = CurrentSlotsInRange;
		OutEnemySlots = CurrentEnemySlots;
		OutAllySlots = CurrentAllySlots;
		return CurrentIsMeleeAttack;
	case EMovementType::Immobilie:
		HandleImmobilieMovementType(CurrentCharacter, CurrentOnlyVisual, CurrentSlotsInRange, CurrentAllySlots,
		                            CurrentEnemySlots);

		OutEnemySlots = CurrentEnemySlots;
		OutAllySlots = CurrentAllySlots;
		return CurrentIsMeleeAttack;

	default: return CurrentIsMeleeAttack;
	}
}

bool AGrid::DetermineMeleeAttack(ACharacterBase* CurrentCharacter)
{
	if (const ARangeCharacterBase* CastRangeCharacter = Cast<ARangeCharacterBase>(CurrentCharacter))
		return CastRangeCharacter->CurrentAmmo <= 0 || IsEnemyAdjacent(CurrentCharacter);

	return true;
}

void AGrid::ResetDataBasedOnVisual(bool CurrentOnlyVisual, ACharacterBase* CurrentCharacter)
{
	if (CurrentOnlyVisual)
	{
		ResetOverlayData();
	}
	else
	{
		ResetMovementData();
		ClearAttackSlots();
	}

	CurrentCharacter->Slot->TotalCost = 0;
}

void AGrid::HandleSlotBasedOnMeleeAttack(bool CurrentIsMeleeAttack, ACharacterBase* CurrentCharacter, ASlotBase* InSlot,
                                         TArray<ASlotBase*>& CurrentAllySlots, TArray<ASlotBase*>& CurrentEnemySlots)
{
	if (CurrentIsMeleeAttack) { return; }

	ACharacterBase* OutEnemyCharacter;
	if (InSlot->IsEnemySlot(CurrentCharacter, OutEnemyCharacter))
	{
		CurrentEnemySlots.AddUnique(InSlot);
		return;
	}

	ACharacterBase* OutAllyCharacter;
	if (InSlot->IsAllySlot(CurrentCharacter, OutAllyCharacter))
	{
		CurrentAllySlots.AddUnique(InSlot);
	}
}

void AGrid::HandleMovableSlot(ACharacterBase* CurrentCharacter, bool CurrentOnlyVisual, ASlotBase* InSlot,
                              TArray<ASlotBase*>& CurrentMovableSlots)
{
	CurrentMovableSlots.Add(InSlot);
	InSlot->TotalCost = 999;

	if (CurrentCharacter->MovementType == EMovementType::Fly)
	{
		if (CurrentOnlyVisual)
		{
			InSlot->OverlayPath.Add(InSlot);
		}
		else
		{
			InSlot->Path.Add(InSlot);

			if (InnerSlots.Contains(InSlot->Coordinates))
			{
				ASlotBase* FindInnerSlot = *InnerSlots.Find(InSlot->Coordinates);
				FindInnerSlot->Path.Add(FindInnerSlot);
			}
		}
	}
}

void AGrid::HandleWalkMovementType(ACharacterBase* CurrentCharacter, bool CurrentOnlyVisual,
                                   TArray<ASlotBase*>& OpenSlots, TArray<ASlotBase*>& ClosedSlots,
                                   TArray<ASlotBase*>& CurrentSlotsInRange, TArray<ASlotBase*>& CurrentMovableSlots,
                                   TArray<ASlotBase*>& CurrentAllySlots, TArray<ASlotBase*>& CurrentEnemySlots)
{
	OpenSlots.Add(CurrentCharacter->Slot);

	while (OpenSlots.Num() != 0)
	{
		ASlotBase* CurrentSlot = GetLowestTravelCostSlot(OpenSlots);

		OpenSlots.Remove(CurrentSlot);
		ClosedSlots.AddUnique(CurrentSlot);

		TArray<ASlotBase*> OutAllSlotsUnitWillStandOn;
		if (CanUnitFitTo(CurrentSlot, CurrentCharacter, OutAllSlotsUnitWillStandOn) == false) { continue; }

		if (CurrentSlot != CurrentCharacter->Slot)
		{
			for (ASlotBase* InSlot : OutAllSlotsUnitWillStandOn)
			{
				CurrentSlotsInRange.AddUnique(InSlot);
			}
		}

		TArray<ASlotBase*> NeighbourSlots = GetNeighbourSlots(CurrentSlot, 1, false);
		for (ASlotBase* CurrentNeighbourSlot : NeighbourSlots)
		{
			TArray<ASlotBase*>& SelectPath = CurrentOnlyVisual
				                                 ? CurrentNeighbourSlot->OverlayPath
				                                 : CurrentNeighbourSlot->Path;

			if ((!ClosedSlots.Contains(CurrentNeighbourSlot) && SelectPath.Num() == 0) == false) { continue; }
			if (!CurrentMovableSlots.Contains(CurrentNeighbourSlot)) { continue; }
			if (!CanUnitFitTo(CurrentNeighbourSlot, CurrentCharacter, OutAllSlotsUnitWillStandOn)) { continue; }

			const int32 CurrentDistanceCost =
				GetDistanceCost(CurrentSlot, CurrentNeighbourSlot) + CurrentSlot->TotalCost;

			if ((CurrentDistanceCost <= CurrentCharacter->CurrentMovementRange * 10) == false) { continue; }

			CurrentNeighbourSlot->TotalCost = CurrentDistanceCost;

			OpenSlots.Add(CurrentNeighbourSlot);

			if (CurrentOnlyVisual)
			{
				CurrentNeighbourSlot->OverlayPath = CurrentSlot->OverlayPath;
				CurrentNeighbourSlot->OverlayPath.Add(CurrentNeighbourSlot);
				continue;
			}

			CurrentNeighbourSlot->Path = CurrentSlot->Path;
			CurrentNeighbourSlot->Path.Add(CurrentNeighbourSlot);

			if (InnerSlots.Contains(CurrentSlot->Coordinates) &&
				InnerSlots.Contains(CurrentNeighbourSlot->Coordinates))
			{
				ASlotBase* FindInnerSlot = *InnerSlots.Find(CurrentSlot->Coordinates);
				ASlotBase* FindNeighbourInnerSlot = *InnerSlots.Find(CurrentNeighbourSlot->Coordinates);
				FindNeighbourInnerSlot->Path = FindInnerSlot->Path;
				FindNeighbourInnerSlot->Path.Add(FindNeighbourInnerSlot);
			}
		} //for end


		NeighbourSlots = GetNeighbourSlots(CurrentSlot, CurrentCharacter->UnitSize, false);
		for (ASlotBase* InNeighbourSlot : NeighbourSlots)
		{
			bool AllFree;
			ACharacterBase* L_OutCharacter;
			TArray<ASlotBase*> L_OutSlotsInRange;
			if (InNeighbourSlot->IsEnemySlot(CurrentCharacter, L_OutCharacter))
			{
				GetSlotsInRange(InNeighbourSlot->ContainedUnit->Slot, InNeighbourSlot->ContainedUnit->UnitSize,
				                false, L_OutSlotsInRange, AllFree);

				for (ASlotBase* InRangeSlot : L_OutSlotsInRange)
				{
					CurrentEnemySlots.AddUnique(InRangeSlot);
					if (!CurrentOnlyVisual)
					{
						(bIsHexGrid ? InNeighbourSlot : InRangeSlot)->AddSlotToCanAttackFromSlots(CurrentSlot);
					}
				}
				continue;
			}

			if (InNeighbourSlot->IsAllySlot(CurrentCharacter, L_OutCharacter))
			{
				GetSlotsInRange(InNeighbourSlot->ContainedUnit->Slot, InNeighbourSlot->ContainedUnit->UnitSize,
				                false, L_OutSlotsInRange, AllFree);

				for (ASlotBase* InRangeSlot : L_OutSlotsInRange)
				{
					CurrentAllySlots.AddUnique(InRangeSlot);
				}
			}
		}
	}

	ClosedSlots.Remove(CurrentCharacter->Slot);
	if (!CurrentCharacter->IsEvenSizedUnit())
	{
		CurrentSlotsInRange.Remove(CurrentCharacter->Slot);
	}
}

void AGrid::HandleFlyMovementType(ACharacterBase* CurrentCharacter, bool CurrentOnlyVisual,
                                  TArray<ASlotBase*>& CurrentMovableSlots, TArray<ASlotBase*>& CurrentSlotsInRange,
                                  TArray<ASlotBase*>& CurrentAllySlots, TArray<ASlotBase*>& CurrentEnemySlots)
{
	CurrentMovableSlots.Add(CurrentCharacter->Slot);

	for (int i = CurrentMovableSlots.Num() - 1; i >= 0; --i)
	{
		ASlotBase* CurrentSlot = CurrentMovableSlots[i];

		TArray<ASlotBase*> L_AllSlotsUnitWillStandOn;
		if (CanUnitFitTo(CurrentSlot, CurrentCharacter, L_AllSlotsUnitWillStandOn))
		{
			if (CurrentSlot != CurrentCharacter->Slot)
			{
				for (ASlotBase* InStandSlot : L_AllSlotsUnitWillStandOn)
				{
					CurrentSlotsInRange.AddUnique(InStandSlot);
				}
			}

			TArray<ASlotBase*> NeighbourSlots = GetNeighbourSlots(CurrentSlot, CurrentCharacter->UnitSize, false);

			for (ASlotBase* CurrentNeighbourSlot : NeighbourSlots)
			{
				bool L_AllFree;
				ACharacterBase* L_OutCharacter;
				if (CurrentNeighbourSlot->IsEnemySlot(CurrentCharacter, L_OutCharacter))
				{
					TArray<ASlotBase*> L_OutSlotsInRange;
					GetSlotsInRange(CurrentNeighbourSlot->ContainedUnit->Slot,
					                CurrentNeighbourSlot->ContainedUnit->UnitSize, false,
					                L_OutSlotsInRange, L_AllFree);

					for (ASlotBase* InRangeSlot : L_OutSlotsInRange)
					{
						CurrentEnemySlots.AddUnique(InRangeSlot);

						if (!CurrentOnlyVisual == false) continue;

						(bIsHexGrid ? CurrentNeighbourSlot : InRangeSlot)->AddSlotToCanAttackFromSlots(CurrentSlot);
					}
					continue;
				}

				if (CurrentNeighbourSlot->IsAllySlot(CurrentCharacter, L_OutCharacter))
				{
					TArray<ASlotBase*> L_OutSlotsInRange;
					GetSlotsInRange(CurrentNeighbourSlot->ContainedUnit->Slot,
					                CurrentNeighbourSlot->ContainedUnit->UnitSize, false,
					                L_OutSlotsInRange, L_AllFree);

					for (ASlotBase* InRangeSlot : L_OutSlotsInRange)
					{
						CurrentAllySlots.AddUnique(InRangeSlot);
					}
				}
			}
		}
		else
		{
			CurrentMovableSlots.Remove(CurrentSlot);
		}
	} //Reverse for End

	CurrentMovableSlots.Remove(CurrentCharacter->Slot);
	if (!CurrentCharacter->IsEvenSizedUnit())
	{
		CurrentSlotsInRange.Remove(CurrentCharacter->Slot);
	}
}

void AGrid::HandleImmobilieMovementType(ACharacterBase* CurrentCharacter, bool CurrentOnlyVisual,
                                        TArray<ASlotBase*>& CurrentSlotsInRange, TArray<ASlotBase*>& CurrentAllySlots,
                                        TArray<ASlotBase*>& CurrentEnemySlots)
{
	ASlotBase* CurrentSlot = CurrentCharacter->Slot;

	TArray<ASlotBase*> NeighbourSlots = GetNeighbourSlots(CurrentSlot, CurrentCharacter->UnitSize, false);
	for (ASlotBase* CurrentNeighbourSlot : NeighbourSlots)
	{
		bool L_AllFree;
		ACharacterBase* L_OutCharacter;
		TArray<ASlotBase*> L_OutSlotsInRange;

		if (CurrentNeighbourSlot->IsEnemySlot(CurrentCharacter, L_OutCharacter))
		{
			GetSlotsInRange(CurrentNeighbourSlot->ContainedUnit->Slot,
			                CurrentNeighbourSlot->ContainedUnit->UnitSize,
			                false, L_OutSlotsInRange, L_AllFree);

			for (ASlotBase* InRangeSlot : L_OutSlotsInRange)
			{
				CurrentEnemySlots.AddUnique(InRangeSlot);

				if (!CurrentOnlyVisual == false) { continue; }

				(bIsHexGrid ? CurrentNeighbourSlot : InRangeSlot)->AddSlotToCanAttackFromSlots(CurrentSlot);
			}

			continue;
		}
		if (CurrentNeighbourSlot->IsAllySlot(CurrentCharacter, L_OutCharacter))
		{
			GetSlotsInRange(CurrentNeighbourSlot->ContainedUnit->Slot,
			                CurrentNeighbourSlot->ContainedUnit->UnitSize,
			                false, L_OutSlotsInRange, L_AllFree);

			for (ASlotBase* InRangeSlot : L_OutSlotsInRange)
			{
				CurrentAllySlots.AddUnique(InRangeSlot);
			}
		}
	}
}

void AGrid::ResetOverlayData()
{
	for (const TTuple<UE::Math::TVector2<double>, ASlotBase*> InSlot : Slots)
	{
		InSlot.Value->ResetMovementData(true);
	}

	for (ASlotBase* InOverlaySlotsInRange : MovementOverlaySlotsInRange)
	{
		InOverlaySlotsInRange->ResetMovementData(true);
	}

	HideOverlaySlots();
}

void AGrid::HideOverlaySlots()
{
	for (ASlotBase* InSlot : MovementOverlaySlotsInRange)
	{
		if (!OverlaySlots.Contains(InSlot->Coordinates)) { continue; }

		ASlotBase* FindOverlaySlot = *OverlaySlots.Find(InSlot->Coordinates);
		FindOverlaySlot->HideSlot();

		if (!InnerOverlaySlots.Contains(InSlot->Coordinates)) { continue; }

		ASlotBase* FindInnerOverlaySlot = *InnerOverlaySlots.Find(InSlot->Coordinates);
		FindInnerOverlaySlot->HideSlot();
	}
	MovementOverlaySlotsInRange.Empty();
}

void AGrid::HideDamageDetailsUI()
{
	RemoveHighlightOnCharacters();

	if (IsValid(UW_DamageDetailsListUI))
	{
		UW_DamageDetailsListUI->RemoveFromParent();
		UW_DamageDetailsListUI = nullptr;
	}
}

void AGrid::ShowDamageDetailsUI(ASlotBase* InTargetSlot, ASlotBase* InStandingSlot)
{
	ACharacterBase* OutMainTargetCharacter;
	BattleControllerRef->GetTargetCharacters(BattleControllerRef->PlayingCharacter, InTargetSlot,
	                                         InStandingSlot, bIsMeleeAttack, HighlightedCharacters,
	                                         OutMainTargetCharacter);

	HideDamageDetailsUI();
	USRPGUtilityLibrary::ReverseArray(HighlightedCharacters);

	SRPG_CHECK(WBP_DamageDetailsListUIClass);
	UW_DamageDetailsListUI = CreateWidget<UUW_DamageDetailsListUI>(GetWorld(), WBP_DamageDetailsListUIClass);
	UW_DamageDetailsListUI->BattleController = BattleControllerRef;
	UW_DamageDetailsListUI->AttackingCharacter = BattleControllerRef->PlayingCharacter;
	UW_DamageDetailsListUI->TargetCharacters = HighlightedCharacters;
	UW_DamageDetailsListUI->bIsMeleeAttack = false;
	UW_DamageDetailsListUI->MainTarget = Cast<ACharacterBase>(InTargetSlot->ContainedUnit);
	UW_DamageDetailsListUI->ShowDamageDetailsUI();

	const FVector2D ViewportMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	UW_DamageDetailsListUI->SetPositionInViewport(ViewportMousePosition, false);
	UW_DamageDetailsListUI->AddToViewport();

	for (ACharacterBase* HighlightedCharacter : HighlightedCharacters)
	{
		HighlightedCharacter->Highlight(true);
	}
}

void AGrid::ShowDamageDetailsUIForAreaAbility()
{
	USRPGUtilityLibrary::ReverseArray(HighlightedCharacters);

	const TMap<ACharacterBase*, FDamageData> DamageData =
		CurrentAreaTargetActiveAbility->GetAbilityDamage(HighlightedCharacters);

	const TMap<ACharacterBase*, FDamageData> ElementDamageData =
		BattleControllerRef->GetElementReduction(CurrentAreaTargetActiveAbility->Element, DamageData);

	SRPG_CHECK(WBP_DamageDetailsListUIClass);
	if (ElementDamageData.Num() > 0 && HighlightedCharacters.Num() > 0)
	{
		UW_DamageDetailsListUI = CreateWidget<UUW_DamageDetailsListUI>(GetWorld(), WBP_DamageDetailsListUIClass);
		UW_DamageDetailsListUI->BattleController = BattleControllerRef;
		UW_DamageDetailsListUI->AttackingCharacter = BattleControllerRef->PlayingCharacter;
		UW_DamageDetailsListUI->TargetCharacters = HighlightedCharacters;
		UW_DamageDetailsListUI->bIsMeleeAttack = bIsMeleeAttack;
		UW_DamageDetailsListUI->MainTarget = nullptr;
		UW_DamageDetailsListUI->ShowDamageDetailsUIForAbility(ElementDamageData);

		const FVector2D ViewportMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
		UW_DamageDetailsListUI->SetPositionInViewport(ViewportMousePosition, false);
		UW_DamageDetailsListUI->AddToViewport();
	}
}

void AGrid::ShowDamageDetailsUIForAreaSpell()
{
	USRPGUtilityLibrary::ReverseArray(HighlightedCharacters);

	const TMap<ACharacterBase*, FDamageData> DamageData =
		BattleControllerRef->CurrentSpell->GetSpellDamage(HighlightedCharacters);

	TMap<ACharacterBase*, FDamageData> ElementDamageData =
		BattleControllerRef->GetElementReduction(BattleControllerRef->CurrentSpell->Element, DamageData);

	SRPG_CHECK(WBP_DamageDetailsListUIClass);
	if (ElementDamageData.Num() > 0 && HighlightedCharacters.Num() > 0)
	{
		UW_DamageDetailsListUI = CreateWidget<UUW_DamageDetailsListUI>(GetWorld(), WBP_DamageDetailsListUIClass);
		UW_DamageDetailsListUI->BattleController = BattleControllerRef;
		UW_DamageDetailsListUI->AttackingCharacter = BattleControllerRef->PlayingCharacter;
		UW_DamageDetailsListUI->TargetCharacters = HighlightedCharacters;
		UW_DamageDetailsListUI->bIsMeleeAttack = false;
		UW_DamageDetailsListUI->MainTarget = nullptr;
		UW_DamageDetailsListUI->ShowDamageDetailsUIForAbility(ElementDamageData);

		const FVector2D ViewportMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
		UW_DamageDetailsListUI->SetPositionInViewport(ViewportMousePosition, false);
		UW_DamageDetailsListUI->AddToViewport();
	}
}

void AGrid::OnMouseOverUI()
{
	HideDamageDetailsUI();
	ResetCurrentSlotUnderMouse();
}

void AGrid::OnAreaTargetSpellEnabled(AAreaTargetSpellBase* InAreaTargetSpell)
{
	CurrentAreaTargetSpell = InAreaTargetSpell;
	bAreaSpellEnabled = true;
}

void AGrid::OnAreaTargetSpellDisabled()
{
	bAreaSpellEnabled = false;
	HideOverlaySlots();
	CurrentAreaTargetSpell = nullptr;
}

void AGrid::OnAreaTargetAbilityEnabled(AAreaTargetActiveAbilityBase* InAreaTargetActiveAbility)
{
	CurrentAreaTargetActiveAbility = InAreaTargetActiveAbility;
	bAreaTargetAbilityEnabled = true;
}

void AGrid::OnAreaTargetAbilityDisabled()
{
	bAreaTargetAbilityEnabled = false;
	HideOverlaySlots();
	CurrentAreaTargetActiveAbility = nullptr;
}

void AGrid::OnEnemyTargetSpellEnabled()
{
	bEnemyTargetSpellEnabled = true;

	if (BattleControllerRef->BattleState == EBattleState::WaitingForPlayerAction)
	{
		GetAllCharacterSlots(AllySlots, EnemySlots);

		for (ASlotBase* InEnemySlot : EnemySlots)
		{
			InEnemySlot->HideSlot();

			if (InEnemySlot == InEnemySlot->ContainedUnit->Slot)
			{
				auto AllSlots = GetAllSlots(InEnemySlot->ContainedUnit->UnitSize);
				if (AllSlots.Contains(InEnemySlot->Coordinates))
				{
					auto CoordinatesSlot = *AllSlots.Find(InEnemySlot->Coordinates);
					CoordinatesSlot->SetEnemySlot(InEnemySlot->ContainedUnit->UnitSize);
				}
			}
		}

		for (ASlotBase* InAllySlot : AllySlots)
		{
			InAllySlot->HideSlot();
		}
	}
}

void AGrid::OnEnemyTargetSpellDisabled()
{
	bEnemyTargetSpellEnabled = false;
}

void AGrid::OnAllyTargetSpellEnabled()
{
	bAllyTargetSpellEnabled = true;

	if (BattleControllerRef->BattleState == EBattleState::WaitingForPlayerAction)
	{
		GetAllCharacterSlots(AllySlots, EnemySlots);

		for (ASlotBase* InEnemySlot : EnemySlots)
		{
			if (InEnemySlot->ContainedUnit->Slot == InEnemySlot)
			{
				auto AllSlots = GetAllSlots(InEnemySlot->ContainedUnit->UnitSize);

				if (AllSlots.Contains(InEnemySlot->Coordinates))
				{
					auto CoordinatesSlot = *AllSlots.Find(InEnemySlot->Coordinates);
					CoordinatesSlot->HideSlot();
				}
			}
		}

		for (ASlotBase* InAllySlot : AllySlots)
		{
			InAllySlot->HideSlot();

			if (InAllySlot == InAllySlot->ContainedUnit->Slot)
			{
				auto AllSlots = GetAllSlots(InAllySlot->ContainedUnit->UnitSize);

				if (AllSlots.Contains(InAllySlot->Coordinates))
				{
					auto CoordinatesSlot = *AllSlots.Find(InAllySlot->Coordinates);
					CoordinatesSlot->SetAllySlot(InAllySlot->ContainedUnit->UnitSize);
				}
			}
		}
	}
}

void AGrid::OnAllyTargetSpellDisabled()
{
	bAllyTargetSpellEnabled = false;
}

void AGrid::OnAllyTargetAbilityEnabled()
{
	bAllyTargetAbilityEnabled = true;

	if (BattleControllerRef->BattleState != EBattleState::WaitingForPlayerAction)
	{
		return;
	}

	for (ASlotBase* InEnemySlot : EnemySlots)
	{
		if (InEnemySlot != InEnemySlot->ContainedUnit->Slot)
		{
			continue;
		}

		auto AllSlots = GetAllSlots(InEnemySlot->ContainedUnit->UnitSize);
		if (AllSlots.Contains(InEnemySlot->Coordinates))
		{
			(*AllSlots.Find(InEnemySlot->Coordinates))->HideSlot();
		}
	}

	for (ASlotBase* InAllySlot : AllySlots)
	{
		if (InAllySlot != InAllySlot->ContainedUnit->Slot)
		{
			continue;
		}

		auto AllSlots = GetAllSlots(InAllySlot->ContainedUnit->UnitSize);
		if (AllSlots.Contains(InAllySlot->Coordinates))
		{
			(*AllSlots.Find(InAllySlot->Coordinates))->SetAllySlot(InAllySlot->ContainedUnit->UnitSize);
		}
	}
}

void AGrid::OnAllyTargetAbilityDisabled()
{
	bAllyTargetAbilityEnabled = false;
	if ((BattleControllerRef->BattleState == EBattleState::WaitingForPlayerAction) == false)
	{
		return;
	}

	for (ASlotBase* InEnemySlot : EnemySlots)
	{
		if (InEnemySlot->ContainedUnit->Slot != InEnemySlot)
		{
			continue;
		}

		auto AllSlots = GetAllSlots(InEnemySlot->ContainedUnit->UnitSize);
		if (!AllSlots.Contains(InEnemySlot->Coordinates))
		{
			continue;
		}

		auto EnemyCoordSlot = *AllSlots.Find(InEnemySlot->Coordinates);
		EnemyCoordSlot->SetEnemySlot(InEnemySlot->ContainedUnit->UnitSize);
	}

	for (ASlotBase* InAllySlot : AllySlots)
	{
		if (InAllySlot != InAllySlot->ContainedUnit->Slot)
		{
			continue;
		}

		if (InAllySlot == BattleControllerRef->PlayingCharacter->Slot)
		{
			auto AllSlots = GetAllSlots(BattleControllerRef->PlayingCharacter->UnitSize);
			if (AllSlots.Contains(BattleControllerRef->PlayingCharacter->Slot->Coordinates))
			{
				(*AllSlots.Find(BattleControllerRef->PlayingCharacter->Slot->Coordinates))->Highlight(true,
					BattleControllerRef->PlayingCharacter->UnitSize);
			}
		}
		else
		{
			auto AllSlots = GetAllSlots(InAllySlot->ContainedUnit->UnitSize);
			if (AllSlots.Contains(InAllySlot->Coordinates))
			{
				(*AllSlots.Find(InAllySlot->Coordinates))->HideSlot();
			}
		}
	}
}

void AGrid::ShowSpellSlots(ASlotBase* InTargetSlot)
{
	RemoveHighlightOnCharacters();

	if (bAllyTargetSpellEnabled)
	{
		ACharacterBase* OutAllyCharacter;
		if (InTargetSlot->IsAllySlot(BattleControllerRef->PlayingCharacter, OutAllyCharacter))
		{
			SetSpellTarget(InTargetSlot, OutAllyCharacter);
			return;
		}
		return;
	}

	if (bEnemyTargetSpellEnabled)
	{
		HideDamageDetailsUI();
		ACharacterBase* OutEnemyCharacter;
		if (InTargetSlot->IsEnemySlot(BattleControllerRef->PlayingCharacter, OutEnemyCharacter))
		{
			SetSpellTarget(InTargetSlot, OutEnemyCharacter);
			ShowDamageDetailsUIForAreaSpell();
			return;
		}
		return;
	}

	HideOverlaySlots();
	if (bAreaSpellEnabled && IsValid(InTargetSlot) && !InTargetSlot->ContainsObstacle())
	{
		if (OnCursorChanged.IsBound())
		{
			OnCursorChanged.Broadcast(EBattleCursorType::Spell, nullptr,
			                          BattleControllerRef->CurrentSpell, nullptr);
		}

		BattleControllerRef->CurrentSpell->SetTargetSlot(InTargetSlot);
		TArray<ASlotBase*> OutSlotsInRange, OutEmptySlots;
		TArray<ACharacterBase*> OutAllies, OutEnemies;
		BattleControllerRef->CurrentSpell->GetSpellSlots(OutSlotsInRange, OutEmptySlots, OutAllies, OutEnemies);

		for (ASlotBase* SlotInRange : OutSlotsInRange)
		{
			if ((!SlotInRange->ContainsObstacle() && OverlaySlots.Contains(SlotInRange->Coordinates)) == false)
			{
				continue;
			}
			auto FindCoordSlot = *OverlaySlots.Find(SlotInRange->Coordinates);
			FindCoordSlot->SetSpellSlot();
			MovementOverlaySlotsInRange.Add(FindCoordSlot);
		}

		if (CurrentAreaTargetSpell->bHighlightEnemies)
		{
			HideDamageDetailsUI();
			HighlightedCharacters = OutEnemies;
			ShowDamageDetailsUIForAreaSpell();

			for (ACharacterBase* InHighlightedCharacter : HighlightedCharacters)
			{
				InHighlightedCharacter->Highlight(true);
			}
			return;
		}

		if (CurrentAreaTargetSpell->bHighlightAllies)
		{
			HighlightedCharacters = OutAllies;
			for (ACharacterBase* InHighlightedCharacter : HighlightedCharacters)
			{
				InHighlightedCharacter->Highlight(true);
			}
			return;
		}
	}
	else
	{
		if (MouseOverSlot.IsBound())
		{
			MouseOverSlot.Broadcast(InTargetSlot);
		}
	}
}

void AGrid::ShowAreaTargetAbilitySlots(ASlotBase* InTargetSlot)
{
	RemoveHighlightOnCharacters();
	HideOverlaySlots();

	if (InTargetSlot->ContainsObstacle())
	{
		return;
	}

	if (OnCursorChanged.IsBound())
	{
		OnCursorChanged.Broadcast(EBattleCursorType::Spell, BattleControllerRef->CurrentActiveAbility,
		                          nullptr, nullptr);
	}

	BattleControllerRef->CurrentActiveAbility->SetTargetSlot(InTargetSlot);

	TArray<ASlotBase*> OutSlotsInRange, OutEmptySlotsInRange;
	TArray<ACharacterBase*> OutAlliesInRange, OutEnemiesInRange;
	BattleControllerRef->CurrentActiveAbility->GetAbilitySlots(OutSlotsInRange, OutEmptySlotsInRange,
	                                                           OutAlliesInRange, OutEnemiesInRange);

	for (ASlotBase* SlotInRange : OutSlotsInRange)
	{
		if (!SlotInRange->ContainsObstacle() && OverlaySlots.Contains(SlotInRange->Coordinates))
		{
			ASlotBase* FindCoordinatesSlot = *OverlaySlots.Find(SlotInRange->Coordinates);
			FindCoordinatesSlot->SetSpellSlot();
			MovementOverlaySlotsInRange.Add(FindCoordinatesSlot);
		}
	}

	if (CurrentAreaTargetActiveAbility->bHighlightEnemies)
	{
		HideDamageDetailsUI();
		HighlightedCharacters = OutEnemiesInRange;
		ShowDamageDetailsUIForAreaAbility();
		for (ACharacterBase* InHighlightedCharacter : HighlightedCharacters)
		{
			InHighlightedCharacter->Highlight(true);
		}
		return;
	}

	if (CurrentAreaTargetActiveAbility->bHighlightAllies)
	{
		HighlightedCharacters = OutAlliesInRange;
		for (ACharacterBase* InHighlightedCharacter : HighlightedCharacters)
		{
			InHighlightedCharacter->Highlight(true);
		}
		return;
	}
}

void AGrid::SetSpellTarget(ASlotBase* InTargetSlot, ACharacterBase* InTargetCharacter)
{
	if (OnCursorChanged.IsBound())
	{
		OnCursorChanged.Broadcast(EBattleCursorType::Spell, nullptr,
		                          BattleControllerRef->CurrentSpell, nullptr);
	}

	BattleControllerRef->CurrentSpell->SetTargetSlot(InTargetSlot);

	HighlightedCharacters = TArray<ACharacterBase*>{InTargetCharacter};

	for (ACharacterBase* InHighlightedCharacter : HighlightedCharacters)
	{
		InHighlightedCharacter->Highlight(true);
	}
}

void AGrid::CastSpellOnSlot(ASlotBase* InTargetSlot)
{
	if (bAllyTargetSpellEnabled)
	{
		ACharacterBase* OutAllyCharacter;
		if (!InTargetSlot->IsAllySlot(BattleControllerRef->PlayingCharacter, OutAllyCharacter))
		{
			return;
		}
		BattleControllerRef->CurrentSpell->PlaySpellAnimation();
		return;
	}

	if (bEnemyTargetSpellEnabled)
	{
		ACharacterBase* OutEnemyCharacter;
		if (!InTargetSlot->IsEnemySlot(BattleControllerRef->PlayingCharacter, OutEnemyCharacter))
		{
			return;
		}
		BattleControllerRef->CurrentSpell->PlaySpellAnimation();
		return;
	}

	if (bAreaSpellEnabled && IsValid(InTargetSlot) && !InTargetSlot->ContainsObstacle())
	{
		BattleControllerRef->CurrentSpell->PlaySpellAnimation();
		return;
	}
	else
	{
		if (SlotClicked.IsBound())
		{
			SlotClicked.Broadcast(InTargetSlot);
		}
	}
}

void AGrid::StopDetectingClosestMovableSlot()
{
	DetectClosestMovableSlotTimeline.Stop();
}

int32 AGrid::UnitSizeAdjustmentAmount(int32 InUnitSize)
{
	//ToDo::결과연산을 봐야합니다.
	double Remainder;
	const int32 ModValue = UKismetMathLibrary::FMod(InUnitSize, 2, Remainder);
	return ModValue == 0 ? 1 : 0;
}

TArray<ASlotBase*> AGrid::GetNeighbourSlots(ASlotBase* InSlot, int32 InCharacterSize, bool OnlyEmpty)
{
	ASlotBase* CurrentSlot = InSlot;
	const int32 CurrentRange = InCharacterSize;
	bool CurrentOnlyEmpty = OnlyEmpty;

	TArray<ASlotBase*> CurrentNeighbourSlots;

	const int32 CeilRange = FMath::CeilToInt(CurrentRange / 2.0f);
	const int32 FirstIndex = -CeilRange;
	const int32 LastIndex = CeilRange + UnitSizeAdjustmentAmount(CurrentRange);

	for (int32 i = FirstIndex; i <= LastIndex; i++)
	{
		for (int32 j = FirstIndex; j <= LastIndex; j++)
		{
			const FVector2D NewCoord = FVector2D(j, i) + CurrentSlot->Coordinates;
			const int32 FloorRangeValue = FMath::Floor(CurrentRange / 2.0f);

			bool ConditionX = NewCoord.X > (CurrentSlot->Coordinates.X + FloorRangeValue) ||
				NewCoord.X < (CurrentSlot->Coordinates.X - FloorRangeValue) + UnitSizeAdjustmentAmount(CurrentRange);

			bool ConditionY = NewCoord.Y > (CurrentSlot->Coordinates.Y + FloorRangeValue) ||
				NewCoord.Y < ((CurrentSlot->Coordinates.Y - FloorRangeValue) + UnitSizeAdjustmentAmount(CurrentRange));

			if ((ConditionY || ConditionX) == false) { continue; }
			if (Slots.Contains(NewCoord) == false) { continue; }

			if (CurrentOnlyEmpty)
			{
				if ((*Slots.Find(NewCoord))->IsEmpty() == false) { continue; }
			}
			CurrentNeighbourSlots.Add(*Slots.Find(NewCoord));
		}
	}

	return CurrentNeighbourSlots;
}

int32 AGrid::GetDistanceCost(ASlotBase* Slot1, ASlotBase* Slot2)
{
	// 슬롯 간 거리 비용
	// 수평 및 수직 슬롯 = 10
	// 대각선 슬롯 = 14

	FVector2D CoordDistance = Slot1->Coordinates - Slot2->Coordinates;
	float DistanceX = FMath::Abs(CoordDistance.X);
	float DistanceY = FMath::Abs(CoordDistance.Y);

	if (DistanceX > DistanceY)
	{
		return FMath::TruncToInt(DistanceY * 14.0f + (10.0f * (DistanceX - DistanceY)));
	}

	return FMath::TruncToInt(DistanceX * 14.0f + (10.0f * (DistanceY - DistanceX)));
}

ASlotBase* AGrid::GetLowestTravelCostSlot(const TArray<ASlotBase*>& InSlots)
{
	int32 CurrentTotalCost = 999;
	ASlotBase* CurrentLowestCostSlot = nullptr;
	for (ASlotBase* InSlot : InSlots)
	{
		if (InSlot->TotalCost < CurrentTotalCost)
		{
			CurrentTotalCost = InSlot->TotalCost;
			CurrentLowestCostSlot = InSlot;
		}
	}
	return CurrentLowestCostSlot;
}

void AGrid::GetSlotsInRange(ASlotBase* InSlot, int32 InRange, bool OnlyEmptySlots, TArray<ASlotBase*>& OutSlotsInRange,
                            bool& OutAllFree)
{
	// 	범위 내 슬롯 반환
	// 범위 1은 선택한 슬롯을 반환합니다.
	// 범위 2는 4개의 슬롯을 반환합니다.
	// 범위 3은 9개의 슬롯을 반환합니다.

	bool CurrentAllEmpty = true;
	TArray<ASlotBase*> CurrentSlotsInRange;

	const float FloorRange = FMath::Floor(InRange / 2.0f);

	int32 FirstIndex = FloorRange - UnitSizeAdjustmentAmount(InRange);
	FirstIndex = -FirstIndex;
	const int32 LastIndex = FloorRange;

	for (int32 i = FirstIndex; i <= LastIndex; i++)
	{
		for (int32 j = FirstIndex; j <= LastIndex; j++)
		{
			FVector2D NewCoordinates = FVector2D(j, i) + InSlot->Coordinates;
			if (Slots.Contains(NewCoordinates))
			{
				ASlotBase* FindCoordSlot = *Slots.Find(NewCoordinates);
				if (FindCoordSlot->IsEmpty())
				{
					CurrentSlotsInRange.Add(FindCoordSlot);
				}
				else
				{
					CurrentAllEmpty = false;
				}
			}
			else
			{
				CurrentAllEmpty = false;
			}
		}
	}

	OutSlotsInRange = CurrentSlotsInRange;
	OutAllFree = CurrentAllEmpty;
}

ASlotBase* AGrid::GetClosestSlotInRange(ASlotBase* InSlot, const TArray<ASlotBase*>& InSlots)
{
	int32 CurrentDistance = 9999;
	ASlotBase* CurrentClosestSlot = nullptr;

	for (ASlotBase* SlotElement : InSlots)
	{
		const float SlotDistance = GetDistanceCost(SlotElement, InSlot);
		if (SlotDistance < CurrentDistance)
		{
			CurrentDistance = SlotDistance;
			CurrentClosestSlot = SlotElement;
		}
	}

	return CurrentClosestSlot;
}

TMap<FVector2D, ASlotBase*>& AGrid::GetAllOverlaySlots(int32 InUnitSize)
{
	//ToDo::결과연산을 봐야합니다.
	double Remainder;
	auto ModValue = UKismetMathLibrary::FMod(InUnitSize, 2, Remainder);
	return ModValue == 0 ? InnerOverlaySlots : OverlaySlots;
}

void AGrid::GetSlotsInCharacterRange(ACharacterBase* InCharacter, int32 InRange, ASlotBase* InStartingSlot,
                                     bool ExcludeSelf,
                                     TArray<ASlotBase*>& OutSlotsInRange, TArray<ASlotBase*>& OutEmptySlotsInRange,
                                     TArray<ACharacterBase*>& OutAlliesInRange,
                                     TArray<ACharacterBase*>& OutEnemiesInRange)
{
	auto CurrentCharacter = InCharacter;
	auto CurrentRange = InRange;
	auto CurrentStartingSlot = InStartingSlot;
	auto CurrentExcludeSelf = ExcludeSelf;

	TArray<ASlotBase*> CurrentSlotsInRange, currentEmptySlotsInRange;
	TArray<ACharacterBase*> CurrentAllies, CurrentEnemies;

	auto CeilCharacterUnitSize = FMath::CeilToInt(CurrentCharacter->UnitSize / 2.0f);

	int32 FirstIndex = -((CurrentRange - 1) + CeilCharacterUnitSize);
	int32 LastIndex = CeilCharacterUnitSize + UnitSizeAdjustmentAmount(CurrentCharacter->UnitSize);
	LastIndex = LastIndex + (CurrentRange - 1);

	for (int32 i = FirstIndex; i <= LastIndex; i++)
	{
		for (int32 j = FirstIndex; j <= LastIndex; j++)
		{
			auto NewCoord = CurrentStartingSlot->Coordinates + FVector2D(j, i);

			if (!Slots.Contains(NewCoord)) { continue; }
			ASlotBase* CurrentSlot = *Slots.Find(NewCoord);

			if (CurrentExcludeSelf)
			{
				if ((CurrentSlot->ContainedUnit == CurrentCharacter) == false) { continue; }
			}

			CurrentSlotsInRange.Add(CurrentSlot);

			if (CurrentSlot->IsEmpty())
			{
				currentEmptySlotsInRange.Add(CurrentSlot);
				continue;
			}

			ACharacterBase* OutEnemyCharacter;
			if (CurrentSlot->IsEnemySlot(CurrentCharacter, OutEnemyCharacter))
			{
				CurrentEnemies.AddUnique(OutEnemyCharacter);
				continue;
			}

			ACharacterBase* OutAllyCharacter;
			if (CurrentSlot->IsAllySlot(CurrentCharacter, OutAllyCharacter))
			{
				CurrentAllies.AddUnique(OutAllyCharacter);
				continue;
			}
		}
	}


	OutSlotsInRange = CurrentSlotsInRange;
	OutEmptySlotsInRange = currentEmptySlotsInRange;
	OutAlliesInRange = CurrentAllies;
	OutEnemiesInRange = CurrentEnemies;
}

void AGrid::RemoveEnemySpawnLocation()
{
	for (int i = EnemySpawnLocations.Num() - 1; i >= 0; --i)
	{
		EnemySpawnLocations[i]->Destroy();
	}
}

bool AGrid::SpawnCharacter(TSubclassOf<ACharacterBase> InUnitClass, FVector2D InCoordinates, bool IsAI, int32 InStack,
                           const FHeroStats& InHeroStats, ACharacterBase*& OutSpawnedCharacter)
{
	auto CurrentCharacter = InUnitClass;
	auto CurrentCoordinates = InCoordinates;
	bool CurrentIsAI = IsAI;
	int32 CurrentStack = InStack;
	FHeroStats CurrentHeroStats = InHeroStats;

	if (!Slots.Contains(CurrentCoordinates))
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("그 곳에 캐릭터를 생성 할 수 없습니다."));
		OutSpawnedCharacter = nullptr;
		return false;
	}

	ASlotBase* FindCoordSlot = *Slots.Find(CurrentCoordinates);
	ACharacterBase* CurrentCharacterCDO = CurrentCharacter->GetDefaultObject<ACharacterBase>();

	bool AllFree;
	TArray<ASlotBase*> OutSlotsInRange;
	GetSlotsInRange(FindCoordSlot, CurrentCharacterCDO->UnitSize, true, OutSlotsInRange, AllFree);

	if (!AllFree)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("그 곳에 캐릭터를 생성 할 수 없습니다."));
		OutSpawnedCharacter = nullptr;
		return false;
	}

	UWorld* World = GetWorld();
	SRPG_CHECK(World, false);

	auto FindCoordAllSlots = *GetAllSlots(CurrentCharacterCDO->UnitSize).Find(CurrentCoordinates);

	auto SpawnRotation = CurrentIsAI ? FRotator(0.0f, 180.0f, 0.0f) : FRotator::ZeroRotator;
	FTransform SpawnTransform = FTransform(SpawnRotation, FindCoordAllSlots->GetActorTransform().GetLocation());

	ACharacterBase* NewCharacter = World->SpawnActorDeferred<ACharacterBase>(CurrentCharacter, SpawnTransform);
	if (NewCharacter)
	{
		NewCharacter->bIsAI = CurrentIsAI;
		NewCharacter->Stack = CurrentStack;
		NewCharacter->BattleController = BattleControllerRef;
		NewCharacter->HeroStats = CurrentHeroStats;
		NewCharacter->Slot = FindCoordSlot;
		NewCharacter->Grid = this;
		UGameplayStatics::FinishSpawningActor(NewCharacter, SpawnTransform);
	}

	SetUnitOnSlot(NewCharacter, FindCoordSlot);
	OutSpawnedCharacter = NewCharacter;
	return true;
}

void AGrid::RemoveUnitOnSlot(ACharacterBase* InUnit)
{
	if (!IsValid(InUnit)) { return; }

	bool AllFree;
	TArray<ASlotBase*> OutSlotsInRage;
	GetSlotsInRange(InUnit->Slot, InUnit->UnitSize, false, OutSlotsInRage, AllFree);

	for (ASlotBase* SlotsInRage : OutSlotsInRage)
	{
		SlotsInRage->SetUnit(nullptr);
	}

	InUnit->OnRemovedFromSlot();
}

TArray<ASlotBase*> AGrid::GetSlotsForObstacleSpawn()
{
	TArray<ASlotBase*> CurrentAvailableSlots;
	for (TTuple<UE::Math::TVector2<double>, ASlotBase*> InSlotPair : Slots)
	{
		ASlotBase* InSlot = InSlotPair.Value;
		if (!InSlot->bCanPlayerCharacterSpawn && InSlot->IsEmpty())
		{
			CurrentAvailableSlots.Add(InSlot);
		}
	}
	return CurrentAvailableSlots;
}

float AGrid::GetDistanceBetweenCoordinates(FVector2D Coordinate1, FVector2D Coordinate2)
{
	auto DistanceCoordinate = (Coordinate1 - Coordinate2);
	return FMath::Abs(DistanceCoordinate.X) + FMath::Abs(DistanceCoordinate.Y);
}

bool AGrid::SpawnObstacle(TSubclassOf<AObstacleBase> InObstacle, FVector2D InCoordinates,
                          AObstacleBase*& OutSpawnedObstacle)
{
	auto CurrentObstacle = InObstacle;
	auto CurrentCoordinates = InCoordinates;

	if (!Slots.Contains(CurrentCoordinates))
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("이 곳에 장애물을 생성할 수 없습니다"));
		OutSpawnedObstacle = nullptr;
		return false;
	}

	auto FindCoordSlot = *Slots.Find(CurrentCoordinates);
	auto CurrentObstacleCDO = CurrentObstacle->GetDefaultObject<AObstacleBase>();

	bool AllFree;
	TArray<ASlotBase*> OutSlotsInRange;
	GetSlotsInRange(FindCoordSlot, CurrentObstacleCDO->UnitSize, true, OutSlotsInRange, AllFree);

	if (!AllFree)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("이 곳에 장애물을 생성할 수 없습니다"));
		OutSpawnedObstacle = nullptr;
		return false;
	}

	UWorld* World = GetWorld();
	SRPG_CHECK(World, false);


	auto AllSlots = GetAllSlots(CurrentObstacleCDO->UnitSize);
	auto NewLocation = (*AllSlots.Find(CurrentCoordinates))->GetActorTransform().GetLocation();

	float NewYaw = CurrentObstacleCDO->bRandomRotation ? UKismetMathLibrary::RandomFloatInRange(0.0f, 359.0f) : 0.0f;

	FTransform SpawnTransform = FTransform(FRotator(0.0f, NewYaw, 0.0f), NewLocation);
	AObstacleBase* NewObstacle = World->SpawnActorDeferred<AObstacleBase>(CurrentObstacle, SpawnTransform);
	if (NewObstacle)
	{
		NewObstacle->Slot = FindCoordSlot;
		NewObstacle->Grid = this;
		UGameplayStatics::FinishSpawningActor(NewObstacle, SpawnTransform);
	}

	SetUnitOnSlot(NewObstacle, FindCoordSlot);
	OutSpawnedObstacle = NewObstacle;
	return true;
}

void AGrid::SetUnitOnSlot(AUnitBase* InUnit, ASlotBase* InSlot)
{
	AUnitBase* CurrentUnit = InUnit;
	ASlotBase* CurrentSlot = InSlot;

	if (!IsValid(CurrentUnit)) { return; }

	bool AllFree;
	TArray<ASlotBase*> OutSlotsInRange;
	GetSlotsInRange(CurrentSlot, CurrentUnit->UnitSize, false, OutSlotsInRange, AllFree);

	for (ASlotBase* InRangeSlot : OutSlotsInRange)
	{
		InRangeSlot->SetUnit(CurrentUnit);
		if (CurrentUnit->bInitialized)
		{
			InRangeSlot->ApplyAuraToContainedUnit();
		}
	}
	CurrentUnit->OnMovedToSlot();
}

TMap<FVector2D, ASlotBase*>& AGrid::GetAllSlots(int32 InUnitSize)
{
	//ToDo: 결과연산을 봐야합니다.
	double Remainder;
	auto ModValue = UKismetMathLibrary::FMod(InUnitSize, 2, Remainder);

	return ModValue == 0 ? InnerSlots : Slots;
}

bool AGrid::CanUnitFitTo(ASlotBase* InSlot, AUnitBase* InUnit, TArray<ASlotBase*>& OutAllSlotsUnitWillStandOn)
{
	auto CurrentSlot = InSlot;
	auto CurrentUnit = InUnit;
	bool CurrentCanUnitFit = true;

	TArray<ASlotBase*> CurrentAllSlotsUnitWillStandOn;

	int32 LastIndex = FMath::Floor(CurrentUnit->UnitSize / 2.0f);
	int32 FirstIndex = LastIndex - UnitSizeAdjustmentAmount(CurrentUnit->UnitSize);
	FirstIndex = -FirstIndex;

	for (int32 i = FirstIndex; i <= LastIndex; i++)
	{
		for (int32 j = FirstIndex; j <= LastIndex; j++)
		{
			FVector2D NewCoord = FVector2D(j, i);

			if (Slots.Contains(NewCoord + CurrentSlot->Coordinates))
			{
				auto CoordFindSlot = *Slots.Find(NewCoord + CurrentSlot->Coordinates);

				if (CoordFindSlot->IsEmpty() || CurrentUnit == CoordFindSlot->ContainedUnit)
				{
					CurrentAllSlotsUnitWillStandOn.Add(CoordFindSlot);
				}
				else
				{
					CurrentCanUnitFit = false;
				}
			}
			else
			{
				CurrentCanUnitFit = false;
			}
		}
	}

	OutAllSlotsUnitWillStandOn = CurrentAllSlotsUnitWillStandOn;
	return CurrentCanUnitFit;
}

float AGrid::GetDistanceToSlot(ASlotBase* Slot1, ASlotBase* Slot2)
{
	FVector2D CoordDistance = Slot1->Coordinates - Slot2->Coordinates;
	float DistanceX = FMath::Abs(CoordDistance.X);
	float DistanceY = FMath::Abs(CoordDistance.Y);

	if (DistanceX > DistanceY)
	{
		return FMath::TruncToInt(DistanceY + (DistanceX - DistanceY));
	}

	return FMath::TruncToInt(DistanceX + (DistanceY - DistanceX));
}

void AGrid::GetAllCharacterSlots(TArray<ASlotBase*>& OutAllySlots, TArray<ASlotBase*>& OutEnemySlots)
{
	TArray<ASlotBase*> CurrentAllySlots, CurrentEnemySlots;
	for (ACharacterBase* InPlayedCharacter : BattleControllerRef->PlayedCharacters)
	{
		bool AllFree;
		TArray<ASlotBase*> OutSlotsInRange;
		GetSlotsInRange(InPlayedCharacter->Slot, InPlayedCharacter->UnitSize, false, OutSlotsInRange, AllFree);
		for (ASlotBase* InSlot : OutSlotsInRange)
		{
			CurrentAllySlots.AddUnique(InSlot);
		}
	}

	for (ACharacterBase* InEnemyCharacter : BattleControllerRef->EnemyCharacters)
	{
		bool AllFree;
		TArray<ASlotBase*> OutSlotsInRange;
		GetSlotsInRange(InEnemyCharacter->Slot, InEnemyCharacter->UnitSize, false, OutSlotsInRange, AllFree);
		for (ASlotBase* InSlot : OutSlotsInRange)
		{
			CurrentEnemySlots.AddUnique(InSlot);
		}
	}

	OutAllySlots = CurrentAllySlots;
	OutEnemySlots = CurrentEnemySlots;
}

ASlotBase* AGrid::GetClosestMovableSlot(ASlotBase* InSlot, int32 InUnitSize, const TArray<ASlotBase*>& InMovableSlots)
{
	ASlotBase* CurrentSlot = InSlot;
	int32 CurrentUnitSize = InUnitSize;
	float CurrentDistance = 99999.0f;
	ASlotBase* CurrentClosestMovableSlot = nullptr;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	for (ASlotBase* InMovableSlot : InMovableSlots)
	{
		FVector WorldLocation, WorldDirection;
		PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

		FVector Start = WorldLocation;
		FVector End = WorldLocation + (WorldDirection * 10000.0f);
		TArray<AActor*> ActorsToIgnore;
		TArray<FHitResult> OutHits;
		bool IsHit = UKismetSystemLibrary::LineTraceMulti(this, Start, End,
		                                                  USRPGTypes::ConvertToTraceTypeVisibility(),
		                                                  false, ActorsToIgnore,
		                                                  EDrawDebugTrace::None, OutHits, true);

		if (!IsHit) { continue; }

		for (const FHitResult& OutHit : OutHits)
		{
			if (CurrentSlot != OutHit.GetActor()) { continue; }


			auto UnitSizeAllSlots = GetAllSlots(CurrentUnitSize);
			if (!UnitSizeAllSlots.Contains(InMovableSlot->Coordinates)) { continue; }

			auto FindCoordSlot = *UnitSizeAllSlots.Find(InMovableSlot->Coordinates);

			auto DistanceValue =
				UKismetMathLibrary::Vector_Distance(OutHit.Location, FindCoordSlot->GetActorLocation());

			if (DistanceValue < CurrentDistance)
			{
				CurrentDistance = DistanceValue;
				CurrentClosestMovableSlot = FindCoordSlot;
			}
		}
	}

	return CurrentClosestMovableSlot;
}

ASlotBase* AGrid::GetClosestSlotToMouse(ASlotBase* InSlotToAttack, const TArray<ASlotBase*>& InMovableSlots)
{
	float CurrentDistance = 99999.0f;
	ASlotBase* CurrentSlotToAttack = InSlotToAttack;

	for (ASlotBase* InMovableSlot : InMovableSlots)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		float DistanceValue = FVector::Distance(InMovableSlot->GetActorLocation(),
		                                        PlayerController->PlayerCameraManager->GetCameraLocation());

		FVector WorldLocation, WorldDirection;
		PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

		double MouseToSlotDistance = UKismetMathLibrary::VSize(
			InMovableSlot->GetActorLocation() - (WorldLocation + (WorldDirection * DistanceValue)));

		FVector2D NewCoord = InMovableSlot->Coordinates - CurrentSlotToAttack->Coordinates;
		auto SelectCoordValue = FMath::Min(FMath::Abs(NewCoord.X), FMath::Abs(NewCoord.Y)) > 0.0f ? 1.4f : 1.0f;

		if (MouseToSlotDistance / SelectCoordValue < CurrentDistance)
		{
			ASlotBase* CurrentClosestSlot = nullptr;
			CurrentDistance = MouseToSlotDistance / SelectCoordValue;
			CurrentClosestSlot = InMovableSlot;
		}
	}

	return CurrentSlotToAttack;
}

ASlotBase* AGrid::GetClosestSlotToMouseImproved(ASlotBase* InSlotToAttack, const TArray<ASlotBase*>& InMovableSlots)
{
	float CurrentDistance = 99999.0f;

	ASlotBase* CurrentClosestSlot = nullptr;

	AUnitBase* CurrentCharacterToAttack = InSlotToAttack->ContainedUnit;
	auto UnitSizeAllSlots = GetAllSlots(CurrentCharacterToAttack->UnitSize);
	ASlotBase* CurrentSlotToAttack = *UnitSizeAllSlots.Find(InSlotToAttack->ContainedUnit->Slot->Coordinates);

	auto AllSlots = GetAllSlots(BattleControllerRef->PlayingCharacter->UnitSize);
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	for (ASlotBase* InMovableSlot : InMovableSlots)
	{
		if (!AllSlots.Contains(InMovableSlot->Coordinates)) { continue; }
		ASlotBase* CurrentSlot = *AllSlots.Find(InMovableSlot->Coordinates);

		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		FVector Start = MouseWorldLocation;
		FVector End = Start + (MouseWorldDirection * 10000.0f);
		TArray<AActor*> ActorsToIgnore;
		TArray<FHitResult> OutHits;
		const bool IsHit = UKismetSystemLibrary::LineTraceMulti(this, Start, End,
		                                                        USRPGTypes::ConvertToTraceTypeVisibility(),
		                                                        false, ActorsToIgnore,
		                                                        EDrawDebugTrace::None, OutHits, true);

		if (!IsHit) { continue; }


		for (const FHitResult& OutHit : OutHits)
		{
			ASlotBase* HitSlot = Cast<ASlotBase>(OutHit.GetActor());
			if (!HitSlot) { continue; }

			float DirectionToAttackSlotYaw = UKismetMathLibrary::FindLookAtRotation(CurrentSlot->GetActorLocation(),
				CurrentSlotToAttack->GetActorLocation()).Yaw;

			bool LeftDir = UKismetMathLibrary::InRange_FloatFloat(DirectionToAttackSlotYaw, -135.0f, -45.0f, false,
			                                                      true);
			bool RightDir =
				UKismetMathLibrary::InRange_FloatFloat(DirectionToAttackSlotYaw, 45.0f, 135.0f, false, true);

			float DirValue = LeftDir || RightDir
				                 ? UKismetMathLibrary::DegSin(DirectionToAttackSlotYaw)
				                 : UKismetMathLibrary::DegCos(DirectionToAttackSlotYaw);

			auto AttackRangeEstimate = (CurrentCharacterToAttack->UnitSize *
				CurrentSlotToAttack->SlotSetting.SlotSize.X) / DirValue;

			auto SlotDir = CurrentSlot->GetActorLocation() - CurrentSlotToAttack->GetActorLocation();
			auto SlotDirVSize = UKismetMathLibrary::VSize(SlotDir);

			FVector DirRangeValue = SlotDir * (AttackRangeEstimate / SlotDirVSize);

			FVector SelectDirValue = DirValue < 0.0f
				                         ? CurrentSlotToAttack->GetActorLocation() - DirRangeValue
				                         : CurrentSlotToAttack->GetActorLocation() + DirRangeValue;

			double DirVSize = UKismetMathLibrary::VSize(OutHit.Location - SelectDirValue);

			if (DirVSize < CurrentDistance)
			{
				CurrentDistance = DirVSize;
				CurrentClosestSlot = CurrentSlot;
			}
		}
	}

	return CurrentClosestSlot;
}

bool AGrid::IsEnemyAdjacent(ACharacterBase* InCharacter)
{
	auto NeighbourSlots = GetNeighbourSlots(InCharacter->Slot, InCharacter->UnitSize, false);
	for (ASlotBase* InNeighbourSlot : NeighbourSlots)
	{
		ACharacterBase* OutEnemyCharacter;
		if (InNeighbourSlot->IsEnemySlot(InCharacter, OutEnemyCharacter))
		{
			return true;
		}
	}
	return false;
}

TArray<ASlotBase*> AGrid::GetAttackSlots(ACharacterBase* InAttackingCharacter, ACharacterBase* InTargetCharacter)
{
	auto CurrentAttackingCharacter = InAttackingCharacter;
	auto CurrentTargetCharacter = InTargetCharacter;

	TArray<ASlotBase*> CurrentNeighbourSlots;

	int32 TargetUnitSize = FMath::CeilToInt(CurrentTargetCharacter->UnitSize / 2.0f);
	int32 AttackingUnitSize = FMath::Floor(CurrentAttackingCharacter->UnitSize / 2.0f);

	int32 FirstIndex = -(TargetUnitSize + AttackingUnitSize);
	int32 LastIndex = TargetUnitSize + UnitSizeAdjustmentAmount(CurrentTargetCharacter->UnitSize);
	LastIndex = LastIndex + (AttackingUnitSize - UnitSizeAdjustmentAmount(CurrentAttackingCharacter->UnitSize));

	for (int32 i = FirstIndex; i <= LastIndex; i++)
	{
		for (int32 j = FirstIndex; i <= LastIndex; j++)
		{
			FVector2D NewCoord(j, i);
			NewCoord += CurrentTargetCharacter->Slot->Coordinates;


			const float FloorTargetUnitSize = FMath::Floor(CurrentTargetCharacter->UnitSize / 2.0f);

			double TargetX1 = (CurrentTargetCharacter->Slot->Coordinates.X - FloorTargetUnitSize) +
				UnitSizeAdjustmentAmount(CurrentTargetCharacter->UnitSize);
			double TargetX2 = (CurrentTargetCharacter->Slot->Coordinates.X + FloorTargetUnitSize);

			bool ConditionX = NewCoord.X < TargetX1 || NewCoord.X > TargetX2;

			double TargetY1 = (CurrentTargetCharacter->Slot->Coordinates.Y - FloorTargetUnitSize) +
				UnitSizeAdjustmentAmount(CurrentTargetCharacter->UnitSize);
			double TargetY2 = (CurrentTargetCharacter->Slot->Coordinates.Y + FloorTargetUnitSize);

			bool ConditionY = NewCoord.Y < TargetY1 || NewCoord.X > TargetY2;

			if ((ConditionX || ConditionY) == false) { continue; }

			if (!Slots.Contains(NewCoord)) { continue; }

			auto FindCoordSlot = *Slots.Find(NewCoord);
			if (FindCoordSlot->IsEmpty() == false) { continue; }

			TArray<ASlotBase*> OutAllSlotsUnitWillStandOn;
			bool CanUnitFit = CanUnitFitTo(FindCoordSlot, CurrentAttackingCharacter, OutAllSlotsUnitWillStandOn);

			if (!CanUnitFit) { continue; }

			CurrentNeighbourSlots.Add(FindCoordSlot);
		}
	}

	return CurrentNeighbourSlots;
}


//#2558
