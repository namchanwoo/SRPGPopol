

#pragma region AGrid

/*void AGrid::ReCreateSlots()
{
	RemoveAllSlots();

	SRPG_IF_CHECK(SlotClass, TEXT("SlotClass가 설정되지 않았습니다. 설정해주세요."));
	SRPG_IF_CHECK(InnerSlotClass, TEXT("InnerSlotClass가 설정되지 않았습니다. 설정해주세요."));

	UWorld* World = GetWorld();
	SRPG_CHECK(World);

	for (int32 Row = 0; Row < NumberOfRows; ++Row)
	{
		for (int32 Column = 0; Column < NumberOfColumns; ++Column)
		{
			const ASlot* SlotCDO = SlotClass->GetDefaultObject<ASlot>();
			FVector SpawnVector = FVector(Column * (SlotCDO->SlotSetting.SlotSize.X * 2.0f),
			                              Row * (SlotCDO->SlotSetting.SlotSize.Y * -2.0f),
			                              0.0f);

			auto SlotTransform = FTransform(FRotator::ZeroRotator, SpawnVector + GetActorTransform().GetLocation());
			if (Column == 0 && (Row == (NumberOfRows - 1)))
			{
				SetHeroSpawnPoint(SlotTransform);
			}

			FActorSpawnParameters SpawnParams;
			FTransform SpawnTransform = SlotTransform;
			ASlot* NewSlot = World->SpawnActor<ASlot>(SlotClass, SpawnTransform, SpawnParams);

			const FVector2D NewCoordinates = FVector2D(Column, Row);
			NewSlot->Coordinates = NewCoordinates;
			NewSlot->Grid = this;
			Slots.Add(NewCoordinates, NewSlot);

			NewSlot->InitializeEvent(NewSlot->SlotSetting);
			NewSlot->OnMouseOverSlot.AddDynamic(this, &AGrid::OnMouseOverSlot);
			NewSlot->OnSlotClicked.AddDynamic(this, &AGrid::OnSlotClicked);
			NewSlot->OnMouseLeaveSlot.AddDynamic(this, &AGrid::OnMouseLeaveSlot);

			if (NewSlot->Coordinates.X < PlayerSpawnColumn)
			{
				NewSlot->SetPlayerSpawnSlot();
			}
			NewSlot->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));


			AGridOverlaySlot* NewOverlaySlot = World->SpawnActor<AGridOverlaySlot>(
				OverlaySlotClass, SpawnTransform, SpawnParams);
			NewOverlaySlot->Coordinates = NewCoordinates;
			NewOverlaySlot->Grid = this;
			OverlaySlots.Add(NewCoordinates, NewOverlaySlot);
			NewOverlaySlot->InitializeEvent(NewSlot->SlotSetting);
			NewOverlaySlot->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
		}
	}

	for (int32 Row = 0; NumberOfRows - 1; ++Row)
	{
		for (int32 Column = 0; NumberOfColumns - 1; ++Column)
		{
			const ASlot* SlotCDO = SlotClass->GetDefaultObject<ASlot>();
			FVector SpawnVector = FVector(Column * (SlotCDO->SlotSetting.SlotSize.X * 2.0f),
			                              Row * (SlotCDO->SlotSetting.SlotSize.Y * -2.0f),
			                              0.0f);
			SpawnVector += FVector(SlotCDO->SlotSetting.SlotSize.X, SlotCDO->SlotSetting.SlotSize.Y * -1.0f, 0.0f);


			FActorSpawnParameters SpawnParams;
			FTransform SpawnTransform = FTransform(FRotator::ZeroRotator,
			                                       SpawnVector + GetActorTransform().GetLocation());
			AInnerSlot* NewInnerSlot = World->SpawnActor<AInnerSlot>(InnerSlotClass, SpawnTransform, SpawnParams);

			FVector2D NewCoordinates = FVector2D(Column, Row);
			NewInnerSlot->Coordinates = NewCoordinates;
			NewInnerSlot->Grid = this;
			NewInnerSlot->InitializeEvent(SlotCDO->SlotSetting);
			NewInnerSlot->OnMouseOverSlot.AddDynamic(this, &AGrid::OnMouseOverSlot);
			NewInnerSlot->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
			InnerSlots.Add(NewCoordinates, NewInnerSlot);
			
			AInnerOverlaySlot* NewInnerOverlaySlot = World->SpawnActor<AInnerOverlaySlot>(
				InnerOverlaySlotClass, SpawnTransform, SpawnParams);
			NewInnerOverlaySlot->Coordinates = NewCoordinates;
			NewInnerOverlaySlot->Grid = this;
			NewInnerOverlaySlot->InitializeEvent(SlotCDO->SlotSetting);
			NewInnerOverlaySlot->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
			InnerOverlaySlots.Add(NewCoordinates, NewInnerOverlaySlot);
		}
	}
}*/

