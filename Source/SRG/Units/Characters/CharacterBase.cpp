#include "CharacterBase.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MeleeCharacters/MeleeCharacterBase.h"
#include "SRG/Abilities/ActiveAbilities/ActiveAbilityBase.h"
#include "SRG/Abilities/Auras/AuraBase.h"
#include "SRG/Abilities/MultiTargetPassiveAbilities/MultiTargetPassiveAbilityBase.h"
#include "SRG/Abilities/PassiveAbilities/PassiveAbilityBase.h"
#include "SRG/Core/BattleController.h"
#include "SRG/Gird/Grid.h"
#include "SRG/Misc/Damage.h"
#include "SRG/StatusEffects/DeBuffs/DeBuffBase.h"
#include "SRG/Widgets/UW_CharacterHealth.h"
#include "SRGCore/AssetTableRef.h"
#include "SRGCore/SRGLog.h"
#include "SRGCore/SRPGUtilityLibrary.h"

ACharacterBase::ACharacterBase()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다.
	// 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	ProjectileHitLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileHitLocation"));
	ProjectileHitLocation->SetRelativeLocation(FVector(0.0f, 0.000092f, 127.973557f));
	ProjectileHitLocation->SetupAttachment(GetRootComponent());

	// Create the spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 40.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->SetupAttachment(GetRootComponent());

	// Create the widget component
	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidget->SetDrawSize(FVector2D(1.0f, 1.0f)); // Assuming the desired size is 1x1
	HealthWidget->SetDrawAtDesiredSize(true);

	// 상호작용 UI 설정
	if (TSubclassOf<UUW_CharacterHealth> WBP_CharacterHealthClass =
		DT::FindClass<UUW_CharacterHealth>(DT_BLUEPRINT_PATH,TEXT("WBP_CharacterHealth")))
	{
		HealthWidget->SetWidgetClass(WBP_CharacterHealthClass);
	}
	HealthWidget->SetupAttachment(SpringArm);
}


void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	SRPG_CHECK(PlayerController);

	InitializeMoveOverTimeTimeline();
	InitializeRotateOverTimeTimeline();
}

void ACharacterBase::InitializeMoveOverTimeTimeline()
{
	// Create a curve object
	MoveOverTimeCurve = NewObject<UCurveFloat>(this);
	if (MoveOverTimeCurve)
	{
		// Add keys to the curve
		MoveOverTimeCurve->FloatCurve.AddKey(0.0f, 0.0f);
		MoveOverTimeCurve->FloatCurve.AddKey(1.0f, 1.0f);

		// Bind the timeline to the curve
		FOnTimelineFloat MoveTimelineUpdate;
		MoveTimelineUpdate.BindUFunction(this, FName("HandleMoveOverTime_Update"));
		MoveOverTimeLine.AddInterpFloat(MoveOverTimeCurve, MoveTimelineUpdate);

		FOnTimelineEvent MoveTimelineFinished;
		MoveTimelineFinished.BindUFunction(this, FName("HandleMoveOverTime_Finished"));
		MoveOverTimeLine.SetTimelineFinishedFunc(MoveTimelineFinished);

		MoveOverTimeLine.SetTimelineLength(1.0f); // Set the desired timeline length
		MoveOverTimeLine.SetLooping(false); // Set whether to repeat the timeline
	}
}

