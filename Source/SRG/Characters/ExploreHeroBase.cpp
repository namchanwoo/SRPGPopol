#include "SRG/Characters/ExploreHeroBase.h"

#include "Blueprint/UserWidget.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Obstacle.h"

#include "SRG/Core/SRPGGameInstance.h"
#include "SRGCore/Utilities/SRGLog.h"
#include "SRGCore/Utilities/AssetTableRef.h"

#include "SRG/Equipment/EquipmentBase.h"
#include "SRG/Controllers/ExplorePlayerController.h"
#include "SRG/Interactables/InteractionDetector.h"
#include "SRG/Libraries/SRPGFunctionLibrary.h"
#include "SRG/Misc/ExploreNavigationPath.h"
#include "SRG/Quests/QuestBase.h"
#include "SRG/Quests/BattleQuests/BattleQuestBase.h"
#include "SRG/Quests/InteractionQuests/InteractionQuestBase.h"
#include "SRG/Units/Characters/CharacterBase.h"

#include "SRG/Widgets/ExploreWidgets/UW_ClaimDialogue.h"
#include "SRG/Widgets/ExploreWidgets/UW_ExploreCursor.h"
#include "SRG/Widgets/ExploreWidgets/UW_GarrisonListClass.h"
#include "SRG/Widgets/ExploreWidgets/UW_HeroInventory.h"
#include "SRG/Widgets/ExploreWidgets/UW_LevelUpDialogue.h"
#include "SRG/Widgets/ExploreWidgets/UW_QuestNotificationUI.h"


AExploreHeroBase::AExploreHeroBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Custom"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("Custom"));
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeRotation(FRotator(-65.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 7.0f;
	SpringArm->CameraRotationLagSpeed = 5.0f;
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);


	RightClickDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("RightClickDetector"));
	RightClickDetector->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.75f));
	RightClickDetector->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
	/*RightClickDetector->BodyInstance.SetInstanceSimulatePhysics(true);*/
	RightClickDetector->SetNotifyRigidBodyCollision(true);
	RightClickDetector->SetCollisionProfileName(TEXT("Custom"));
	RightClickDetector->SetCollisionResponseToAllChannels(ECR_Overlap);
	RightClickDetector->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	RightClickDetector->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightClickDetector->SetupAttachment(GetCapsuleComponent());


	InteractionDetector = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionDetector"));
	InteractionDetector->InitSphereRadius(200.0f);
	RightClickDetector->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
	InteractionDetector->SetupAttachment(GetCapsuleComponent());

	if (const TSubclassOf<UUserWidget> WBP_QuestNotificationUIClassSrc =
		DT::FindClass<UUserWidget>(DT_WIDGET_PATH, FName(TEXT("WBP_QuestNotificationUI"))))
	{
		WBP_QuestNotificationUIClass = WBP_QuestNotificationUIClassSrc;
	}
}

void AExploreHeroBase::BeginPlay()
{
	Super::BeginPlay();

	// 바인딩 상호 작용 이벤트
	BindInteractions();

	// 플레이어 컨트롤러 설정을 시도하고 실패할 경우 반환
	if (!ValidateAndSetPlayerController())
	{
		return;
	}

	// 초기 캐릭터 설정
	SetInitialCharacterSettings();

	// 퀘스트 알림 UI 생성 및 구성
	CreateQuestNotificationUI();
}

void AExploreHeroBase::BindInteractions()
{
	// 상호 작용 감지를 위한 바인딩 오버랩 이벤트
	InteractionDetector->OnComponentBeginOverlap.AddDynamic(
		this, &AExploreHeroBase::OnComponentBeginOverlap_InteractionDetector);
	InteractionDetector->OnComponentEndOverlap.AddDynamic(
		this, &AExploreHeroBase::OnComponentEndOverlap_InteractionDetector);

	// 오른쪽 클릭 감지를 위한 이벤트 위에 커서 표시
	RightClickDetector->OnBeginCursorOver.AddDynamic(this, &AExploreHeroBase::OnBeginCursorOver_RightClickDetector);
	RightClickDetector->OnEndCursorOver.AddDynamic(this, &AExploreHeroBase::OnEndCursorOver_RightClickDetector);

	// 오른쪽 클릭 감지를 위한 클릭이벤트
	RightClickDetector->OnClicked.AddDynamic(this, &AExploreHeroBase::OnClicked_RightClickDetector);
}