/*void AGrid::RemoveAllSlots()
{
	TArray<ASlotBase*> SlotValues;
	Slots.GenerateValueArray(SlotValues);
	for (int i = SlotValues.Num() - 1; i >= 0; --i)
	{
		SlotValues[i]->ClearAllCallbacks();
		SlotValues[i]->Destroy();
	}
	Slots.Empty();

	TArray<ASlotBase*> OverlaySlotValues;
	OverlaySlots.GenerateValueArray(OverlaySlotValues);
	for (int i = OverlaySlotValues.Num() - 1; i >= 0; --i)
	{
		OverlaySlotValues[i]->Destroy();
	}
	OverlaySlots.Empty();

	TArray<ASlotBase*> InnerSlotValues;
	InnerSlots.GenerateValueArray(InnerSlotValues);
	for (int i = InnerSlotValues.Num() - 1; i >= 0; --i)
	{
		InnerSlotValues[i]->ClearAllCallbacks();
		InnerSlotValues[i]->Destroy();
	}
	InnerSlots.Empty();

	TArray<ASlotBase*> InnerOverlaySlotValues;
	InnerOverlaySlots.GenerateValueArray(InnerOverlaySlotValues);
	for (int i = InnerOverlaySlotValues.Num() - 1; i >= 0; --i)
	{
		InnerOverlaySlotValues[i]->Destroy();
	}
	InnerOverlaySlots.Empty();

	HeroSpawnLocation->Destroy();

	TArray<AActor*> OutActors;
	GetAttachedActors(OutActors);
	for (int i = OutActors.Num() - 1; i >= 0; --i)
	{
		OutActors[i]->Destroy();
	}
}*/

/*
void AGrid::OnMovableSlotTimeline_Detect()
{
	ASlotBase* ClosestSlotToMouse =
		GetClosestSlotToMouseImproved(LocalCurrentMouseOverSlot, LocalCurrentMouseOverSlot->CanAttackFromSlots);
	if (CurrentSlotUnderMouse == ClosestSlotToMouse) { return; }

	ResetCurrentSlotUnderMouse();
	CurrentSlotUnderMouse = ClosestSlotToMouse;
	if (!IsValid(CurrentSlotUnderMouse)) { return; }

	ShowDamageDetailsUI(LocalCurrentMouseOverSlot, CurrentSlotUnderMouse);

	if (bIsHexGrid)
	{
		if (OnCursorChanged.IsBound())
		{
			OnCursorChanged.Broadcast(EBattleCursorType::MeleeAttack, BattleControllerRef->CurrentActiveAbility,
			                          nullptr, nullptr);
		}

		bool AllFree;
		TArray<ASlotBase*> OutSlotsInRange;
		GetSlotsInRange(CurrentSlotUnderMouse, BattleControllerRef->PlayingCharacter->UnitSize,
		                false, OutSlotsInRange, AllFree);
		auto ClosetSlot = GetClosestSlotInRange(LocalCurrentMouseOverSlot, OutSlotsInRange);

		if (OnCursorAngleChanged.IsBound())
		{
			float NewAngle = UKismetMathLibrary::FindLookAtRotation(ClosetSlot->GetActorLocation(),
			                                                        LocalCurrentMouseOverSlot->GetActorLocation()).Yaw;

			NewAngle -= UGameplayStatics::GetPlayerController(this, 0)->
			            PlayerCameraManager->GetCameraRotation().Yaw + 90.0f;
			OnCursorAngleChanged.Broadcast(NewAngle);
		}
	}
	else
	{
		if (OnCursorChanged.IsBound())
		{
			OnCursorChanged.Broadcast(EBattleCursorType::MeleeAttack, BattleControllerRef->CurrentActiveAbility,
			                          nullptr, nullptr);
		}

		bool AllFree;
		TArray<ASlotBase*> OutSlotsInRange;
		GetSlotsInRange(LocalCurrentMouseOverSlot->ContainedUnit->Slot,
		                LocalCurrentMouseOverSlot->ContainedUnit->UnitSize, false,
		                OutSlotsInRange, AllFree);

		const auto ClosetSlot = GetClosestSlotInRange(CurrentSlotUnderMouse, OutSlotsInRange);

		if (OnCursorAngleChanged.IsBound())
		{
			float NewAngle = UKismetMathLibrary::FindLookAtRotation(CurrentSlotUnderMouse->GetActorLocation(),
			                                                        ClosetSlot->GetActorLocation()).Yaw;

			NewAngle -= UGameplayStatics::GetPlayerController(this, 0)->
			            PlayerCameraManager->GetCameraRotation().Yaw + 90.0f;
			OnCursorAngleChanged.Broadcast(NewAngle);
		}
	}

	if (BattleControllerRef->PlayingCharacter->Slot == CurrentSlotUnderMouse)
	{
		return;
	}

	ShowMovementPath(BattleControllerRef->PlayingCharacter, CurrentSlotUnderMouse);
	HighlightSlot(CurrentSlotUnderMouse, BattleControllerRef->PlayingCharacter->UnitSize, true);
}
*/