void ACharacterBase::InitializeRotateOverTimeTimeline()
{
	// Create a curve object
	RotateOverTimeCurve = NewObject<UCurveFloat>(this);
	if (RotateOverTimeCurve)
	{
		// Add keys to the curve
		RotateOverTimeCurve->FloatCurve.AddKey(0.0f, 0.0f);
		RotateOverTimeCurve->FloatCurve.AddKey(1.0f, 1.0f);

		// Bind the timeline to the curve
		FOnTimelineFloat RotateTimelineUpdate;
		RotateTimelineUpdate.BindUFunction(this, FName("HandleRotateOverTime_Update"));
		RotateOverTimeLine.AddInterpFloat(RotateOverTimeCurve, RotateTimelineUpdate);

		FOnTimelineEvent RotateTimelineFinished;
		RotateTimelineFinished.BindUFunction(this, FName("HandleRotateOverTime_Finished"));
		RotateOverTimeLine.SetTimelineFinishedFunc(RotateTimelineFinished);

		RotateOverTimeLine.SetTimelineLength(1.0f); // Set the desired timeline length
		RotateOverTimeLine.SetLooping(false); // Set whether to repeat the timeline
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveOverTimeLine.TickTimeline(DeltaTime);
	RotateOverTimeLine.TickTimeline(DeltaTime);

	if (PlayerController)
		SpringArm->SetWorldRotation(FRotator(0.0f, PlayerController->GetControlRotation().Yaw, 0.0f));
}

void ACharacterBase::InitializeEvent()
{
	Super::InitializeEvent();

	// BattleController가 유효한지 확인하십시오.
	if (!IsValid(BattleController))
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("BattleController가 유효하지 않습니다"));
		return;
	}

	// BattleController의 OnNewTurn 이벤트에 바인딩
	BattleController->OnNewTurn.AddDynamic(this, &ACharacterBase::OnNewTurnHandler);

	SetStartingStack();

	SetStats();

	SetAbilities();

	ApplyAuras();

	ReceiveAuras();

	ResetRetaliation(MaxRetaliationCount);

	if (bIsBoss)
	{
		HealthWidget->DestroyComponent();
	}

	HealthUI = Cast<UUW_CharacterHealth>(HealthWidget->GetUserWidgetObject());
	if (!HealthUI)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("HealthUI로 형변환에 실패했습니다."));
		return;
	}

	HealthUI->InitializeEvent(this);
}

void ACharacterBase::Highlight(bool IsOn)
{
	// 캐릭터의 하이라이트 상태에 따라 스켈레탈 메시의 커스텀 뎁스와 스텐실 값을 설정합니다.
	SkeletalMesh->SetRenderCustomDepth(IsOn);
	SkeletalMesh->SetCustomDepthStencilValue(bIsAI ? 2 : 1);

	// OnCharacterHighlighted 이벤트가 바인딩되어 있는 경우 해당 이벤트를 브로드캐스트합니다.
	if (OnCharacterHighlighted.IsBound())
	{
		OnCharacterHighlighted.Broadcast(IsOn);
	}
}

void ACharacterBase::OnMovedToSlot()
{
	ApplyAuras();

	// OnCharacterMoveToSlot 이벤트가 바인딩되어 있는 경우 해당 이벤트를 브로드캐스트합니다.
	if (OnCharacterMoveToSlot.IsBound())
	{
		OnCharacterMoveToSlot.Broadcast(this);
	}
}

void ACharacterBase::OnRemovedFromSlot()
{
	// OnCharacterRemovedFromSlot 이벤트가 바인딩되어 있는 경우 해당 이벤트를 브로드캐스트합니다.
	if (OnCharacterRemovedFromSlot.IsBound())
	{
		OnCharacterRemovedFromSlot.Broadcast(this);
	}

	// OnRefreshAuras 이벤트가 바인딩되어 있는 경우 해당 이벤트를 브로드캐스트합니다.
	if (OnRefreshAuras.IsBound())
	{
		OnRefreshAuras.Broadcast();
	}
}

void ACharacterBase::SetStartingStack()
{
	// 현재 스택 값을 시작 스택으로 설정합니다.
	StartingStack = Stack;
}

void ACharacterBase::ResetRetaliation(int32 NewRetaliationCount)
{
	// 새로운 보복 횟수로 현재 보복 횟수를 재설정합니다.
	CurrentRetaliationCount = NewRetaliationCount;
}

void ACharacterBase::SetStats()
{
	// CharacterStats와 HeroStats를 더하여 최종 스탯을 설정합니다.
	MainStats = CharacterStats + HeroStats;

	// 최대 체력과 마나를 설정합니다.
	SetHealth(MainStats.MaxHealth, false);
	SetMana(MainStats.MaxMana);

	// 공격력, 방어력, 최소/최대 데미지, 탄환 수, 이동 범위를 설정합니다.
	CurrentAttack = MainStats.Attack;
	CurrentDefense = MainStats.Defense;
	CurrentMinDamage = MainStats.MinDamage;
	CurrentMaxDamage = MainStats.MaxDamage;
	CurrentAmmo = MainStats.Ammo;
	CurrentMovementRange = MainStats.MovementRange;

	// Stats가 업데이트되었음을 알리는 이벤트를 브로드캐스트합니다.
	if (OnStatsUpdated.IsBound())
		OnStatsUpdated.Broadcast();
}