bool AExploreHeroBase::ValidateAndSetPlayerController()
{
	// 액터를 플레이어 컨트롤러 클래스로 캐스팅하려고 시도합니다.
	ExplorePlayerController = Cast<AExplorePlayerController>(
		UGameplayStatics::GetActorOfClass(this, AExplorePlayerController::StaticClass()));

	// 캐스팅에 실패하면 오류를 기록하고 false를 반환합니다.
	if (!IsValid(ExplorePlayerController))
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("AExplorePlayerController 캐스팅에 실패했습니다."));
		return false;
	}

	// 플레이어 컨트롤러 및 초기 회전 설정
	ExplorePlayerController = Cast<AExplorePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	InitialRotation = ExplorePlayerController->GetControlRotation();
	return true;
}

void AExploreHeroBase::SetInitialCharacterSettings()
{
	// 최대 보행 속도, 최대 마나, 캐릭터 및 새로 고침 상호 작용 설정
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	SetFullMana();
	SetCharacters();
	RefreshInteractions();
}

void AExploreHeroBase::CreateQuestNotificationUI()
{
	// 퀘스트 알림 UI 위젯 생성 및 뷰포트에 추가
	WBP_QuestNotificationUI = CreateWidget<UUW_QuestNotificationUI>(GetWorld(), WBP_QuestNotificationUIClass);
	WBP_QuestNotificationUI->ExploreHero = this;
	WBP_QuestNotificationUI->AddToViewport(999);
}

void AExploreHeroBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExploreHeroBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AExploreHeroBase::SetCharacters()
{
	const int32 Difference = MaxCharacterLimit - PlayerCharacterList.Num();

	if (Difference > 0)
	{
		// 제한보다 적은 캐릭터, 빈 캐릭터 추가
		for (int32 i = 0; i < Difference; i++)
		{
			PlayerCharacterList.Add(FPlayerCharacterData());
		}
	}
	else if (Difference < 0)
	{
		// 제한보다 더 많은 캐릭터, 여분의 캐릭터를 제거하십시오
		PlayerCharacterList.RemoveAt(MaxCharacterLimit, -Difference);
	}
}

void AExploreHeroBase::SetFullMana()
{
	const FHeroStats HeroStat = USRPGFunctionLibrary::GetHeroStatsWithEquipment(this, this);
	if (HeroStat.Knowledge < 0)
	{
		SRPG_LOG_WARNING(TEXT("SetFullMana에서 부정적인 지식 통계가 감지되었습니다."));
		return;
	}
	CurrentMana = HeroStat.Knowledge * 10;
}

void AExploreHeroBase::DismissCharacter(int32 Index)
{
	if (!PlayerCharacterList.IsValidIndex(Index))
	{
		SRPG_LOG_WARNING(TEXT("DismissCharacter에서 잘못된 인덱스가 감지되었습니다."));
		return;
	}

	PlayerCharacterList[Index].Character = nullptr;
	PlayerCharacterList[Index].Stack = 0;
}

void AExploreHeroBase::DetectInteraction(AActor* Actor, UPrimitiveComponent* Component)
{
	AInteractionDetector* LocalInteractionDetector = Cast<AInteractionDetector>(Actor);
	if (LocalInteractionDetector && LocalInteractionDetector->InteractionSphere == Component &&
		LocalInteractionDetector->bCanInteract)
	{
		LocalInteractionDetector->SetExploreHero(this);
		if (LocalInteractionDetector->bIsForceInteraction)
		{
			LocalInteractionDetector->OnInteractClicked_InteractionUI(0);
		}
		else
		{
			LocalInteractionDetector->ShowInteractionUI();
		}
	}
}

void AExploreHeroBase::RefreshInteractions()
{
	TArray<UPrimitiveComponent*> OverlappingComponents;
	InteractionDetector->GetOverlappingComponents(OverlappingComponents);

	for (const auto& OverlappingComponent : OverlappingComponents)
	{
		DetectInteraction(GetOwner(), OverlappingComponent);
	}
}