/*
void AGrid::OnMouseOverSlot(ASlotBase* InSlot, bool bIsInnerSlot)
{
	m_CurrentMouseOverSlot = InSlot;
	if (BattleControllerRef->BattleState == EBattleState::DeploymentPhase)
	{
		if (MouseOverSlot.IsBound())
		{
			MouseOverSlot.Broadcast(InSlot);
		}
	}
	else if (BattleControllerRef->BattleState == EBattleState::WaitingForPlayerAction)
	{
		StopDetectingClosestMovableSlot();
		ResetCurrentSlotUnderMouse();

		if (bAreaTargetAbilityEnabled)
		{
			ShowAreaTargetAbilitySlots(InSlot);
			return;
		}

		if (IsValid(BattleControllerRef->CurrentSpell))
		{
			ShowSpellSlots(InSlot);
			return;
		}

		ACharacterBase* OutAllyCharacter;
		if (bAllyTargetAbilityEnabled && AllySlots.Contains(InSlot) &&
			InSlot->IsAllySlot(BattleControllerRef->PlayingCharacter, OutAllyCharacter))
		{
			RemoveHighlightOnCharacters();
			if (OnCursorChanged.IsBound())
			{
				OnCursorChanged.Broadcast(EBattleCursorType::Spell, BattleControllerRef->CurrentActiveAbility,
				                          nullptr, nullptr);
			}
			BattleControllerRef->CurrentActiveAbility->SetTargetSlot(InSlot);

			TArray<ASlotBase*> OutSlotsInRange, OutEmptySlotsInRange;
			TArray<ACharacterBase*> OutEnemiesInRange;
			BattleControllerRef->CurrentActiveAbility->GetAbilitySlots(OutSlotsInRange, OutEmptySlotsInRange,
			                                                           HighlightedCharacters, OutEnemiesInRange);

			for (ACharacterBase* InHighlightedCharacter : HighlightedCharacters)
			{
				InHighlightedCharacter->Highlight(true);
			}
			return;
		}
		
		if (SlotsInRange.Contains(InSlot))
		{
			HideDamageDetailsUI();
			if (IsValid(BattleControllerRef)) { return; }

			ASlotBase* ClosestMovableSlot = GetClosestMovableSlot(
				InSlot, BattleControllerRef->PlayingCharacter->UnitSize, MovableSlots);
			if (!IsValid(ClosestMovableSlot)) { return; }

			CurrentSlotUnderMouse = ClosestMovableSlot;
			ShowMovementPath(BattleControllerRef->PlayingCharacter, CurrentSlotUnderMouse);
			HighlightSlot(CurrentSlotUnderMouse, BattleControllerRef->PlayingCharacter->UnitSize, true);

			if (OnCursorChanged.IsBound())
			{
				EBattleCursorType SelectCursorType = {};
				if (BattleControllerRef->PlayingCharacter->MovementType == EMovementType::Walk)
					SelectCursorType = EBattleCursorType::Walk;
				else if (BattleControllerRef->PlayingCharacter->MovementType == EMovementType::Fly)
					SelectCursorType = EBattleCursorType::Fly;
				else if (BattleControllerRef->PlayingCharacter->MovementType == EMovementType::Immobilie)
					SelectCursorType = EBattleCursorType::Default;
				OnCursorChanged.Broadcast(SelectCursorType, nullptr, nullptr, nullptr);
			}
		}

		ACharacterBase* OutEnemyCharacter;
		if ((!bAllyTargetAbilityEnabled && !IsValid(BattleControllerRef->CurrentSpell)) &&
			EnemySlots.Contains(InSlot) &&
			InSlot->IsEnemySlot(BattleControllerRef->PlayingCharacter, OutEnemyCharacter))
		{
			if (bIsMeleeAttack)
			{
				DetectClosestMovableSlotTimeline.PlayFromStart();
				return;
			}

			if (GetDistanceToSlot(BattleControllerRef->PlayingCharacter->Slot, InSlot->ContainedUnit->Slot) >
				BattleControllerRef->RangePenaltyDistance)
			{
				if (OnCursorChanged.IsBound())
				{
					OnCursorChanged.Broadcast(EBattleCursorType::BrokenRangeAttack,
					                          BattleControllerRef->CurrentActiveAbility,
					                          nullptr, nullptr);
				}
			}
			else
			{
				if (OnCursorChanged.IsBound())
				{
					OnCursorChanged.Broadcast(EBattleCursorType::RangeAttack,
					                          BattleControllerRef->CurrentActiveAbility,
					                          nullptr, nullptr);
				}
			}
			ShowDamageDetailsUI(InSlot, InSlot);
			return;
		}
		else
		{
			HideDamageDetailsUI();
		}
	}
}
*/