void ACharacterBase::SetAbilities()
{
	if (IsValid(MultiTargetPassiveAbility))
	{
		if (UWorld* World = GetWorld())
		{
			CurrentMultiTargetPassiveAbility =
				World->SpawnActorDeferred<AMultiTargetPassiveAbilityBase>(MultiTargetPassiveAbility, FTransform());

			if (CurrentMultiTargetPassiveAbility)
			{
				CurrentMultiTargetPassiveAbility->AbilityOwner = this;
				CurrentMultiTargetPassiveAbility->BattleController = BattleController;
				UGameplayStatics::FinishSpawningActor(CurrentMultiTargetPassiveAbility, FTransform());
			}
		}
	}

	for (TSubclassOf<AActiveAbilityBase> ActiveAbility : ActiveAbilities)
	{
		if (UWorld* World = GetWorld())
		{
			if (AActiveAbilityBase* NewActiveAbility =
				World->SpawnActorDeferred<AActiveAbilityBase>(ActiveAbility, FTransform()))
			{
				NewActiveAbility->AbilityOwner = this;
				NewActiveAbility->BattleController = BattleController;
				UGameplayStatics::FinishSpawningActor(NewActiveAbility, FTransform());
				CurrentActiveAbilities.Add(NewActiveAbility);
			}
		}
	}

	for (TSubclassOf<APassiveAbilityBase> PassiveAbility : PassiveAbilities)
	{
		if (UWorld* World = GetWorld())
		{
			if (APassiveAbilityBase* NewPassiveAbility =
				World->SpawnActorDeferred<APassiveAbilityBase>(PassiveAbility, FTransform()))
			{
				NewPassiveAbility->AbilityOwner = this;
				NewPassiveAbility->BattleController = BattleController;
				UGameplayStatics::FinishSpawningActor(NewPassiveAbility, FTransform());
				CurrentPassiveAbilities.Add(NewPassiveAbility);
			}
		}
	}

	for (TSubclassOf<AAuraBase> AuraClass : Auras)
	{
		if (UWorld* World = GetWorld())
		{
			if (AAuraBase* NewAura = World->SpawnActorDeferred<AAuraBase>(AuraClass, FTransform()))
			{
				NewAura->AbilityOwner = this;
				NewAura->BattleController = BattleController;
				UGameplayStatics::FinishSpawningActor(NewAura, FTransform());
				NewAura->AddBuffsToCharacter(this);
				CurrentAuras.Add(NewAura);
			}
		}
	}
}

void ACharacterBase::ApplyStats(const FCharacterStats& InApplyStat)
{
	CurrentAttack += InApplyStat.Attack;
	CurrentDefense += InApplyStat.Defense;
	CurrentMinDamage += InApplyStat.MinDamage;
	CurrentMaxDamage += InApplyStat.MaxDamage;
	CurrentAmmo += InApplyStat.Ammo;
	CurrentMovementRange += InApplyStat.MovementRange;

	if (OnStatsUpdated.IsBound())
	{
		OnStatsUpdated.Broadcast();
	}
}

void ACharacterBase::ApplyElementReductions(const FElementReductions& InElementReductions)
{
	CurrentNormalReduction += InElementReductions.Normal;
	CurrentFireReduction += InElementReductions.Fire;
	CurrentEarthReduction += InElementReductions.Earth;
	CurrentAirReduction += InElementReductions.Air;
	CurrentWaterReduction += InElementReductions.Water;
}


void ACharacterBase::ApplyAuras()
{
	for (AAuraBase* CurrentAura : CurrentAuras)
	{
		if (IsValid(CurrentAura))
		{
			CurrentAura->ApplyAura();
		}
	}
}

void ACharacterBase::ReceiveAuras()
{
	TArray<ASlotBase*> SlotsInRange;
	bool AllFree;
	BattleController->GetGrid()->GetSlotsInRange(Slot, UnitSize, false, SlotsInRange, AllFree);

	for (ASlotBase* SlotInRange : SlotsInRange)
	{
		if (IsValid(SlotInRange))
		{
			SlotInRange->ApplyAuraToContainedUnit();
		}
	}
}

void ACharacterBase::SetHealth(int32 NewHealth, bool IsHeal)
{
	CurrentHealth = NewHealth;

	// 현재 체력과 치유 여부를 브로드캐스트합니다.
	if (OnHealthSet.IsBound())
	{
		OnHealthSet.Broadcast(CurrentHealth, IsHeal);
	}


	// 캐릭터가 사망한 경우 사망 처리를 수행합니다.
	if (IsDead())
	{
		// 캐릭터 사망 이벤트를 브로드캐스트합니다.
		if (OnDead.IsBound())
		{
			OnDead.Broadcast(this);
		}
		// 사망 처리를 수행합니다.
		Die();
	}
}