void AExploreHeroBase::AddGold(int32 Amount)
{
	// 금액이 음수가 아닌지 확인하십시오.
	if (Amount < 0)
	{
		SRPG_LOG_WARNING(TEXT("마이너스 골드 금액을 추가하려고 했습니다."));
		return;
	}
	Gold += Amount;
}

void AExploreHeroBase::RemoveGold(int32 Amount)
{
	// 금액이 음수가 아닌지 확인하십시오.
	if (Amount < 0)
	{
		SRPG_LOG_WARNING(TEXT("마이너스 골드 금액을 제거하려고 시도했습니다."));
		return;
	}

	// 제거할 골드가 충분한지 확인합니다.
	if (Gold < Amount)
	{
		SRPG_LOG_WARNING(TEXT("사용 가능한 것보다 더 많은 금을 제거하려고 시도했습니다."));
		return;
	}

	Gold -= Amount;
}

void AExploreHeroBase::UpdateInteractionQuest(AActor* InteractableActor)
{
	// 현재 활성화된 모든 퀘스트에 대해 반복합니다.
	for (AQuestBase* ActiveQuest : ActiveQuests)
	{
		// 현재 퀘스트가 상호작용 퀘스트인지 확인합니다.
		if (AInteractionQuestBase* InteractionQuestBase = Cast<AInteractionQuestBase>(ActiveQuest))
		{
			// 해당 퀘스트가 활성 퀘스트 데이터에 존재하는지 확인합니다.
			if (!ActiveQuestData.Contains(InteractionQuestBase->GetClass()))
			{
				continue;
			}

			// 현재 퀘스트의 진행 단계 데이터를 가져옵니다.
			const FQuestStepData* QuestStepData = ActiveQuestData.Find(InteractionQuestBase->GetClass());

			// 상호작용하려는 엑터가 현재 퀘스트 단계의 상호작용 가능 엑터인지 확인합니다.
			if (InteractionQuestBase->InteractableActors[QuestStepData->CurrentStep] != InteractableActor)
			{
				continue;
			}

			// 상호작용이 가능한 엑터가 맞다면, 퀘스트를 업데이트합니다.
			UpdateQuest(ActiveQuest, 1, false, false);
		}
	}
}

EQuestStatus AExploreHeroBase::GetQuestStatus(TSubclassOf<AQuestBase> InQuest)
{
	// 퀘스트가 사용 가능하지 않은 경우, 상태를 Unavailable로 반환합니다.
	if (!IsQuestAvailable(InQuest))
	{
		return EQuestStatus::Unavailable;
	}

	// 퀘스트가 활성화된 상태인 경우
	if (ActiveQuestData.Contains(InQuest))
	{
		const FQuestStepData QuestStepData = *ActiveQuestData.Find(InQuest);

		// 모든 퀘스트 단계가 완료된 경우, 상태를 ReadyToDeliver로 반환합니다.
		if (QuestStepData.CurrentStep == QuestStepData.TotalSteps)
		{
			return EQuestStatus::ReadyToDeliver;
		}

		// 그렇지 않은 경우, 상태를 Active로 반환합니다.
		return EQuestStatus::Active;
	}

	// 퀘스트가 완료된 상태인 경우, 상태를 Completed로 반환합니다.
	if (CompletedQuestData.Contains(InQuest))
	{
		return EQuestStatus::Completed;
	}

	// 그 외의 경우, 새로운 퀘스트로 간주하고 상태를 New로 반환합니다.
	return EQuestStatus::New;
}

