#include "EnemySpawnLocation.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "SRGCore/Utilities/AssetTableRef.h"


AEnemySpawnLocation::AEnemySpawnLocation()
{
	// 매 프레임마다 Tick()을 호출하도록 이 액터를 설정합니다. 필요하지 않은 경우 성능을 개선하기 위해 이 기능을 끌 수 있습니다.
	PrimaryActorTick.bCanEverTick = false;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	SetRootComponent(DefaultScene);

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(DefaultScene);

	if (const auto SM_Sphere = DT::FindObject<UStaticMesh>(
		DT_MESHES_PATH, FName(TEXT("SM_Sphere"))))
	{
		Sphere->SetStaticMesh(SM_Sphere);
	}

	if (const auto MI_EnemySpawnLocation = DT::FindObject<UMaterialInstance>(
		DT_MATERIAL_PATH, FName(TEXT("MI_EnemySpawnLocation"))))
	{
		Sphere->SetMaterial(0, MI_EnemySpawnLocation);
	}


	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetRelativeRotation(FRotator(0.0, -179.0f, 0.0f));
	Arrow->ArrowColor = FColor(255, 0, 0, 255);
	Arrow->bHiddenInGame = false;
	Arrow->SetupAttachment(Sphere);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Sphere);
	Box->SetBoxExtent(FVector(2.0f, 2.0f, 100.0f));
	Box->SetVisibility(false);
}

void AEnemySpawnLocation::BeginPlay()
{
	Super::BeginPlay();
}