void ACharacterBase::SetMana(int32 NewMana)
{
	CurrentMana = NewMana;

	if (OnManaSet.IsBound())
	{
		// 현재 마나 값을 브로드캐스트합니다.
		OnManaSet.Broadcast(CurrentMana);
	}
}

void ACharacterBase::Heal(int32 InAmount)
{
	ShowDamage(0, 0, InAmount, EElement::Normal, false);
	const int32 NewHealth = FMath::Clamp(InAmount + CurrentHealth, 0, MainStats.MaxHealth);
	SetHealth(NewHealth, true);
}

void ACharacterBase::IncreaseMana(int32 InAmount)
{
	const int32 NewMana = FMath::Clamp(InAmount + CurrentMana, 0, MainStats.MaxMana);
	SetMana(NewMana);
}

void ACharacterBase::ReduceMana(int32 InAmount)
{
	const int32 NewMana = FMath::Clamp(CurrentMana - InAmount, 0, MainStats.MaxMana);
	SetMana(NewMana);
}

void ACharacterBase::Resurrect(int32 InStackAmount)
{
	// 부활 후 최소 스택 값 계산
	const int32 MinStack = FMath::Min(InStackAmount + Stack, StartingStack);

	// 스택 차이로 인한 데미지 표시(부활 스택 - 이전 스택)
	const int32 DamageStack = MinStack - Stack;
	ShowDamage(DamageStack, 0, 0, EElement::Normal, true);

	// 부활 후 스택 값을 최소 스택으로 업데이트
	Stack = MinStack;

	// OnResurrected 이벤트가 바인딩되었는지 확인하고 브로드캐스트합니다.
	if (OnResurrected.IsBound())
	{
		OnResurrected.Broadcast();
	}
}

void ACharacterBase::TakeDamageEvent(int32 InDamage, EElement InElement)
{
	int32 DiedStack, RemainingHealth;
	CalculateDyingStack(InDamage, DiedStack, RemainingHealth); // 데미지를 받은 후 사망한 스택과 남은 체력 계산.

	Stack -= DiedStack; // 사망한 스택만큼 현재 스택에서 차감.

	ShowDamage(DiedStack, InDamage, 0, InElement, false); // 시각적으로 데미지를 표시.

	if (InDamage <= 0 || IsDead())
		return; // 만약 데미지가 0 이하이거나 이미 사망한 상태라면, 함수 실행 종료.

	SetHealth(RemainingHealth, false); // 캐릭터의 체력을 남은 체력으로 설정.

	UAnimMontage* SelectPlayMontage = (bIsDefending && IsValid(DefendMontage)) ? DefendMontage : GetHitMontage;
	PlayAnimationMontage(SelectPlayMontage); // 방어 상태에 따라 적절한 몽타주를 재생.

	if (OnDamageTaken.IsBound())
		OnDamageTaken.Broadcast(); // 데미지를 받은 이벤트를 브로드캐스트.
}

void ACharacterBase::ShowDamage(int32 InStack, int32 InDamage, int32 InHeal, EElement InElement, bool IsResurrect)
{
	// 현재 월드를 가져옵니다.
	if (UWorld* World = GetWorld())
	{
		// 데미지 효과 생성 위치를 설정합니다.
		const FTransform SpawnTransform = FTransform(ProjectileHitLocation->GetComponentLocation());

		// ADamage 액터를 생성합니다.
		if (ADamage* NewDamage = World->SpawnActorDeferred<ADamage>(ADamage::StaticClass(), SpawnTransform))
		{
			// 데미지 효과에 값들을 설정합니다.
			NewDamage->Stack = InStack; // 스택 값 설정
			NewDamage->Damage = InDamage; // 데미지 값 설정
			NewDamage->Heal = InHeal; // 치유 값 설정
			NewDamage->Element = InElement; // 요소 값 설정
			NewDamage->bIsAI = bIsAI; // AI 여부 설정
			NewDamage->bIsResurrect = IsResurrect; // 부활 여부 설정

			// 데미지 효과 액터를 생성 완료합니다.
			UGameplayStatics::FinishSpawningActor(NewDamage, SpawnTransform);
		}
	}
}