void AExploreHeroBase::AddQuest(AQuestBase* NewQuest)
{
	// 함수 로깅 시작
	;

	// NewQuest가 유효한지 확인
	if (NewQuest == nullptr)
	{
		SRPG_LOG_ERROR(TEXT("AExploreHeroBase::AddQuest() 에서 NewQuest가 nullptr입니다."));
		return;
	}

	UClass* QuestClass = NewQuest->GetClass(); // 다중 호출을 피하기 위해 퀘스트 클래스 저장

	// 새 퀘스트의 상태 얻기
	const EQuestStatus NewQuestStatus = GetQuestStatus(QuestClass);

	if (NewQuestStatus == EQuestStatus::New)
	{
		// 새 퀘스트 단계 데이터 생성
		const FQuestStepData NewQuestStepData = FQuestStepData(0, NewQuest->GetSteps());

		// 새 퀘스트를 활성 퀘스트 데이터와 활성 퀘스트에 추가
		ActiveQuestData.Add(QuestClass, NewQuestStepData);
		ActiveQuests.Add(NewQuest);

		// 게임 인스턴스 얻기 및 USRGGameInstance 타입인지 확인
		USRPGGameInstance* SRGGameInstance = Cast<USRPGGameInstance>(UGameplayStatics::GetGameInstance(this));
		SRPG_CHECK(SRGGameInstance); // SRPG_CHECK을 사용하여 null 검사

		// 새 퀘스트를 게임 인스턴스에 추가
		SRGGameInstance->AddQuest(NewQuest);

		// 퀘스트 클래스의 기본 객체 얻기
		const AQuestBase* QuestCDO = QuestClass->GetDefaultObject<AQuestBase>();
		if (QuestCDO && QuestCDO->bShowNotification) // QuestCDO가 유효하고 bShowNotification이 true인지 확인
		{
			// 퀘스트 알림 표시
			WBP_QuestNotificationUI->ShowQuestNotification(QuestClass, NewQuestStepData);
		}
	}
}

void AExploreHeroBase::LoadQuests(AQuestBase* InQuest)
{
	ActiveQuests.Add(InQuest);
}

void AExploreHeroBase::DeliverQuest(AQuestBase* InQuest)
{
	// NewQuest가 유효한지 확인
	if (InQuest == nullptr)
	{
		SRPG_LOG_ERROR(TEXT("AExploreHeroBase::AddQuest() 에서 InQuest가 nullptr입니다."));
		return;
	}

	UClass* QuestClass = InQuest->GetClass(); // 다중 호출을 피하기 위해 퀘스트 클래스 저장

	// 새 퀘스트의 상태 얻기
	const EQuestStatus InQuestStatus = GetQuestStatus(QuestClass);

	if (InQuestStatus == EQuestStatus::ReadyToDeliver)
	{
		ActiveQuestData.Remove(QuestClass);
		ActiveQuests.Remove(InQuest);
		CompletedQuestData.Add(QuestClass, InQuest->GetSteps());

		// 게임 인스턴스 얻기 및 USRGGameInstance 타입인지 확인
		USRPGGameInstance* SRGGameInstance = Cast<USRPGGameInstance>(UGameplayStatics::GetGameInstance(this));
		SRPG_CHECK(SRGGameInstance); // SRPG_CHECK을 사용하여 null 검사

		SRGGameInstance->RemoveQuest(InQuest);

		// 퀘스트 클래스의 기본 객체 얻기
		if (AQuestBase* QuestCDO = QuestClass->GetDefaultObject<AQuestBase>())
		{
			ShowQuestRewards(QuestCDO->QuestRewards);
		}
	}
}

void AExploreHeroBase::ShowQuestRewards(const FQuestRewards& QuestRewards)
{
	if (!WBP_ClaimDialogueClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("WBP_ClaimDialogueClas 가 설정되지 않나 위젯을 생성할 수 없습니다."));
		return;
	}

	CurrentQuestRewards = QuestRewards;
	WBP_ClaimDialogue = CreateWidget<UUW_ClaimDialogue>(GetWorld(), WBP_ClaimDialogueClass);
	WBP_ClaimDialogue->ExploreHero = this;
	WBP_ClaimDialogue->DialogueTitle = FText::FromString(TEXT("Quest Rewards"));
	WBP_ClaimDialogue->YesButtonText = FText::FromString(TEXT("Claim"));
	WBP_ClaimDialogue->Characters = QuestRewards.Characters;
	WBP_ClaimDialogue->Equipment = QuestRewards.Equipment;
	WBP_ClaimDialogue->Gold = QuestRewards.Gold;
	WBP_ClaimDialogue->Exp = QuestRewards.Exp;
	WBP_ClaimDialogue->AddToViewport();
	WBP_ClaimDialogue->OnClaimClicked.AddDynamic(this, &AExploreHeroBase::OnQuestRewardsClaimed);
	UGameplayStatics::SetGamePaused(this, true);
}

