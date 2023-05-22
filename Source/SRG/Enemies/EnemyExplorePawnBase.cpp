#include "SRG/Enemies/EnemyExplorePawnBase.h"

#include "Runtime/NavigationSystem/Public/NavigationSystem.h"

#include "AIController.h"
#include "AITerritory.h"
#include "MyDetourCrowdAIController.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NavAreas/NavArea_Obstacle.h"
#include "SRG/Core/SRGGameInstance.h"
#include "SRG/Controllers/ExplorePlayerController.h"
#include "SRG/Interactables/InteractionDetector.h"
#include "SRG/Widgets/ExploreWidgets/UW_EnemyInfoDialogue.h"
#include "SRG/Widgets/ExploreWidgets/UW_ExploreCursor.h"
#include "SRGCore/AssetTableRef.h"
#include "SRGCore/SRGLog.h"


AEnemyExplorePawnBase::AEnemyExplorePawnBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ConfigureCapsuleComponent();
	ConfigureMesh();
	ConfigureCharacterMovement();
	CreateInteractionActor();
	CreateRightClickDetector();
	AssignAIControllerClass();
	LoadWBP_EnemyInfoDialogueClass();
}

void AEnemyExplorePawnBase::ConfigureCapsuleComponent()
{
	GetCapsuleComponent()->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
	GetCapsuleComponent()->SetCollisionProfileName("Custom");
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
}

void AEnemyExplorePawnBase::ConfigureMesh()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName("Custom");
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
}

void AEnemyExplorePawnBase::ConfigureCharacterMovement()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AEnemyExplorePawnBase::CreateInteractionActor()
{
	InteractionActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("InteractionActor"));
	InteractionActor->SetChildActorClass(AInteractionDetector::StaticClass());
	InteractionActor->SetRelativeRotation(FRotator(0.0f, -0.00001f, 0.0f));
}

void AEnemyExplorePawnBase::CreateRightClickDetector()
{
	RightClickDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("RightClickDetector"));
	RightClickDetector->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.75f));
	RightClickDetector->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightClickDetector->SetNotifyRigidBodyCollision(true);
	RightClickDetector->SetCollisionProfileName("Custom");
	RightClickDetector->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightClickDetector->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	RightClickDetector->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
}

void AEnemyExplorePawnBase::AssignAIControllerClass()
{
	AIControllerClass = AMyDetourCrowdAIController::StaticClass();
}

void AEnemyExplorePawnBase::LoadWBP_EnemyInfoDialogueClass()
{
	if (TSubclassOf<UUW_EnemyInfoDialogue> WBP_EnemyInfoDialogue =
		DT::FindClass<UUW_EnemyInfoDialogue>(DT_BLUEPRINT_PATH, FName(TEXT("WBP_EnemyInfoDialogue"))))
	{
		WBP_EnemyInfoDialogueClass = WBP_EnemyInfoDialogue;
	}
}

void AEnemyExplorePawnBase::BeginPlay()
{
	Super::BeginPlay();

	// TimeLine 셋팅 설정
	SetupTimeLine();

	// RightClickDetector 설정
	SetupRightClickDetector();

	// AIController 설정 및 움직임 완료 Callback 설정
	SetupAIController();

	EnableAI();

	// ExplorePlayerController 캐스팅 및 에러 처리
	ExplorePlayerController = Cast<AExplorePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!ExplorePlayerController)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("ExplorePlayerController 캐스팅에 실패했습니다."));
		return;
	}

	// GameInstance 캐스팅 및 에러 처리
	GameInstance = Cast<USRGGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GameInstance)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("USRGGameInstance 캐스팅에 실패했습니다."));
		return;
	}

	// InteractionDetector 캐스팅 및 에러 처리
	InteractionDetector = Cast<AInteractionDetector>(InteractionActor->GetChildActor());
	if (!InteractionDetector)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("AInteractionDetector 캐스팅에 실패했습니다."));
		return;
	}

	// ExploreHero 캐스팅 및 에러 처리
	ExploreHero = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!ExploreHero)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("AExploreHeroBase 캐스팅에 실패했습니다."));
		return;
	}

	// InteractionDetector의 Interacted 및 Loaded Callback 설정
	SetupInteractionDetector();
}