void ACharacterBase::CalculateDyingStack(int32 InDamage, int32& DiedStack, int32& RemainingHealth)
{
	int32 CurrentDamage = InDamage;

	if (CurrentDamage < CurrentHealth)
	{
		// 현재 데미지가 현재 체력보다 작을 경우
		DiedStack = 0;
		RemainingHealth = FMath::Clamp(CurrentHealth - CurrentDamage, 0, MainStats.MaxHealth);
	}
	else
	{
		// 현재 데미지가 현재 체력보다 클 경우
		double Remainder;
		int32 ModValue = UKismetMathLibrary::FMod(CurrentDamage - CurrentHealth, MainStats.MaxHealth, Remainder);

		// 스택 감소 및 남은 체력 계산
		DiedStack = FMath::Min(ModValue + 1, Stack);
		RemainingHealth = MainStats.MaxHealth - FMath::TruncToInt32(Remainder);
	}
}

bool ACharacterBase::IsDead()
{
	// 캐릭터의 스택이 0 이하인지 확인
	return Stack <= 0;
}

void ACharacterBase::Die()
{
	if (IsValid(HealthWidget))
	{
		HealthWidget->DestroyComponent(); // 유효한 경우 건강 위젯을 파괴하십시오.
	}

	if (!DeathAnimation)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("DeathAnimation is nullptr..")); // 사망 애니메이션이 nullptr이면 오류를 기록합니다.
		return;
	}

	FTimerHandle DeathTimerHandle; // 죽음 타이머에 대한 핸들입니다.
	const float DelayTime = DeathAnimation->GetPlayLength(); // 사망 애니메이션의 지속시간.

	// 사망 이벤트의 실행을 지연시키도록 타이머를 설정하십시오.
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		OnDied(); // 타이머가 만료되면 OnDied 이벤트를 호출합니다.

		// 추가 실행을 방지하려면 타이머 핸들을 지우십시오.
		GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
	}), DelayTime, false);
}

void ACharacterBase::OnDied()
{
	FTimerHandle DiedTimerHandle; // 사망 타이머에 대한 핸들입니다.
	float DiedDelayTime = 1.0f; // 타이머의 지속 시간.

	// 캐릭터의 파괴를 지연시키는 타이머를 설정하십시오.
	GetWorld()->GetTimerManager().SetTimer(DiedTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		Destroy(); // 타이머가 만료되면 캐릭터를 파괴하십시오.

		// 추가 실행을 방지하려면 타이머 핸들을 지우십시오.
		GetWorld()->GetTimerManager().ClearTimer(DiedTimerHandle);
	}), DiedDelayTime, false);
}

void ACharacterBase::Rotate(ASlotBase* InTargetSlot)
{
	TargetSlot = InTargetSlot;

	// 대상 슬롯을 보는 데 필요한 회전을 계산합니다.
	const FRotator LookAtRotation =
		UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetSlot->GetActorLocation());

	// 대상 슬롯의 회전에 상대적인 캐릭터 회전의 요 값을 정규화합니다.
	const float NormalizeYaw = UKismetMathLibrary::NormalizeAxis(GetActorRotation().Yaw - LookAtRotation.Yaw);

	// yaw 값이 범위를 벗어나고 회전이 허용되는지 확인하십시오.
	const bool IsYawOutOfRange = !UKismetMathLibrary::InRange_FloatFloat(NormalizeYaw, -1.0f, 1.0f);

	if (bCanRotate && IsYawOutOfRange && Slot != TargetSlot)
	{
		// 정규화된 요를 기준으로 회전 타임라인의 재생 속도를 설정합니다.
		RotateOverTimeLine.SetPlayRate(RotationSpeed / FMath::Abs(NormalizeYaw));

		// 회전 방향에 따라 적절한 몽타주를 선택합니다.
		const auto SelectMontage = UKismetMathLibrary::InRange_FloatFloat(NormalizeYaw, 0.0f, 180.0f, true, false)
			                           ? RotateLeftMontage
			                           : RotateRightMontage;

		// 회전 애니메이션 몽타주를 재생합니다.
		PlayAnimationMontage(SelectMontage);

		// 회전 타임라인을 시작합니다.
		RotateOverTimeLine.PlayFromStart();
	}
	else
	{
		// 캐릭터가 회전을 마쳤음을 나타내는 이벤트를 브로드캐스팅합니다.
		if (OnCharacterRotated.IsBound())
		{
			OnCharacterRotated.Broadcast();
		}
	}
}