void AExploreHeroBase::ReceiveQuestRewards(const FQuestRewards& InQuestRewards)
{
	AddEquipments(InQuestRewards.Equipment);
	AddGold(InQuestRewards.Gold);
	AddExp(InQuestRewards.Exp);
}

void AExploreHeroBase::AddEquipment(TSubclassOf<AEquipmentBase> InEquipment)
{
	BackPack.Add(InEquipment);
}

void AExploreHeroBase::AddEquipments(const TArray<TSubclassOf<AEquipmentBase>> InEquipments)
{
	for (TSubclassOf<AEquipmentBase> InEquipment : InEquipments)
	{
		AddEquipment(InEquipment);
	}
}

void AExploreHeroBase::RemoveEquipment(TSubclassOf<AEquipmentBase> InEquipment)
{
	const int32 RemoveIndex = BackPack.Find(InEquipment);
	if (RemoveIndex == INDEX_NONE)
		BackPack.RemoveAt(RemoveIndex);
}

void AExploreHeroBase::AddCharacters(const TArray<FPlayerCharacterData>& InCharacters)
{
	if (!WBP_GarrisonListClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("WBP_GarrisonListClass가 nullptr입니다."))
		return;
	}

	WBP_GarrisonList = CreateWidget<UUW_GarrisonListClass>(GetWorld(), WBP_GarrisonListClass);
	WBP_GarrisonList->AddToViewport();
	WBP_GarrisonList->OnContinueClicked.AddDynamic(this, &AExploreHeroBase::OnGarrisonListClosedHandler);
}

void AExploreHeroBase::BlockPlayerInput(bool IsBlocked)
{
	bIsInputBlocked = IsBlocked;
}

void AExploreHeroBase::Equip(TSubclassOf<AEquipmentBase> InEquipment)
{
	const int32 EquipIndex = BackPack.Find(InEquipment);

	if (EquipIndex != INDEX_NONE)
	{
		BackPack.RemoveAt(EquipIndex);
		Equipment.Add(InEquipment);
	}
}

void AExploreHeroBase::UnEquip(TSubclassOf<AEquipmentBase> InEquipment)
{
	const int32 UnEquipIndex = Equipment.Find(InEquipment);

	if (UnEquipIndex != INDEX_NONE)
	{
		Equipment.RemoveAt(UnEquipIndex);
		BackPack.Add(InEquipment);
	}
}

bool AExploreHeroBase::HasOnlyCharacter()
{
	// 유효한 캐릭터의 수를 계산하는 변수입니다.
	int32 NumberOfCharacters = 0;

	// PlayerCharacterList의 모든 요소를 순회하며 각 캐릭터가 유효한지 확인합니다.
	for (const FPlayerCharacterData& CharacterData : PlayerCharacterList)
	{
		// 현재 캐릭터가 유효한 경우 NumberOfCharacters를 증가시킵니다.
		if (IsValid(CharacterData.Character))
		{
			NumberOfCharacters++;

			// 유효한 캐릭터가 두 개 이상 발견되면 더 이상 순회할 필요가 없으므로 false를 반환합니다.
			if (NumberOfCharacters > 1)
			{
				return false;
			}
		}
	}

	// 모든 캐릭터를 확인한 후에 유효한 캐릭터가 정확히 하나인지 확인하여 결과를 반환합니다.
	return NumberOfCharacters == 1;
}

bool AExploreHeroBase::HasEmptySlot(int32& EmptySlotIndex)
{
	for (int32 Index = 0; Index < PlayerCharacterList.Num(); ++Index)
	{
		if (!IsValid(PlayerCharacterList[Index].Character))
		{
			EmptySlotIndex = Index;
			return true;
		}
	}

	EmptySlotIndex = -1; // 빈 슬롯이 없는 경우 -1을 반환합니다.
	return false;
}

