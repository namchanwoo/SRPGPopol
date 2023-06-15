﻿#include "AITerritory.h"

#include "Kismet/KismetMathLibrary.h"
#include "SRGCore/Utilities/AssetTableRef.h"
#include "SRGCore/Utilities/ColorNames.h"


AAITerritory::AAITerritory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->SetCollisionProfileName(TEXT("Custom"));
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sphere->SetCollisionObjectType(ECC_WorldDynamic);
	Sphere->SetHiddenInGame(true);
	Sphere->SetupAttachment(RootComponent);

	if (UStaticMesh* SM_Sphere = DT::FindObject<UStaticMesh>(DT_MESHES_PATH, FName(TEXT("SM_Sphere"))))
	{
		Sphere->SetStaticMesh(SM_Sphere);
	}

	if (UMaterialInstance* MI_AITerritory = DT::FindObject<UMaterialInstance>(DT_MATERIAL_PATH, FName(TEXT("MI_AITerritory"))))
	{
		Sphere->SetMaterial(0, MI_AITerritory);
	}

	Color = SRPG_COLOR_SKY_BLUE;
}

void AAITerritory::OnConstruction(const FTransform& Transform)
{
	// Super::OnConstruction(Transform);
	
	Sphere->SetWorldScale3D(FVector(Radius));
	UMaterialInstanceDynamic* SphereMaterialInstance = Sphere->CreateDynamicMaterialInstance(0);
	SphereMaterialInstance->SetVectorParameterValue(FName(TEXT("color")), Color);
}

void AAITerritory::BeginPlay()
{
	Super::BeginPlay();
}

void AAITerritory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