void AEnemyExplorePawnBase::SetupTimeLine()
{
	FOnTimelineEvent EnableAITimelineUpdateCallback;
	EnableAITimelineUpdateCallback.BindUFunction(this, FName("EnableAIUpdateCallbackFunction"));
	EnableAITimeLine.SetTimelinePostUpdateFunc(EnableAITimelineUpdateCallback);

	FOnTimelineEvent EnableAITimelineFinishedCallback;
	EnableAITimelineFinishedCallback.BindUFunction(this, FName("EnableAIFinishedCallbackFunction"));
	EnableAITimeLine.SetTimelineFinishedFunc(EnableAITimelineFinishedCallback);

	EnableAITimeLine.SetLooping(false);
	EnableAITimeLine.SetTimelineLength(1.0f);
	EnableAITimeLine.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
}

void AEnemyExplorePawnBase::SetupRightClickDetector()
{
	RightClickDetector->OnBeginCursorOver.
	                    AddDynamic(this, &AEnemyExplorePawnBase::OnBeginCursorOver_RightClickDetector);
	RightClickDetector->OnEndCursorOver.AddDynamic(this, &AEnemyExplorePawnBase::OnEndCursorOver_RightClickDetector);
	RightClickDetector->OnClicked.AddDynamic(this, &AEnemyExplorePawnBase::OnClicked_RightClickDetector);
}

void AEnemyExplorePawnBase::SetupAIController()
{
	const AAIController* AIController = Cast<AAIController>(GetController());
	AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(
		this, &AEnemyExplorePawnBase::OnMoveCompletedCallback);
}

void AEnemyExplorePawnBase::SetupInteractionDetector()
{
	InteractionDetector->OnInteracted.AddDynamic(this, &AEnemyExplorePawnBase::OnInteracted_Handle);
	InteractionDetector->OnLoaded.AddDynamic(this, &AEnemyExplorePawnBase::OnLoaded_Handle);
}

void AEnemyExplorePawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnableAITimeLine.TickTimeline(DeltaTime);
}

void AEnemyExplorePawnBase::EnableAI()
{
	// AI 영역이 유효한지 확인합니다.
	if (!IsValid(AITerritory))
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("AI 영역이 존재하지 않습니다. 맵에서 설정해주세요."));
		return;
	}

	// 캐릭터 이동 속도를 설정합니다.
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// 재생 속도를 무작위로 설정합니다.
	const float NewPlayRate = 1.0f / FMath::FRandRange(2.0f, 5.0f);

	// AI 타임라인을 시작하고 재생 속도를 설정합니다.
	EnableAITimeLine.SetPlayRate(NewPlayRate);
	EnableAITimeLine.PlayFromStart();
}

void AEnemyExplorePawnBase::StopAI()
{
	// AI 컨트롤러를 가져옵니다.
	AAIController* AIController = Cast<AAIController>(GetController());

	// 이동을 중지합니다.
	AIController->StopMovement();

	// AI 타임라인을 중지합니다.
	EnableAITimeLine.Stop();
}

void AEnemyExplorePawnBase::StartBattle()
{
	// ExploreHero를 통해 전투 퀘스트 업데이트
	ABattleQuestBase* BattleQuest = ExploreHero->UpdateBattleQuest(this);

	// 게임 인스턴스를 사용하여 전투를 위해 맵 변경
	GameInstance->ChangeMapForBattle(EnemyCharacterList, ObstacleList, Gold, BattleQuest, Drops, BattleMapName);
}

void AEnemyExplorePawnBase::GetAITerritory(FVector& OutOrigin, float& OutRadius)
{
	// AITerritory의 위치를 가져와서 OutOrigin에 할당합니다.
	OutOrigin = AITerritory->GetActorLocation();

	// AITerritory의 반지름에 50을 곱해서 OutRadius에 할당합니다.
	// (반지름 값을 조정하여 올바른 크기로 변환합니다.)
	OutRadius = AITerritory->Radius * 50.0f;
}

void AEnemyExplorePawnBase::OnInteracted_Handle(int32 InButtonIndex)
{
	// StartBattle 함수 호출
	StartBattle();
}