/*
void AGrid::OnSlotClicked(ASlotBase* InSlot, FKey InButton)
{
	if (InButton == EKeys::RightMouseButton)
	{
		if (ACharacterBase* CastCharacterUnit = Cast<ACharacterBase>(InSlot->ContainedUnit))
		{
			BattleControllerRef->ShowCharacterDetails(CastCharacterUnit);
		}
		return;
	}

	if (InButton == EKeys::LeftMouseButton)
	{
		if (BattleControllerRef->BattleState == EBattleState::DeploymentPhase)
		{
			if (SlotClicked.IsBound())
			{
				SlotClicked.Broadcast(InSlot);
			}
		}
		else if (BattleControllerRef->BattleState == EBattleState::DeploymentPhase)
		{
			StopDetectingClosestMovableSlot();

			if (IsValid(InSlot) && bAreaTargetAbilityEnabled)
			{
				if (InSlot->ContainsObstacle())
				{
					return;
				}

				BattleControllerRef->ActiveAbility(InSlot);
				ResetSlots();
				CurrentSlotUnderMouse = nullptr;
				BattleControllerRef->ChangeBattleState(EBattleState::PlayerIsPlaying);
				return;
			}


			ACharacterBase* OutAllyCharacter;
			if (bAllyTargetAbilityEnabled && AllySlots.Contains(InSlot) &&
				InSlot->IsAllySlot(BattleControllerRef->PlayingCharacter, OutAllyCharacter))
			{
				BattleControllerRef->ActiveAbility(InSlot);
				ResetSlots();
				CurrentSlotUnderMouse = nullptr;
				BattleControllerRef->ChangeBattleState(EBattleState::PlayerIsPlaying);
				return;
			}

			if (IsValid(BattleControllerRef))
			{
				CastSpellOnSlot(InSlot);
				return;
			}

			ACharacterBase* OutEnemyCharacter;
			const bool IsEnemySlot = EnemySlots.Contains(InSlot) &&
				InSlot->IsEnemySlot(BattleControllerRef->PlayingCharacter, OutEnemyCharacter);

			if (!bIsMeleeAttack)
			{
				if (IsEnemySlot)
				{
					BattleControllerRef->RangeAttack(InSlot);
					ResetSlots();
					CurrentSlotUnderMouse = nullptr;
					BattleControllerRef->ChangeBattleState(EBattleState::PlayerIsPlaying);
					return;
				}
			}

			if (IsValid(CurrentSlotUnderMouse))
			{
				if (IsEnemySlot)
				{
					if (!Slots.Contains(CurrentSlotUnderMouse->Coordinates)) { return; }

					if (*Slots.Find(CurrentSlotUnderMouse->Coordinates) == BattleControllerRef->PlayingCharacter->Slot)
					{
						BattleControllerRef->MeleeAttackAction(InSlot);
						ResetSlots();
						CurrentSlotUnderMouse = nullptr;
						BattleControllerRef->ChangeBattleState(EBattleState::PlayerIsPlaying);
						return;
					}
					else
					{
						BattleControllerRef->MoveAction(CurrentSlotUnderMouse, InSlot);
						CurrentSlotUnderMouse = nullptr;
						BattleControllerRef->ChangeBattleState(EBattleState::PlayerIsPlaying);
						return;
					}
				}
				else
				{
					if (!Slots.Contains(CurrentSlotUnderMouse->Coordinates)) { return; }
					if (!MovableSlots.Contains(*Slots.Find(CurrentSlotUnderMouse->Coordinates))) { return; }

					BattleControllerRef->MoveAction(CurrentSlotUnderMouse, nullptr);
					CurrentSlotUnderMouse = nullptr;
					BattleControllerRef->ChangeBattleState(EBattleState::PlayerIsPlaying);
				}
			}
		}
	}
}
*/

/*
void AGrid::ShowMovementForPlayerCharacter(ACharacterBase* InCharacter)
{
	bIsMeleeAttack = CalculateMovableSlots(InCharacter, false, MovableSlots, SlotsInRange, EnemySlots, AllySlots);

	for (TTuple<UE::Math::TVector2<double>, ASlotBase*> InSlot : Slots)
	{
		if (BattleControllerRef->PlayingCharacter->Slot == InSlot.Value)
		{
			HighlightPlayingCharacterSlot(InSlot.Value);
		}
		else
		{
			if (InSlot.Value->IsEmpty())
			{
				if (SlotsInRange.Contains(InSlot.Value))
				{
					InSlot.Value->Movable(true);
				}
				else
				{
					InSlot.Value->Movable(false);
				}
			}
			else
			{
				HighlightEnemySlots(InSlot.Value);
			}
		}
	}

	CheckSlotUnderMouse();
}

*/

/*
void AGrid::ShowMovementPath(ACharacterBase* InCharacter, ASlotBase* InTargetSlot)
{
	PathSpline->ClearSplinePoints(true);
	if (InTargetSlot->Path.IsEmpty())
	{
		return;
	}

	auto AllSlots = GetAllSlots(InCharacter->UnitSize);
	if (!AllSlots.Contains(InCharacter->Slot->Coordinates))
	{
		return;
	}

	const ASlotBase* CharacterCoordSlot = *AllSlots.Find(InCharacter->Slot->Coordinates);
	PathSpline->AddSplinePoint(CharacterCoordSlot->GetActorLocation(), ESplineCoordinateSpace::World);

	for (int i = 0; i < InTargetSlot->Path.Num(); i++)
	{
		const ASlotBase* InTargetPathSlot = InTargetSlot->Path[i];
		if (!AllSlots.Contains(InTargetPathSlot->Coordinates))
		{
			continue;
		}

		const ASlotBase* FindTargetCoordSlot = *AllSlots.Find(InTargetPathSlot->Coordinates);
		PathSpline->AddSplinePoint(FindTargetCoordSlot->GetActorLocation(), ESplineCoordinateSpace::World);

		FVector ArriveTangent = PathSpline->GetArriveTangentAtSplinePoint(i, ESplineCoordinateSpace::Local) * 0.5f;
		FVector LeaveTangent = PathSpline->GetLeaveTangentAtSplinePoint(i, ESplineCoordinateSpace::Local) * 0.5f;
		PathSpline->SetTangentsAtSplinePoint(i, ArriveTangent, LeaveTangent, ESplineCoordinateSpace::Local, true);
	}

	if (!(!BattleControllerRef->PlayingCharacter->bIsAI && !BattleControllerRef->bIsAutoPlay))
	{
		return;
	}

	PathMesh->ClearInstances();
	int32 LastIndex = FMath::Floor((PathSpline->GetSplineLength() / 35.0f));

	for (int32 i = 0; i <= LastIndex; i++)
	{
		auto DistanceSpline = PathSpline->GetLocationAtDistanceAlongSpline(i * 35.0f, ESplineCoordinateSpace::World);
		PathMesh->AddInstance(FTransform(FRotator::ZeroRotator, DistanceSpline,
		                                 FVector(0.75f, 0.75f, 0.75f)), true);

		if (i > 0)
		{
			FTransform OutInstanceTransform;
			PathMesh->GetInstanceTransform(i - 1, OutInstanceTransform, true);
			OutInstanceTransform.SetRotation(
				UKismetMathLibrary::FindLookAtRotation(OutInstanceTransform.GetLocation(),
				                                       DistanceSpline).Quaternion());

			PathMesh->UpdateInstanceTransform(i - 1, OutInstanceTransform, true);
		}
	}

	int32 PathInstanceCount = PathMesh->GetInstanceCount() - 1;
	FTransform OutInstanceTransform;
	PathMesh->GetInstanceTransform(PathInstanceCount, OutInstanceTransform, true);
	UE::Math::TQuat<double> CloseRotation = PathSpline->FindRotationClosestToWorldLocation(
		OutInstanceTransform.GetLocation(), ESplineCoordinateSpace::World).Quaternion();
	OutInstanceTransform.SetRotation(CloseRotation);

	PathMesh->UpdateInstanceTransform(PathInstanceCount, OutInstanceTransform, true);
}
*/