bool AExploreHeroBase::HasThisCharacter(TSubclassOf<ACharacterBase> InCharacter, int32& Index)
{
	for (int32 i = 0; i < PlayerCharacterList.Num(); ++i)
	{
		if (PlayerCharacterList[Index].Character == InCharacter)
		{
			Index = i;
			return true;
		}
	}

	// 캐릭터를 찾을 수 없는 경우 -1을 반환합니다.
	Index = -1;
	return false;
}

bool AExploreHeroBase::IsQuestAvailable(TSubclassOf<AQuestBase> InQuest)
{
	// InQuest에서 기본 객체를 가져와서 검사합니다.
	// QuestCDO가 유효한 경우, 필요한 모든 퀘스트가 완료되었는지 확인합니다.
	if (AQuestBase* QuestCDO = InQuest->GetDefaultObject<AQuestBase>())
	{
		// 각 필요 퀘스트에 대해
		for (auto& RequiredQuest : QuestCDO->RequiredQuests)
		{
			// 이 퀘스트가 완료 퀘스트 데이터에 없으면 false를 반환합니다.
			if (!CompletedQuestData.Contains(RequiredQuest))
			{
				return false;
			}
		}
	}

	// 모든 필요한 퀘스트가 완료된 경우, true를 반환합니다.
	return true;
}

ABattleQuestBase* AExploreHeroBase::UpdateBattleQuest(AEnemyExplorePawnBase* EnemyPawn)
{
	// 현재 활성화된 모든 퀘스트에 대해 반복합니다.
	for (AQuestBase* ActiveQuest : ActiveQuests)
	{
		// 현재 퀘스트가 전투 퀘스트인지 확인합니다.
		if (ABattleQuestBase* BattleQuestBase = Cast<ABattleQuestBase>(ActiveQuest))
		{
			// 해당 퀘스트가 활성 퀘스트 데이터에 존재하는지 확인합니다.
			if (!ActiveQuestData.Contains(BattleQuestBase->GetClass()))
			{
				continue;
			}

			// 현재 퀘스트의 진행 단계 데이터를 가져옵니다.
			FQuestStepData* QuestStepData = ActiveQuestData.Find(BattleQuestBase->GetClass());

			// 플레이어가 전투하려는 적 피스가 현재 퀘스트 단계의 적 피스인지 확인합니다.
			if (BattleQuestBase->EnemyPawn[QuestStepData->CurrentStep] != EnemyPawn)
			{
				continue;
			}

			// 전투하려는 적 피스가 맞다면, 해당 전투 퀘스트를 반환합니다.
			return BattleQuestBase;
		}
	}

	// 전투 퀘스트가 없으면 nullptr를 반환합니다.
	return nullptr;
}

void AExploreHeroBase::UpdateQuest(AQuestBase* InQuest, int StepsProgress, bool OverrideProgress,
                                   bool IgnoreNotification)
{
	// 해당 퀘스트의 현재 상태를 확인합니다.
	const EQuestStatus QuestStatus = GetQuestStatus(InQuest->GetClass());

	// 퀘스트가 활성 상태가 아니라면, 함수를 여기에서 종료합니다.
	if (QuestStatus != EQuestStatus::Active)
	{
		return;
	}

	// 해당 퀘스트가 활성 퀘스트 데이터에 존재하지 않는다면, 함수를 여기에서 종료합니다.
	if (!ActiveQuestData.Contains(InQuest->GetClass()))
	{
		return;
	}

	// 현재 퀘스트의 진행 단계 데이터를 가져옵니다.
	FQuestStepData* CurrentQuestStepData = ActiveQuestData.Find(InQuest->GetClass());

	// OverrideProgress 값에 따라 진행 단계를 업데이트합니다.
	const int32 ProgressValue = OverrideProgress ? StepsProgress : StepsProgress + CurrentQuestStepData->CurrentStep;

	// 진행 단계를 업데이트하고, 이 값을 퀘스트 단계 범위 내로 제한합니다.
	CurrentQuestStepData->CurrentStep = FMath::Clamp(ProgressValue, 0, InQuest->GetSteps());

	// 모든 퀘스트 단계가 완료되지 않았다면, 퀘스트를 활성화시킵니다.
	if (CurrentQuestStepData->CurrentStep != CurrentQuestStepData->TotalSteps)
	{
		InQuest->Activate(CurrentQuestStepData->CurrentStep);
	}

	// 통지 무시 옵션이 비활성화되어 있고, 퀘스트에 통지 표시가 설정되어 있다면, 퀘스트 통지를 보여줍니다.
	if (!IgnoreNotification && InQuest->bShowNotification)
	{
		WBP_QuestNotificationUI->ShowQuestNotification(InQuest->GetClass(), *CurrentQuestStepData);
	}
}

