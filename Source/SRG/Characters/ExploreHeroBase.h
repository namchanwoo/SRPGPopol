// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SRG/Interactables/QuestNPC/QuestNPCBase.h"
#include "SRGCore/SRGEnumStruct.h"
#include "ExploreHeroBase.generated.h"

class ABattleQuestBase;
class AEnemyExplorePawnBase;
class AQuestBase;
class UCameraComponent;
class USpringArmComponent;
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

	/*ToDo:구현해야함*/
	void UpdateInteractionQuest(AActor* Actor);

	/*ToDo:구현해야함*/
	EQuestStatus GetQuestStatus(TSubclassOf<AQuestBase> InQuest);

	/*ToDo:구현해야함*/
	void AddQuest(AQuestBase* NewQuest);

	/*ToDo:구현해야함*/
	void LoadQuests(AQuestBase* InQuest);
	
	/*ToDo:구현해야함*/
	void DeliverQuest(AQuestBase* InQuest);

	/*ToDo:구현해야함*/
	ABattleQuestBase* UpdateBattleQuest(AEnemyExplorePawnBase* EnemyPawn);

	/*ToDo:구현해야함*/
	void UpdateQuest(AQuestBase* InQuest, int StepsProgress, bool OverrideProgress, bool IgnoreNotification);

	/*ToDo:구현해야함*/
	void AddExp(int32 InExperience);
#pragma endregion	Interfactions


#pragma region   	Input Event

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Explore Hero")
	void CheckMovement();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Explore Hero")
	void StopCheckingMovement();

#pragma endregion	Input Event


#pragma region   	Path Event

public:
	UFUNCTION(BlueprintCallable, Category="Explore Hero")
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

	/*---	      	    Setting    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExploreHeroBase|Setting")
	TSubclassOf<UUW_QuestNotificationUI> QuestNotificationUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExploreHeroBase|Setting|Animation")
	UAnimSequence* IdleAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExploreHeroBase|Setting|Animation")
	UAnimSequence* WalkAnimation;
	
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
	float TargetZoom = 800.0f;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	float MinZoom = 400.0f;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	float MaxZoom = 1200.0f;

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
	TArray<TSubclassOf<AEquipmentBase>> BackPack;
	
	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	TArray<TSubclassOf<AEquipmentBase>> Equipment;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	TArray<FPlayerCharacterData> PlayerCharacterList;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	TMap<TSubclassOf<AQuestBase>,FQuestStepData> ActiveQuestData;
	
	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase")
	TMap<TSubclassOf<AQuestBase>,int32> CompletedQuestData;
	

	/*---	      	    Component    	      	---*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ExploreHeroBase|Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ExploreHeroBase|Component")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ExploreHeroBase|Component")
	USphereComponent* InteractionDetector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ExploreHeroBase|Component")
	UBoxComponent* RightClickDetector;


	/*---	      	    Ref    	      	---*/
public:
	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase|Reference")
	AExplorePlayerController* ExplorePlayerController;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase|Reference")
	UUW_QuestNotificationUI* QuestNotificationUI;

	UPROPERTY(BlueprintReadWrite, Category="ExploreHeroBase|Reference")
	AExploreNavigationPath* NavigationPath;


#pragma endregion	Field Members
};
