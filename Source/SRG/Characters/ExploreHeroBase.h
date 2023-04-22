// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SRGCore/SRGEnumStruct.h"
#include "ExploreHeroBase.generated.h"

class AExploreNavigationPath;
class UBoxComponent;
class USphereComponent;
class AEquipmentBase;
class UUW_QuestNotificationUI;
class AExplorePlayerController;

UCLASS()
class SRG_API AExploreHeroBase : public ACharacter
{
	GENERATED_BODY()

public:
	AExploreHeroBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region   	Explore Hero Setup

public:
	void SetCharacters();

	void SetFullMana();

#pragma endregion	Explore Hero Setup


#pragma region   	Interfactions

public:
	void DetectInteraction(AActor* Actor, UPrimitiveComponent* PrimitiveComponent);

	void RefreshInteractions();

	void AddGold(int32 Amount);

	void RemoveGold(int32 Amount);

#pragma endregion	Interfactions


#pragma region   	Input Event

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Explore Hero")
	void CheckMovement();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Explore Hero")
	void StopCheckingMovement();

#pragma endregion	Input Event


#pragma region   	Path Event
public:
	UFUNCTION(BlueprintCallable,Category="Explore Hero")
	void ShowPath(FVector TargetLocation);

#pragma endregion	Path Event


#pragma region   	OnFunction

protected:
	UFUNCTION()
	void OnInteractionDetectorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractionDetectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                                     int32 OtherBodyIndex);

	UFUNCTION()
	void OnRightClickDetectorBeginCursorOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnRightClickDetectorEndCursorOver(UPrimitiveComponent* TouchedComponent);

#pragma endregion	OnFunction


#pragma region   	Field Members

public:
	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	bool bContinuousMovement = true;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	bool bIsInputBlocked;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	int32 Exp;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	int32 MaxLevel = 10;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	int32 CurrentMana;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	int32 MaxCharacterLimit = 7;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	int32 Gold;


	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	float MovementSpeed = 400.f;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	float ExpExponent = 15.0f;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	FRotator InitialRotation;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	FHeroStats InitialStats;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	FHeroStats FirstLevelStats;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	FHeroStats LastLevelStats;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	TArray<FPlayerCharacterData> PlayerCharacterList;


	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase|Component")
	USphereComponent* InteractionDetector;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase|Component")
	UBoxComponent* RightClickDetector;


	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase|Setting")
	TSubclassOf<UUW_QuestNotificationUI> QuestNotificationUIClass;


	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase|Reference")
	AExplorePlayerController* ExplorePlayerController;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase|Reference")
	UUW_QuestNotificationUI* QuestNotificationUI;
	
	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase|Reference")
	AExploreNavigationPath* NavigationPath;


#pragma endregion	Field Members
};