void AExploreHeroBase::AddExp(int32 AddExperience)
{
	// 경험치가 0보다 작거나 같다면 아무것도 하지 마십시오.
	if (AddExperience <= 0)
	{
		SRPG_LOG_WARNING(TEXT("AddExp가 양수가 아닌 값으로 호출되었습니다."));
		return;
	}

	// 현재 레벨 및 통계 가져오기
	int32 BeforeLevel;
	FHeroStats BeforeHeroStats;
	USRPGFunctionLibrary::GetHeroStats(this, BeforeLevel, BeforeHeroStats);

	// 경험치 증가
	Exp += AddExperience;

	// 업데이트된 레벨 및 통계 가져오기
	int32 AfterLevel;
	FHeroStats AfterHeroStats;
	USRPGFunctionLibrary::GetHeroStats(this, AfterLevel, AfterHeroStats);

	// 영웅 레벨업 여부 확인
	if (AfterLevel > BeforeLevel)
	{
		// 영웅이 레벨업하면 마나를 보충합니다.
		SetFullMana();

		// 레벨업 동안 얻은 스탯 계산
		FHeroStats GainedStats = AfterHeroStats - BeforeHeroStats;

		// 레벨 업 대화상자 표시
		ShowLevelUp(GainedStats, AfterLevel - BeforeLevel, AfterLevel);
	}
}

void AExploreHeroBase::ShowLevelUp(const FHeroStats& GainedStats, int32 LevelUpAmount, int32 CurrentLevel)
{
	// 대화 상자 클래스가 설정되어 있는지 확인하십시오.
	if (!WBP_LevelUpDialogueClass)
	{
		SRPG_LOG_WARNING(TEXT("WBP_LevelUpDialogueClass가 설정되지 않았습니다."));
		return;
	}

	// 레벨 업 대화 상자 만들기
	WBP_LevelUpDialogue = CreateWidget<UUW_LevelUpDialogue>(GetWorld(), WBP_LevelUpDialogueClass);

	// 대화 상자가 성공적으로 생성되었는지 확인
	if (!WBP_LevelUpDialogue)
	{
		SRPG_LOG_WARNING(TEXT("WBP_LevelUpDialogue를 생성하지 못했습니다."));
		return;
	}

	// 대화 상자 속성 설정
	WBP_LevelUpDialogue->ExploreHero = this;
	WBP_LevelUpDialogue->GainedStats = GainedStats;
	WBP_LevelUpDialogue->LevelUpAmount = LevelUpAmount;
	WBP_LevelUpDialogue->CurrentLevel = CurrentLevel;

	// 대화상자 표시
	WBP_LevelUpDialogue->AddToViewport();

	// 대화 상자에서 계속 버튼 클릭 처리
	WBP_LevelUpDialogue->OnContinueClicked.AddDynamic(this, &AExploreHeroBase::OnLevelUpClosed);

	// 게임 일시 중지
	UGameplayStatics::SetGamePaused(this, true);
}

void AExploreHeroBase::ShowPath(FVector TargetLocation)
{
	// 현재 네비게이션 경로가 유효한지 확인하고, 유효하다면 경로를 지웁니다.
	if (IsValid(NavigationPath))
	{
		NavigationPath->ClearPath();
	}

	// 현재 게임의 세계를 가져옵니다.
	UWorld* world = GetWorld();
	if (!world)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("World가 유효하지 않습니다."));
		return;
	}

	// 배우를 스폰하기 위한 파라미터를 설정합니다.
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = nullptr;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;

	// 배우를 스폰할 위치와 회전을 설정합니다.
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(GetActorLocation());
	SpawnTransform.SetRotation(GetActorRotation().Quaternion());

	// NavigationPath 배우를 스폰합니다.
	NavigationPath = world->SpawnActor<AExploreNavigationPath>(AExploreNavigationPath::StaticClass(),
	                                                           SpawnTransform, SpawnParams);

	// 스폰된 배우에게 경로를 표시하도록 요청합니다.
	NavigationPath->ShowPath(GetActorLocation(), TargetLocation);
}