void ACharacterBase::Move()
{
	// 현재 재생 중인 애니메이션 몽타주를 중지합니다.
	StopAnimationMontage();

	// 이동 시작 몽타주가 유효한지 확인합니다.
	if (IsValid(StartMovingMontage))
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

		// 동적 델리게이트를 바인딩하여 몽타주 블렌딩 아웃 및 종료 이벤트를 처리합니다.
		AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ACharacterBase::OnMontageBlendingOut_Move);
		AnimInstance->OnMontageEnded.AddDynamic(this, &ACharacterBase::OnMontageEnded_Move);

		// 시작 이동 몽타주를 재생합니다.
		AnimInstance->Montage_Play(StartMovingMontage, 1.0f);

		return;
	}

	// 캐릭터의 움직임을 처리합니다.
	HandleMovement();
}

void ACharacterBase::HandleMovement()
{
	// 캐릭터의 이동 플래그를 true로 설정합니다.
	bIsMoving = true;

	// 스플라인 길이와 보행 속도를 기준으로 재생 속도를 계산합니다.
	const float PlayRate = 1.0f / (Grid->PathSpline->GetSplineLength() / WalkingSpeed);

	// 이동 타임라인의 재생 속도를 설정합니다.
	MoveOverTimeLine.SetPlayRate(PlayRate);

	// 처음부터 이동 타임라인 재생을 시작합니다.
	MoveOverTimeLine.PlayFromStart();
}

void ACharacterBase::HandleMoveOverTime_Update(float Value)
{
	// 주어진 시간 값에서 경로 스플라인의 위치를 가져옵니다.
	FVector LocationAtTime = Grid->PathSpline->GetLocationAtTime(Value, ESplineCoordinateSpace::World, true);
	LocationAtTime.Z = GetActorLocation().Z;

	// 주어진 시간 값에서 경로 스플라인의 회전을 가져옵니다.
	const FRotator RotationAtTime = Grid->PathSpline->GetRotationAtTime(Value, ESplineCoordinateSpace::World, true);

	// 현재 회전을 지정된 시간의 회전 방향으로 보간합니다.
	const FRotator NewRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), RotationAtTime,
	                                                           UGameplayStatics::GetWorldDeltaSeconds(this), 10.0f);

	// 액터의 위치와 회전을 계산된 값으로 설정합니다.
	SetActorLocationAndRotation(LocationAtTime, NewRotation);
}

void ACharacterBase::HandleMoveOverTime_Finished()
{
	// 이동 애니메이션을 재생합니다.
	if (IsValid(EndMovingMontage))
	{
		const auto AnimInstance = SkeletalMesh->GetAnimInstance();
		AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ACharacterBase::OnMontageBlendingOut_Move);
		AnimInstance->OnMontageEnded.AddDynamic(this, &ACharacterBase::OnMontageEnded_Move);
		AnimInstance->Montage_Play(EndMovingMontage, 1.0f);
		return;
	}

	// 이동 애니메이션이 없는 경우 이동 상태를 해제합니다.
	bIsMoving = false;

	// 이동이 완료되었음을 알리는 이벤트를 브로드캐스트합니다.
	if (OnCharacterMoved.IsBound())
	{
		OnCharacterMoved.Broadcast();
	}
}

void ACharacterBase::HandleRotateOverTime_Update(float Value)
{
	// 현재 액터의 회전 값을 가져옵니다.
	FRotator NewRotator = GetActorRotation();

	// 목표 슬롯을 향하는 회전 값을 계산합니다.
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), TargetSlot->GetActorLocation());

	// 보간된 회전 값을 적용합니다.
	NewRotator.Yaw = UKismetMathLibrary::RInterpTo_Constant(GetActorRotation(), LookAtRotation,
	                                                        UGameplayStatics::GetWorldDeltaSeconds(this),
	                                                        RotationSpeed).Yaw;

	// 액터의 회전 값을 설정합니다.
	SetActorRotation(NewRotator);
}

void ACharacterBase::HandleRotateOverTime_Finished()
{
	if (OnCharacterRotated.IsBound())
		OnCharacterRotated.Broadcast();
}