/*
void AGrid::ShowCharacterMovementOverlay(ACharacterBase* InCharacter)
{
	if ((IsValid(InCharacter) && BattleControllerRef->PlayingCharacter != InCharacter) == false)
	{
		return;
	}

	if (CharacterMovementOverlaySlots.Contains(InCharacter))
	{
		MovementOverlaySlotsInRange = CharacterMovementOverlaySlots.Find(InCharacter)->OverlaySlots;
	}
	else
	{
		TArray<ASlotBase*> OutMoveAbleSlots;
		TArray<ASlotBase*> OutSlotsInRange;
		TArray<ASlotBase*> OutEnemySlots;
		TArray<ASlotBase*> OutAllySlot;
		CalculateMovableSlots(InCharacter, true, OutMoveAbleSlots, OutSlotsInRange, OutEnemySlots, OutAllySlot);
		OutSlotsInRange.Append(OutEnemySlots);

		CharacterMovementOverlaySlots.Add(InCharacter, FListOfSlots(OutSlotsInRange));
		MovementOverlaySlotsInRange = OutSlotsInRange;
	}

	for (ASlotBase* InSlot : MovementOverlaySlotsInRange)
	{
		if (InSlot->IsEmpty())
		{
			if (!OverlaySlots.Contains(InSlot->Coordinates))
			{
				continue;
			}

			ASlotBase* FindOverlaySlot = *OverlaySlots.Find(InSlot->Coordinates);
			FindOverlaySlot->SetSlotType(ESlotType::MovementOverlaySlot, 1);
		}
		else
		{
			ACharacterBase* EnemyCharacter;
			if (InSlot->IsEnemySlot(InCharacter, EnemyCharacter) &&
				InSlot->ContainedUnit->Slot == InSlot)
			{
				auto AllSlots = GetAllOverlaySlots(InSlot->ContainedUnit->UnitSize);

				if (!OverlaySlots.Contains(InSlot->Coordinates))
				{
					continue;
				}

				const ASlotBase* FindOverlaySlot = *OverlaySlots.Find(InSlot->Coordinates);
				if (AllSlots.Contains(FindOverlaySlot->Coordinates))
				{
					continue;
				}

				ASlotBase* FindToAllSlot = *AllSlots.Find(FindOverlaySlot->Coordinates);
				HighlightMovementSlots(FindToAllSlot, InSlot->ContainedUnit->UnitSize);
			}
		}
	}
}
*/


/*
void AGrid::HighlightSlot(ASlotBase* InSlot, int32 InSize, bool IsOn)
{
	ASlotBase* CurrentSlot = InSlot;
	int32 CurrentSize = InSize;

	if (!Slots.Contains(CurrentSlot->Coordinates)) { return; }

	if (CurrentSize % 2 == 0 &&
		*Slots.Find(CurrentSlot->Coordinates) != BattleControllerRef->PlayingCharacter->Slot)
	{
		CurrentSlot->HideSlot();
	}

	bool AllFree;
	TArray<ASlotBase*> OutSlotsInRange;
	GetSlotsInRange(CurrentSlot, CurrentSize, false, OutSlotsInRange, AllFree);

	for (ASlotBase* InSlotsInRange : OutSlotsInRange)
	{
		if (IsOn)
		{
			InSlotsInRange->SaveSlotType();
			if (InSlotsInRange != BattleControllerRef->PlayingCharacter->Slot)
			{
				InSlotsInRange->HideSlot();
			}
		}
		else
		{
			if (InSlotsInRange != BattleControllerRef->PlayingCharacter->Slot)
			{
				InSlotsInRange->Highlight(false, 1);
			}
		}
	}

	if (IsOn)
	{
		CurrentSlot->Highlight(true, CurrentSize);
	}
}
*/

