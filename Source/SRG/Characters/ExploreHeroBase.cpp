#include "SRG/Characters/ExploreHeroBase.h"

#include "DetourCrowdAIController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "SRG/Controllers/ExplorePlayerController.h"
#include "SRG/Interactables/InteractionDetector.h"
#include "SRG/Libraries/SRGFunctionLibrary.h"
#include "SRG/Misc/ExploreNavigationPath.h"
#include "SRG/Widgets/ExploreWidgets/UW_ExploreCursor.h"
#include "SRGCore/SRGLog.h"
#include "SRG/Widgets/ExploreWidgets/UW_QuestNotificationUI.h"
#include "SRGCore/AssetTableRef.h"

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

	RightClickDetector->BodyInstance.SetInstanceSimulatePhysics(true);
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

	auto WBP_QuestNotificationUISrc = DT::FindClass<UUserWidget>(DT_BLUEPRINT_PATH, FName(TEXT("WBP_QuestNotificationUI")));
	if (WBP_QuestNotificationUISrc)
	{
		QuestNotificationUIClass = WBP_QuestNotificationUISrc;
	}
}

void AExploreHeroBase::BeginPlay()
{
	Super::BeginPlay();

	InteractionDetector->OnComponentBeginOverlap.AddDynamic(this, &AExploreHeroBase::OnInteractionDetectorBeginOverlap);
	InteractionDetector->OnComponentEndOverlap.AddDynamic(this, &AExploreHeroBase::OnInteractionDetectorEndOverlap);

	RightClickDetector->OnBeginCursorOver.AddDynamic(this, &AExploreHeroBase::OnRightClickDetectorBeginCursorOver);
	RightClickDetector->OnEndCursorOver.AddDynamic(this, &AExploreHeroBase::OnRightClickDetectorEndCursorOver);


	ExplorePlayerController = Cast<AExplorePlayerController>(UGameplayStatics::GetActorOfClass(this, AExplorePlayerController::StaticClass()));

	if (!IsValid(ExplorePlayerController))
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("AExplorePlayerController 캐스팅 변환에 실패했습니다."));
		return;
	}

	//탐험 플레이 컨트롤러를 설정합니다.
	ExplorePlayerController = Cast<AExplorePlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	InitialRotation = ExplorePlayerController->GetControlRotation();

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	//기본적인 탐색 캐릭터를 설정합니다.
	SetFullMana();
	SetCharacters();
	RefreshInteractions();

	//UUW_QuestNotificationUI 위젯을 생성합니다.
	QuestNotificationUI = CreateWidget<UUW_QuestNotificationUI>(GetWorld(), QuestNotificationUIClass);
	QuestNotificationUI->ExploreHero = this;
	QuestNotificationUI->AddToViewport();
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
	int32 LastIndex;
	if (PlayerCharacterList.Num() > MaxCharacterLimit)
	{
		LastIndex = PlayerCharacterList.Num() - MaxCharacterLimit;
		for (int32 i = 0; i < LastIndex; i++)
		{
			PlayerCharacterList.RemoveAt(MaxCharacterLimit);
		}

		return;
	}
	if (PlayerCharacterList.Num() < MaxCharacterLimit)
	{
		LastIndex = MaxCharacterLimit - PlayerCharacterList.Num();
		for (int32 i = 0; i < LastIndex; i++)
		{
			PlayerCharacterList.Add(FPlayerCharacterData());
		}
		return;
	}

	if (PlayerCharacterList.Num() == MaxCharacterLimit)
	{
	}
}

void AExploreHeroBase::SetFullMana()
{
	const FHeroStats HeroStat = USRGFunctionLibrary::GetHeroStatsWithEquipment(this, this);
	CurrentMana = HeroStat.Knowledge * 10;
}

void AExploreHeroBase::DetectInteraction(AActor* Actor, UPrimitiveComponent* PrimitiveComponent)
{
	AInteractionDetector* LocalInteractionDetector = Cast<AInteractionDetector>(Actor);
	if (!LocalInteractionDetector)
		return;

	if (!(LocalInteractionDetector->InteractionSphere == PrimitiveComponent && LocalInteractionDetector->bCanInteract))
	{
		return;
	}

	LocalInteractionDetector->SetExploreHero(this);
	if (LocalInteractionDetector->bIsForceInteraction)
	{
		LocalInteractionDetector->OnInteractClicked_InteractButton(0);
	}
	else
	{
		LocalInteractionDetector->ShowInteractionUI();
	}
}

void AExploreHeroBase::RefreshInteractions()
{
	TArray<UPrimitiveComponent*> OverlappingComponents;
	InteractionDetector->GetOverlappingComponents(OverlappingComponents);

	for (UPrimitiveComponent* OverlappingComponent : OverlappingComponents)
	{
		DetectInteraction(GetOwner(), OverlappingComponent);
	}
}

void AExploreHeroBase::AddGold(int32 Amount)
{
	Gold += Amount;
}

void AExploreHeroBase::RemoveGold(int32 Amount)
{
	Gold -= Amount;
}

void AExploreHeroBase::UpdateInteractionQuest(AActor* Actor)
{
}

EQuestStatus AExploreHeroBase::GetQuestStatus(TSubclassOf<AQuestBase> InQuest)
{
	return {};
}

void AExploreHeroBase::AddQuest(AQuestBase* NewQuest)
{
}

void AExploreHeroBase::LoadQuests(AQuestBase* InQuest)
{
}

void AExploreHeroBase::DeliverQuest(AQuestBase* InQuest)
{
}

ABattleQuestBase* AExploreHeroBase::UpdateBattleQuest(AEnemyExplorePawnBase* EnemyPawn)
{
	return nullptr;
}

void AExploreHeroBase::UpdateQuest(AQuestBase* InQuest, int StepsProgress, bool OverrideProgress, bool IgnoreNotification)
{
}

void AExploreHeroBase::AddExp(int32 InExperience)
{
}

void AExploreHeroBase::ShowPath(FVector TargetLocation)
{
	if (IsValid(NavigationPath))
	{
		NavigationPath->ClearPath();
	}

	UWorld* world = GetWorld();
	if (!world)
	{
		SRPG_LOG_SCREEN_ERROR(TEXT("World가 유효하지 않습니다."));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = nullptr;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(GetActorLocation());
	SpawnTransform.SetRotation(GetActorRotation().Quaternion());

	NavigationPath = world->SpawnActor<AExploreNavigationPath>(AExploreNavigationPath::StaticClass(), SpawnTransform, SpawnParams);
	NavigationPath->ShowPath(GetActorLocation(), TargetLocation);
}

void AExploreHeroBase::OnInteractionDetectorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	DetectInteraction(OtherActor, OtherComp);
}

void AExploreHeroBase::OnInteractionDetectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                       int32 OtherBodyIndex)
{
	AInteractionDetector* CastInteractionDetector = Cast<AInteractionDetector>(OtherActor);
	if (!CastInteractionDetector)
		return;


	if (!(CastInteractionDetector->InteractionSphere == OverlappedComponent && CastInteractionDetector->bCanInteract))
	{
		return;
	}

	CastInteractionDetector->HideInteractionUI();
}

void AExploreHeroBase::OnRightClickDetectorBeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
	ExplorePlayerController->ExploreCursor->ChangeCursor(EExploreCursorType::Inventory);
}

void AExploreHeroBase::OnRightClickDetectorEndCursorOver(UPrimitiveComponent* TouchedComponent)
{
	ExplorePlayerController->ExploreCursor->ChangeCursor(EExploreCursorType::Default);
}