void ACharacterBase::OnMontageBlendingOut_Move(UAnimMontage* Montage, bool bInterrupted)
{
	// 블렌딩 아웃 몽타주가 StartMovingMontage인지 확인합니다.
	if (Montage == StartMovingMontage)
	{
		// 그렇다면 움직임을 처리하십시오.
		HandleMovement();
	}
	// 블렌딩 아웃 몽타주가 EndMovingMontage인지 확인합니다.
	else if (Montage == EndMovingMontage)
	{
		// 움직이지 않는 캐릭터로 설정
		bIsMoving = false;

		// 바인딩된 경우 OnCharacterMoved 이벤트 브로드캐스트
		if (OnCharacterMoved.IsBound())
		{
			OnCharacterMoved.Broadcast();
		}
	}
}

void ACharacterBase::OnMontageEnded_Move(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == StartMovingMontage || Montage == EndMovingMontage)
	{
		const auto AnimInstance = SkeletalMesh->GetAnimInstance();
		AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ACharacterBase::OnMontageBlendingOut_Move);
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ACharacterBase::OnMontageEnded_Move);
	}
}

void ACharacterBase::PlayAnimationMontage(UAnimMontage* MontageToPlay)
{
	// 주어진 애니메이션 몽타주가 유효한지 확인합니다.
	if (IsValid(MontageToPlay))
	{
		// 스켈레탈 메시의 애니메이션 인스턴스를 가져옵니다.
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

		// 애니메이션 몽타주의 시작 알림 이벤트에 대한 콜백 함수를 추가합니다.
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ACharacterBase::OnPlayMontageNotifyBegin_PlayAnim);

		// 애니메이션 몽타주의 종료 이벤트에 대한 콜백 함수를 추가합니다.
		AnimInstance->OnMontageEnded.AddDynamic(this, &ACharacterBase::OnMontageEnded_PlayAnim);

		// 애니메이션 인스턴스를 사용하여 애니메이션 몽타주를 재생합니다.
		AnimInstance->Montage_Play(MontageToPlay, 1.0f);
	}
}


void ACharacterBase::OnPlayMontageNotifyBegin_PlayAnim(FName NotifyName,
                                                       const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	// "Attack" 알림이면 OnAttacked 이벤트를 호출합니다.
	if (NotifyName == FName(TEXT("Attack")) && OnAttacked.IsBound())
	{
		OnAttacked.Broadcast();
	}
	// "UseAbility" 알림이면 OnAbilityUsed 이벤트를 호출합니다.
	else if (NotifyName == FName(TEXT("UseAbility")) && OnAbilityUsed.IsBound())
	{
		OnAbilityUsed.Broadcast();
	}
}

void ACharacterBase::OnMontageEnded_PlayAnim(UAnimMontage* Montage, bool bInterrupted)
{
	if (OnAnimationEnded.IsBound())
		OnAnimationEnded.Broadcast();

	// Get the animation instance of the skeletal mesh.
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

	// Remove the delegates.
	AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ACharacterBase::OnPlayMontageNotifyBegin_PlayAnim);
	AnimInstance->OnMontageEnded.RemoveDynamic(this, &ACharacterBase::OnMontageEnded_PlayAnim);
}

void ACharacterBase::StopAnimationMontage()
{
	if (OnStopAnimationMontage.IsBound())
		OnStopAnimationMontage.Broadcast();
}

void ACharacterBase::RemoveCharacterInstantly()
{
	// OnDead 대리자가 바인딩된 경우 이 이벤트를 브로드캐스트합니다.
	if (OnDead.IsBound())
	{
		OnDead.Broadcast(this);
	}

	// HealthWidget이 유효한 경우 파괴하여 캐릭터와 관련된 모든 건강 표시를 제거합니다.
	if (IsValid(HealthWidget))
	{
		HealthWidget->DestroyComponent();
	}

	// OnRemovedInstantly 대리자가 바인딩된 경우 이 이벤트를 브로드캐스트합니다.
	if (OnRemovedInstantly.IsBound())
	{
		OnRemovedInstantly.Broadcast();
	}

	// 캐릭터를 파괴하십시오.
	Destroy();
}

void ACharacterBase::OnNewTurnHandler(int32 NewTurn)
{
	ResetRetaliation(MaxRetaliationCount);
}

void ACharacterBase::AddBuff(ABuffBase* InBuff)
{
	CurrentBuffs.Add(InBuff);

	if (OnStatusEffectsUpdated.IsBound())
		OnStatusEffectsUpdated.Broadcast();
}

void ACharacterBase::RemoveBuff(ABuffBase* InBuff)
{
	CurrentBuffs.Remove(InBuff);

	if (OnStatusEffectsUpdated.IsBound())
		OnStatusEffectsUpdated.Broadcast();
}