void AExploreHeroBase::OnComponentBeginOverlap_InteractionDetector(UPrimitiveComponent* OverlappedComponent,
                                                                   AActor* OtherActor,
                                                                   UPrimitiveComponent* OtherComp,
                                                                   int32 OtherBodyIndex, bool bFromSweep,
                                                                   const FHitResult& SweepResult)
{
	DetectInteraction(OtherActor, OtherComp);
}

void AExploreHeroBase::OnComponentEndOverlap_InteractionDetector(UPrimitiveComponent* OverlappedComponent,
                                                                 AActor* OtherActor,
                                                                 UPrimitiveComponent* OtherComp,
                                                                 int32 OtherBodyIndex)
{
	AInteractionDetector* CastInteractionDetector = Cast<AInteractionDetector>(OtherActor);
	if (CastInteractionDetector)
	{
		if (CastInteractionDetector->InteractionSphere == OverlappedComponent && CastInteractionDetector->bCanInteract)
		{
			CastInteractionDetector->HideInteractionUI();
		}
	}
}

void AExploreHeroBase::OnBeginCursorOver_RightClickDetector(UPrimitiveComponent* TouchedComponent)
{
	SRPG_LOG_SCREEN(TEXT("커서가 들어왔습니다!~!!"));
	ExplorePlayerController->GetExploreCursor()->ChangeCursor(EExploreCursorType::Inventory);
}

void AExploreHeroBase::OnEndCursorOver_RightClickDetector(UPrimitiveComponent* TouchedComponent)
{
	SRPG_LOG_SCREEN(TEXT("커서가 나갔습니다!~!!"));
	ExplorePlayerController->GetExploreCursor()->ChangeCursor(EExploreCursorType::Default);
}

void AExploreHeroBase::OnClicked_RightClickDetector(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (!WBP_HeroInventoryClass)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("WBP_HeroInventoryClass 클래스가 존재하지 않습니다."));
		return;
	}

	if (!bIsInputBlocked)
	{
		if (ButtonPressed == EKeys::RightMouseButton && !UGameplayStatics::IsGamePaused(this))
		{
			StopCheckingMovement();
			UGameplayStatics::SetGamePaused(this, true);

			WBP_HeroInventory = CreateWidget<UUW_HeroInventory>(GetWorld(), WBP_HeroInventoryClass);
			WBP_HeroInventory->AddToViewport();
			WBP_HeroInventory->OnOkClicked.AddDynamic(this, &AExploreHeroBase::OnOkClickedHandle);
		}
	}
}

void AExploreHeroBase::OnOkClickedHandle()
{
	if (IsValid(WBP_HeroInventory))
	{
		WBP_HeroInventory->RemoveFromParent();
	}
	UGameplayStatics::SetGamePaused(this, false);
}

void AExploreHeroBase::OnLevelUpClosed()
{
	if (IsValid(WBP_LevelUpDialogue))
	{
		WBP_LevelUpDialogue->RemoveFromParent();
	}

	UGameplayStatics::SetGamePaused(this, false);
}

void AExploreHeroBase::OnQuestRewardsClaimed()
{
	if (IsValid(WBP_ClaimDialogue))
		WBP_ClaimDialogue->RemoveFromParent();

	UGameplayStatics::SetGamePaused(this, false);
	ReceiveQuestRewards(CurrentQuestRewards);
}

void AExploreHeroBase::OnGarrisonListClosedHandler()
{
	if (WBP_GarrisonList)
	{
		WBP_GarrisonList->RemoveFromParent();
	}

	if (OnGarrisonListClosed.IsBound())
	{
		OnGarrisonListClosed.Broadcast();
	}
}
