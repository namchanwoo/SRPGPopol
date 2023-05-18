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
	// 매 프레임마다 Tick()을 호출하도록 이 문자를 설정합니다. 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = true;


	GetCapsuleComponent()->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
	GetCapsuleComponent()->SetCollisionProfileName("Custom");
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);


	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName("Custom");
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;


	// ChildActorComponent 생성
	InteractionActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("InteractionActor"));
	InteractionActor->SetChildActorClass(AInteractionDetector::StaticClass());
	InteractionActor->SetRelativeRotation(FRotator(0.0f, -0.00001f, 0.0f));

	// RightClickDetector BoxComponent 만들기
	RightClickDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("RightClickDetector"));
	RightClickDetector->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.75f));
	RightClickDetector->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightClickDetector->SetNotifyRigidBodyCollision(true);
	RightClickDetector->SetCollisionProfileName("Custom");
	RightClickDetector->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightClickDetector->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	RightClickDetector->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());

	bUseControllerRotationYaw = false;

	AIControllerClass = AMyDetourCrowdAIController::StaticClass();

	TSubclassOf<UUW_EnemyInfoDialogue> WBP_EnemyInfoDialogue =
		DT::FindClass<UUW_EnemyInfoDialogue>(DT_BLUEPRINT_PATH, FName(TEXT("WBP_EnemyInfoDialogue")));
	if (WBP_EnemyInfoDialogue)
	{
		WBP_EnemyInfoDialogueClass = WBP_EnemyInfoDialogue;
	}
}


void AEnemyExplorePawnBase::BeginPlay()
{
	Super::BeginPlay();

	//TimeLine 셋팅
	{
		FOnTimelineEvent EnableAITimelineUpdateCallback;
		EnableAITimelineUpdateCallback.BindUFunction(this, FName("EnableAIUpdateCallbackFunction"));
		EnableAITimeLine.SetTimelinePostUpdateFunc(EnableAITimelineUpdateCallback);

		// Timeline이 끝났을 때 실행할 Callback 함수들입니다.
		FOnTimelineEvent EnableAITimelineFinishedCallback;
		EnableAITimelineFinishedCallback.BindUFunction(this, FName("EnableAIFinishedCallbackFunction"));
		EnableAITimeLine.SetTimelineFinishedFunc(EnableAITimelineFinishedCallback);

		EnableAITimeLine.SetLooping(false);
		EnableAITimeLine.SetTimelineLength(1.0f);
		EnableAITimeLine.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	}

	// RightClickDetector 셋팅
	{
		RightClickDetector->OnBeginCursorOver.AddDynamic(this, &AEnemyExplorePawnBase::OnBeginCursorOver_RightClickDetector);
		RightClickDetector->OnEndCursorOver.AddDynamic(this, &AEnemyExplorePawnBase::OnEndCursorOver_RightClickDetector);
		RightClickDetector->OnClicked.AddDynamic(this, &AEnemyExplorePawnBase::OnClicked_RightClickDetector);
	}

	{
		AAIController* AIController = Cast<AAIController>(GetController());
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemyExplorePawnBase::OnMoveCompletedCallback);
	}


	EnableAI();

	ExplorePlayerController = Cast<AExplorePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!ExplorePlayerController)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("ExplorePlayerController 캐스팅에 실패했습니다."));
		return;
	}

	GameInstance = Cast<USRGGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GameInstance)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("USRGGameInstance 캐스팅에 실패했습니다."));
		return;
	}

	InteractionDetector = Cast<AInteractionDetector>(InteractionActor->GetChildActor());
	if (!InteractionDetector)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("AInteractionDetector 캐스팅에 실패했습니다."));
		return;
	}

	ExploreHero = Cast<AExploreHeroBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!ExploreHero)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("AExploreHeroBase 캐스팅에 실패했습니다."));
		return;
	}

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
	if (!IsValid(AITerritory))
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("AI 영역이 존재하지 않습니다. 맵에서 설정해주세요."));
		return;
	}


	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	const float NewPlayRate = 1.0f / FMath::FRandRange(2.0f, 5.0f);
	EnableAITimeLine.SetPlayRate(NewPlayRate);
	EnableAITimeLine.PlayFromStart();
}

void AEnemyExplorePawnBase::StopAI()
{
	// UAIBlueprintHelperLibrary::GetAIController(this);
	AAIController* AIController = Cast<AAIController>(GetController());
	AIController->StopMovement();
	EnableAITimeLine.Stop();
}

void AEnemyExplorePawnBase::StartBattle()
{
	ABattleQuestBase* BattleQuest = ExploreHero->UpdateBattleQuest(this);
	GameInstance->ChangeMapForBattle(EnemyCharacterList, ObstacleList, Gold, BattleQuest, Drops, BattleMapName);
}

void AEnemyExplorePawnBase::GetAITerritory(FVector& OutOrigin, float& OutRadius)
{
	OutOrigin = AITerritory->GetActorLocation();
	OutRadius = AITerritory->Radius * 50.0f;
}

void AEnemyExplorePawnBase::OnInteracted_Handle(int32 InButtonIndex)
{
	StartBattle();
}

void AEnemyExplorePawnBase::OnLoaded_Handle(bool bShouldLoadEnemyPawns)
{
	if (bShouldLoadEnemyPawns && bRespawnAfterLoad)
	{
		InteractionDetector->EnableInteraction();
	}
	else
	{
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
	if (ButtonPressed == EKeys::RightMouseButton && UGameplayStatics::IsGamePaused(this))
	{
		EnemyInfoDialogue = CreateWidget<UUW_EnemyInfoDialogue>(GetWorld(), WBP_EnemyInfoDialogueClass);
		EnemyInfoDialogue->EnemyCharacterList = EnemyCharacterList;
		EnemyInfoDialogue->AddToViewport();

		EnemyInfoDialogue->OnContinueClicked.AddDynamic(this, &AEnemyExplorePawnBase::OnContinueClicked_Handler);
		UGameplayStatics::SetGamePaused(this, true);
	}
}

void AEnemyExplorePawnBase::OnContinueClicked_Handler()
{
	EnemyInfoDialogue->RemoveFromParent();
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
		SRPG_LOG_SCREEN(TEXT("경로 이동이 완료되었습니다!"));

		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		const float NewPlayRate = 1.0f / FMath::FRandRange(2.0f, 5.0f);
		EnableAITimeLine.SetPlayRate(NewPlayRate);
		EnableAITimeLine.PlayFromStart();
	}
}

bool AEnemyExplorePawnBase::GetRandomReachablePointInRadius(UWorld* World, const FVector& Origin, float Radius, FNavLocation& RandomLocation)
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSystem)
	{
		return false;
	}
	return NavSystem->GetRandomReachablePointInRadius(Origin, Radius, RandomLocation);
}

void AEnemyExplorePawnBase::MoveToLocation(AAIController* AIController, const FVector& Destination, float AcceptanceRadius, bool bStopOnOverlap)
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

	if (PathFollowingRequestResult.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		SRPG_LOG_SCREEN(TEXT("경로가 성공했습니다."));
	}
}