void ACharacterBase::AddDeBuff(ADeBuffBase* InDeBuff)
{
	CurrentDeBuffs.Add(InDeBuff);

	if (OnStatusEffectsUpdated.IsBound())
		OnStatusEffectsUpdated.Broadcast();
}

void ACharacterBase::RemoveDeBuff(ADeBuffBase* InDeBuff)
{
	CurrentDeBuffs.Remove(InDeBuff);

	if (OnStatusEffectsUpdated.IsBound())
		OnStatusEffectsUpdated.Broadcast();
}

void ACharacterBase::SetDefending(bool IsDefending)
{
	if (IsDefending != bIsDefending)
	{
		bIsDefending = IsDefending;

		const float FloorDefense = FMath::Floor(MainStats.Defense * 0.3f);
		const float DefenseValue = IsDefending ? FloorDefense * -1.0f : FloorDefense;
		const FCharacterStats NewApplyStats = FCharacterStats(0, 0, 0, DefenseValue, 0, 0, 0, 0);
		ApplyStats(NewApplyStats);
	}
}

void ACharacterBase::SetWaiting(bool IsWaiting)
{
	bIsWaiting = IsWaiting;
}

void ACharacterBase::DragStarted()
{
	if (OnDragStarted.IsBound())
		OnDragStarted.Broadcast();
}

void ACharacterBase::PlayVictoryAnimation()
{
	PlayAnimationMontage(VictoryMontage);
}

bool ACharacterBase::IsMeleeCharacter()
{
	return Cast<AMeleeCharacterBase>(this) != nullptr;
}

void ACharacterBase::PlayMeleeAttackAnimation()
{
	if (UAnimMontage* RandomMeleeMontage = USRPGUtilityLibrary::GetRandomElement<UAnimMontage>(MeleeAttackMontages))
		PlayAnimationMontage(RandomMeleeMontage);
}

bool ACharacterBase::AlreadyGotBuff(ABuffBase* InBuff, ABuffBase*& ExistingBuff)
{
	// 입력된 버프가 유효하지 않은 경우, false를 반환
	if (InBuff == nullptr)
	{
		ExistingBuff = nullptr;
		return false;
	}

	// 현재 적용된 모든 버프를 확인
	for (ABuffBase* CurrentBuff : CurrentBuffs)
	{
		// 버프의 클래스와 출처가 일치하는 경우, true를 반환
		if ((CurrentBuff->GetClass() == InBuff->GetClass()) && (CurrentBuff->bIsFromAura == InBuff->bIsFromAura))
		{
			ExistingBuff = CurrentBuff;
			return true;
		}
	}

	// 일치하는 버프가 없는 경우, false를 반환
	ExistingBuff = nullptr;
	return false;
}

bool ACharacterBase::AlreadyGotDeBuff(ADeBuffBase* InDeBuff, ADeBuffBase*& ExistingDeBuff)
{
	// 입력된 디버프가 유효하지 않은 경우, false를 반환
	if (InDeBuff == nullptr)
	{
		ExistingDeBuff = nullptr;
		return false;
	}

	// 현재 적용된 모든 디버프를 확인
	for (ADeBuffBase* CurrentDeBuff : CurrentDeBuffs)
	{
		// 디버프의 클래스와 출처가 일치하는 경우, true를 반환
		if ((CurrentDeBuff->GetClass() == InDeBuff->GetClass()) && (CurrentDeBuff->bIsFromAura == InDeBuff->
			bIsFromAura))
		{
			ExistingDeBuff = CurrentDeBuff;
			return true;
		}
	}

	// 일치하는 디버프가 없는 경우, false를 반환
	ExistingDeBuff = nullptr;
	return false;
}

void ACharacterBase::ClearMeleeAttackCallback()
{
	// Check if the delegates have any bindings to this instance and if so, remove them
	if (OnAnimationEnded.IsBound())
	{
		OnAnimationEnded.RemoveAll(this);
	}

	if (OnAttacked.IsBound())
	{
		OnAttacked.RemoveAll(this);
	}
}

void ACharacterBase::ClearAbilityCallback()
{
	// Check if the delegates have any bindings to this instance and if so, remove them
	if (OnAnimationEnded.IsBound())
	{
		OnAnimationEnded.RemoveAll(this);
	}

	if (OnAttacked.IsBound())
	{
		OnAttacked.RemoveAll(this);
	}
}