/*
bool AGrid::CalculateMovableSlots(ACharacterBase* InCharacter, bool OnlyVisual, TArray<ASlotBase*>& OutMoveAbleSlots,
                                  TArray<ASlotBase*>& OutSlotsInRange, TArray<ASlotBase*>& OutEnemySlots,
                                  TArray<ASlotBase*>& OutAllySlots)
{
	auto CurrentCharacter = InCharacter;
	auto CurrentOnlyVisual = OnlyVisual;
	bool CurrentIsMeleeAttack = false;
	int32 CurrentDistanceCost;

	TArray<ASlotBase*> OpenSlots, ClosedSlots;
	TArray<ASlotBase*> CurrentSlotsInRange, CurrentMovableSlots, CurrentAllySlots, CurrentEnemySlots;

	ARangeCharacterBase* CastRangeCharacter = Cast<ARangeCharacterBase>(CurrentCharacter);
	if (CastRangeCharacter)
		CurrentIsMeleeAttack = CastRangeCharacter->CurrentAmmo <= 0 || IsEnemyAdjacent(CurrentCharacter);
	else
		CurrentIsMeleeAttack = true;

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

	for (TTuple<UE::Math::TVector2<double>, ASlotBase*> InSlotPair : Slots)
	{
		ASlotBase* InSlot = InSlotPair.Value;

		if (CurrentIsMeleeAttack == false)
		{
			ACharacterBase* OutEnemyCharacter;
			if (!InSlot->IsEnemySlot(CurrentCharacter, OutEnemyCharacter))
			{
				ACharacterBase* OutAllyCharacter;
				if ((!InSlot->IsAllySlot(CurrentCharacter, OutAllyCharacter)) == false)
				{
					CurrentAllySlots.AddUnique(InSlot);
				}
			}
			else
			{
				CurrentEnemySlots.AddUnique(InSlot);
			}
		}


		auto CoordDistance = GetDistanceBetweenCoordinates(CurrentCharacter->Slot->Coordinates, InSlot->Coordinates);
		const bool IsMovementRange = CoordDistance <= CurrentCharacter->CurrentMovementRange;
		if ((!InSlot->ContainsObstacle() && CurrentCharacter->Slot != InSlot && IsMovementRange) == false) { continue; }

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

	if (CurrentCharacter->MovementType == EMovementType::Walk)
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
				TArray<ASlotBase*> SelectPath = CurrentOnlyVisual
					                                ? CurrentNeighbourSlot->OverlayPath
					                                : CurrentNeighbourSlot->Path;

				if ((!ClosedSlots.Contains(CurrentNeighbourSlot) && SelectPath.Num() == 0) == false) { continue; }
				if (!CurrentMovableSlots.Contains(CurrentNeighbourSlot)) { continue; }
				if (!CanUnitFitTo(CurrentNeighbourSlot, CurrentCharacter, OutAllSlotsUnitWillStandOn)) { continue; }

				CurrentDistanceCost = GetDistanceCost(CurrentSlot, CurrentNeighbourSlot) + CurrentSlot->TotalCost;

				if ((CurrentDistanceCost <= CurrentCharacter->CurrentMovementRange * 10) == false) { continue; }

				CurrentNeighbourSlot->TotalCost = CurrentDistanceCost;
				OpenSlots.Add(CurrentNeighbourSlot);

				if (CurrentOnlyVisual)
				{
					CurrentNeighbourSlot->OverlayPath = CurrentSlot->OverlayPath;
					CurrentNeighbourSlot->OverlayPath.Add(CurrentNeighbourSlot);
					continue;
				}

				//!CurrentOnlyVisual
				CurrentNeighbourSlot->Path = CurrentSlot->Path;
				CurrentNeighbourSlot->Path.Add(CurrentNeighbourSlot);

				if (InnerSlots.Contains(CurrentSlot->Coordinates) &&
					InnerSlots.Contains(CurrentNeighbourSlot->Coordinates))
				{
					auto FindInnerSlot = *InnerSlots.Find(CurrentSlot->Coordinates);
					auto FindNeighbourInnerSlot = *InnerSlots.Find(CurrentNeighbourSlot->Coordinates);
					FindNeighbourInnerSlot->Path = FindInnerSlot->Path;
					FindNeighbourInnerSlot->Path.Add(FindNeighbourInnerSlot);
				}
			} //


			NeighbourSlots = GetNeighbourSlots(CurrentSlot, CurrentCharacter->UnitSize, false);
			for (ASlotBase* InNeighbourSlot : NeighbourSlots)
			{
				ACharacterBase* OutEnemyCharacter;
				if (InNeighbourSlot->IsEnemySlot(CurrentCharacter, OutEnemyCharacter))
				{
					bool AllFree;
					GetSlotsInRange(InNeighbourSlot->ContainedUnit->Slot, InNeighbourSlot->ContainedUnit->UnitSize,
					                false, OutSlotsInRange, AllFree);

					for (ASlotBase* InRangeSlot : OutSlotsInRange)
					{
						CurrentEnemySlots.AddUnique(InRangeSlot);
						if (!CurrentOnlyVisual)
						{
							(bIsHexGrid ? InNeighbourSlot : InRangeSlot)->AddSlotToCanAttackFromSlots(CurrentSlot);
						}
					}
					continue;
				}

				ACharacterBase* OutAllyCharacter;
				if (InNeighbourSlot->IsAllySlot(CurrentCharacter, OutAllyCharacter))
				{
					bool AllFree;
					GetSlotsInRange(InNeighbourSlot->ContainedUnit->Slot, InNeighbourSlot->ContainedUnit->UnitSize,
					                false, OutSlotsInRange, AllFree);

					for (ASlotBase* InRangeSlot : OutSlotsInRange)
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

		OutMoveAbleSlots = ClosedSlots;
		OutSlotsInRange = CurrentSlotsInRange;
		OutEnemySlots = CurrentEnemySlots;
		OutAllySlots = CurrentAllySlots;
		return CurrentIsMeleeAttack;
	}


	if (CurrentCharacter->MovementType == EMovementType::Fly)
	{
		CurrentMovableSlots.Add(CurrentCharacter->Slot);
		for (int i = CurrentMovableSlots.Num() - 1; i >= 0; --i)
		{
			ASlotBase* CurrentSlot = CurrentMovableSlots[i];

			TArray<ASlotBase*> OutAllSlotsUnitWillStandOn;
			if (CanUnitFitTo(CurrentSlot, CurrentCharacter, OutAllSlotsUnitWillStandOn))
			{
				if (CurrentSlot != CurrentCharacter->Slot)
				{
					for (ASlotBase* InStandSlot : OutAllSlotsUnitWillStandOn)
					{
						CurrentSlotsInRange.AddUnique(InStandSlot);
					}
				}

				auto NeighbourSlots = GetNeighbourSlots(CurrentSlot, CurrentCharacter->UnitSize, false);

				for (ASlotBase* CurrentNeighbourSlot : NeighbourSlots)
				{
					ACharacterBase* OutEnemyCharacter;
					ACharacterBase* OutAllyCharacter;
					if (CurrentNeighbourSlot->IsEnemySlot(CurrentCharacter, OutEnemyCharacter))
					{
						bool AllFree;
						GetSlotsInRange(CurrentNeighbourSlot->ContainedUnit->Slot,
						                CurrentNeighbourSlot->ContainedUnit->UnitSize, false,
						                OutSlotsInRange, AllFree);

						for (ASlotBase* InRangeSlot : OutSlotsInRange)
						{
							CurrentEnemySlots.AddUnique(InRangeSlot);
							if ((!CurrentOnlyVisual) == false) continue;
							(bIsHexGrid ? CurrentNeighbourSlot : InRangeSlot)->AddSlotToCanAttackFromSlots(CurrentSlot);
						}
					}
					else if (CurrentNeighbourSlot->IsAllySlot(CurrentCharacter, OutAllyCharacter))
					{
						bool AllFree;
						GetSlotsInRange(CurrentNeighbourSlot->ContainedUnit->Slot,
						                CurrentNeighbourSlot->ContainedUnit->UnitSize, false,
						                OutSlotsInRange, AllFree);

						for (ASlotBase* InRangeSlot : OutSlotsInRange)
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
		OutMoveAbleSlots = ClosedSlots;
		OutSlotsInRange = CurrentSlotsInRange;
		OutEnemySlots = CurrentEnemySlots;
		OutAllySlots = CurrentAllySlots;
		return CurrentIsMeleeAttack;
	}


	if (CurrentCharacter->MovementType == EMovementType::Immobilie)
	{
		auto CurrentSlot = CurrentCharacter->Slot;

		TArray<ASlotBase*> NeighbourSlots = GetNeighbourSlots(CurrentSlot, CurrentCharacter->UnitSize, false);
		for (ASlotBase* CurrentNeighbourSlot : NeighbourSlots)
		{
			ACharacterBase* OutEnemyCharacter;
			ACharacterBase* OutAllyCharacter;
			if (CurrentNeighbourSlot->IsEnemySlot(CurrentCharacter, OutEnemyCharacter))
			{
				bool AllFree;
				GetSlotsInRange(CurrentNeighbourSlot->ContainedUnit->Slot,
				                CurrentNeighbourSlot->ContainedUnit->UnitSize,
				                false, OutSlotsInRange, AllFree);

				for (ASlotBase* InRangeSlot : OutSlotsInRange)
				{
					CurrentEnemySlots.AddUnique(InRangeSlot);
					if ((!CurrentOnlyVisual) == false) { continue; }

					(bIsHexGrid ? CurrentNeighbourSlot : InRangeSlot)->AddSlotToCanAttackFromSlots(CurrentSlot);
				}
			}
			else if (CurrentNeighbourSlot->IsAllySlot(CurrentCharacter, OutAllyCharacter))
			{
				bool AllFree;
				GetSlotsInRange(CurrentNeighbourSlot->ContainedUnit->Slot,
				                CurrentNeighbourSlot->ContainedUnit->UnitSize,
				                false, OutSlotsInRange, AllFree);

				for (ASlotBase* InRangeSlot : OutSlotsInRange)
				{
					CurrentAllySlots.AddUnique(InRangeSlot);
				}
			}
		} //#for end

		OutMoveAbleSlots = TArray<ASlotBase*>();
		OutSlotsInRange = TArray<ASlotBase*>();
		OutEnemySlots = CurrentEnemySlots;
		OutAllySlots = CurrentAllySlots;
		return CurrentIsMeleeAttack;
	}
	return false;
}*/