void AEnemyExplorePawnBase::OnLoaded_Handle(bool bShouldLoadEnemyPawns)
{
	if (bShouldLoadEnemyPawns && bRespawnAfterLoad)
	{
		// 상호작용 가능하도록 InteractionDetector 활성화
		InteractionDetector->EnableInteraction();
	}
	else
	{
		// InteractionDetector 파괴
		InteractionDetector->Destroy();
	}
}

void AEnemyExplorePawnBase::OnBeginCursorOver_RightClickDetector(UPrimitiveComponent* TouchedComponent)
{
	ExplorePlayerController->ExploreCursor->ChangeCursor(EExploreCursorType::Enemy);
}

void AEnemyExplorePawnBase::OnEndCursorOver_RightClickDetector(UPrimitiveComponent* TouchedComponent)
{
	ExplorePlayerController->ExploreCursor->ChangeCursor(EExploreCursorType::Default);
}

void AEnemyExplorePawnBase::OnClicked_RightClickDetector(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::RightMouseButton && !UGameplayStatics::IsGamePaused(this))
	{
		if (!WBP_EnemyInfoDialogueClass)
		{
			// WBP_EnemyInfoDialogueClass 클래스가 설정되지 않았을 경우 에러 메시지를 화면에 표시하고 함수를 종료합니다.
			SRPG_LOG_SCREEN_ERROR(TEXT("WBP_EnemyInfoDialogueClass 클래스가 설정되지 않았습니다."));
			return;
		}

		// EnemyInfoDialogue 위젯을 생성하고 설정합니다.
		EnemyInfoDialogue = CreateWidget<UUW_EnemyInfoDialogue>(GetWorld(), WBP_EnemyInfoDialogueClass);
		EnemyInfoDialogue->EnemyCharacterList = EnemyCharacterList;
		EnemyInfoDialogue->AddToViewport();

		// OnContinueClicked 이벤트에 대한 동적 바인딩을 추가합니다.
		EnemyInfoDialogue->OnContinueClicked.AddDynamic(this, &AEnemyExplorePawnBase::OnContinueClicked_Handler);

		// 게임 일시 정지 상태로 설정합니다.
		UGameplayStatics::SetGamePaused(this, true);
	}
}

void AEnemyExplorePawnBase::OnContinueClicked_Handler()
{
	if (IsValid(EnemyInfoDialogue))
	{
		// EnemyInfoDialogue 위젯이 유효한 경우 부모로부터 제거합니다.
		EnemyInfoDialogue->RemoveFromParent();
	}

	// 게임 일시 정지 상태를 해제합니다.
	UGameplayStatics::SetGamePaused(this, false);
}

void AEnemyExplorePawnBase::EnableAIUpdateCallbackFunction()
{
}

void AEnemyExplorePawnBase::EnableAIFinishedCallbackFunction()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FVector Origin;
	float Radius;
	FNavLocation RandomLocation;
	GetAITerritory(Origin, Radius);

	if (!GetRandomReachablePointInRadius(World, Origin, Radius, RandomLocation))
	{
		return;
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}

	MoveToLocation(AIController, RandomLocation, 50.0f, false);
}

void AEnemyExplorePawnBase::OnMoveCompletedCallback(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.Code == EPathFollowingResult::Success)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		const float NewPlayRate = 1.0f / FMath::FRandRange(2.0f, 5.0f);
		EnableAITimeLine.SetPlayRate(NewPlayRate);
		EnableAITimeLine.PlayFromStart();
	}
}

bool AEnemyExplorePawnBase::GetRandomReachablePointInRadius(UWorld* World, const FVector& Origin, float Radius,
                                                            FNavLocation& RandomLocation)
{
	const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSystem)
	{
		return false;
	}
	return NavSystem->GetRandomReachablePointInRadius(Origin, Radius, RandomLocation);
}

void AEnemyExplorePawnBase::MoveToLocation(AAIController* AIController, const FVector& Destination,
                                           float AcceptanceRadius, bool bStopOnOverlap)
{
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Destination);

	if (bStopOnOverlap)
	{
		AcceptanceRadius = 0.0f;
	}

	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	FNavPathSharedPtr NavPath;
	FPathFollowingRequestResult PathFollowingRequestResult = AIController->MoveTo(MoveRequest, &NavPath);
}