#pragma endregion AGrid

#pragma region BattleController


/*
void ABattleController::SpawnHero()
{
	if (IsValid(SRPGGameInstance->RpgSaveGame))
	{
		int32 loadExp, loadGold, loadCurrentMana;
		TArray<FPlayerCharacterData> loadPlayerCharacterList;
		TSubclassOf<AExploreHeroBase> loadExploreHero;
		TMap<TSubclassOf<AQuestBase>, FQuestStepData> loadActiveQuestData;
		TMap<TSubclassOf<AQuestBase>, int32> loadCompletedQuestData;
		TArray<TSubclassOf<AEquipmentBase>> loadBackPack, loadEquipment;
		SRPGGameInstance->RpgSaveGame->LoadHero(loadExp, loadGold, loadCurrentMana, loadPlayerCharacterList,
		                                        loadExploreHero, loadActiveQuestData, loadCompletedQuestData,
		                                        loadBackPack, loadEquipment);


		const AExploreHeroBase* loadExploreHeroCDO = loadExploreHero->GetDefaultObject<AExploreHeroBase>();
		const int32 heroLevel = USRPGFunctionLibrary::GetHeroLevel(loadExp, loadExploreHeroCDO->ExpExponent,
		                                                           loadExploreHeroCDO->MaxLevel);
		const FHeroStats heroStats = USRPGFunctionLibrary::CalculateHeroStats(heroLevel, loadExploreHeroCDO->MaxLevel,
		                                                                      loadExploreHeroCDO->InitialStats,
		                                                                      loadExploreHeroCDO->FirstLevelStats,
		                                                                      loadExploreHeroCDO->LastLevelStats);

		UWorld* World = GetWorld();
		SRPG_CHECK(World);

		const FTransform spawnTransform = Grid->GetHeroSpawnLocation()->GetActorTransform();
		BattleHero = World->SpawnActorDeferred<ABattleHeroBase>(loadExploreHeroCDO->BattleHero, spawnTransform);
		SRPG_CHECK(BattleHero)

		BattleHero->BattleController = this;
		BattleHero->HeroStats = heroStats;
		BattleHero->BackPack = loadBackPack;
		BattleHero->Equipment = loadEquipment;
		BattleHero->Level = heroLevel;
		BattleHero->Gold = loadGold;
		BattleHero->CurrentMana = loadCurrentMana;
		BattleHero->Name = loadExploreHeroCDO->Name;
		BattleHero->HeroImage = loadExploreHeroCDO->HeroImage;
		BattleHero->HeroDetailsImage = loadExploreHeroCDO->HeroDetailsImage;
		BattleHero->Spells = loadExploreHeroCDO->Spells;
		UGameplayStatics::FinishSpawningActor(BattleHero, spawnTransform);

		BattleHero->InitializeEvent();
		BattleHero->OnRightClicked.AddDynamic(this, &ABattleController::ShowHeroDetails);

		Grid->GetHeroSpawnLocation()->Destroy();
	}
	else
	{
		SpawnDebugHero();
	}
}

void ABattleController::SpawnDebugHero()
{
	const AExploreHeroBase* DebugHeroCDO = DebugHero->GetDefaultObject<AExploreHeroBase>();
	const int32 HeroLevel = USRPGFunctionLibrary::GetHeroLevel(DebugHeroCDO->Exp, DebugHeroCDO->ExpExponent,
															   DebugHeroCDO->MaxLevel);

	const FHeroStats HeroStats = USRPGFunctionLibrary::CalculateHeroStats(HeroLevel, DebugHeroCDO->MaxLevel,
																		  DebugHeroCDO->InitialStats,
																		  DebugHeroCDO->FirstLevelStats,
																		  DebugHeroCDO->LastLevelStats);

	UWorld* World = GetWorld();
	if (!World)
	{
		SRPG_LOG_ERROR(TEXT("World를 가져올 수 없습니다. 함수를 종료합니다."));
		return;
	}

	const auto SpawnTransform = Grid->GetHeroSpawnLocation()->GetActorTransform();
	BattleHero = World->SpawnActorDeferred<ABattleHeroBase>(DebugHeroCDO->BattleHero, SpawnTransform);
	SRPG_CHECK(BattleHero)
	BattleHero->BattleController = this;
	BattleHero->HeroStats = HeroStats;
	BattleHero->Level = HeroLevel;
	BattleHero->CurrentMana = HeroStats.Knowledge * 10;
	BattleHero->Gold = DebugHeroCDO->Gold;
	BattleHero->BackPack = DebugHeroCDO->BackPack;
	BattleHero->Equipment = DebugHeroCDO->Equipment;
	BattleHero->Name = DebugHeroCDO->Name;
	BattleHero->HeroImage = DebugHeroCDO->HeroImage;
	BattleHero->HeroDetailsImage = DebugHeroCDO->HeroDetailsImage;
	BattleHero->Spells = DebugHeroCDO->Spells;
	UGameplayStatics::FinishSpawningActor(BattleHero, SpawnTransform);

	BattleHero->InitializeEvent();
	BattleHero->OnRightClicked.AddDynamic(this, &ABattleController::ShowHeroDetails);
	
	Grid->GetHeroSpawnLocation()->Destroy();
}
*/




#pragma endregion BattleController