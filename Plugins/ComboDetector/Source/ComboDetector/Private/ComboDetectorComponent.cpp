// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboDetectorComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UComboDetectorComponent::UComboDetectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UComboDetectorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (bWasInitiated)
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UComboDetectorComponent::DetectCombo(EComboInput In)
{
	if (bPauseInputs)
		return;

	if (Nodes.Num() == 0)
		return;

	uint8 Input = (uint8)In;

	if (Nodes[CurrentNode].Contains(Input))
	{
		ComboQueue.Enqueue(Nodes[CurrentNode][Input]);
		CurrentNode = Nodes[CurrentNode][Input].NextNode;

		if (!bIsExecutingCombo)
		{
			bIsExecutingCombo = true;
			ExecuteCombo();
		}

		if (CurrentNode == -1)
		{
			bPauseInputs = true;
			bComboWasCompleted = true;
		}
	}
	else
	{
		if (bIsExecutingCombo)
			bPauseInputs = true;
	}
}

void UComboDetectorComponent::CreateNodes()
{
	Nodes.Empty();

	if (!CombosData)
		return;

	Nodes.Add(FKeyNode());

	int32 C_Node = 0;

	for (int32 cb = 0; cb < CombosData->AllCombos.Num(); cb++)
	{
		C_Node = 0;

		const TArray<FComboWrapper>& CurrentCombo = CombosData->AllCombos[cb].Combo;

		for (int32 i = 0; i < CurrentCombo.Num(); i++)
		{
			uint8 CurrentKey = (uint8)CurrentCombo[i].Inputs;

			if (!Nodes[C_Node].Contains(CurrentKey))
			{
				if (i < CurrentCombo.Num() - 1)
				{
					Nodes[C_Node].Add(CurrentKey, FCurrentCombo(Nodes.Num(), CurrentCombo[i]));
					Nodes.Add(FKeyNode());
					C_Node = Nodes.Num() - 1;
				}
				else
				{
					Nodes[C_Node].Add(CurrentKey, FCurrentCombo(-1, CurrentCombo[i]));
				}
			}
			else
			{
				C_Node = Nodes[C_Node][CurrentKey].NextNode;
			}
		}
	}
}

void UComboDetectorComponent::ExecuteCombo()
{
	FCurrentCombo Combo;
	ComboQueue.Dequeue(Combo);

	GetWorld()->GetTimerManager().SetTimer(DequeNextComboHandle, this, &UComboDetectorComponent::ExecuteNextCombo, Combo.ComboToExecute.Duration, false);

	ExecuteComboEvent.Broadcast(Combo.ComboToExecute.Animation, Combo.ComboToExecute.PlayRate);
}

void UComboDetectorComponent::Init()
{
	CreateNodes();
	bWasInitiated = true;
}

void UComboDetectorComponent::ExecuteNextCombo()
{
	if (ComboQueue.IsEmpty())
	{
		CurrentNode = 0;
		bPauseInputs = false;
		bIsExecutingCombo = false;

		if (bComboWasCompleted)
		{
			bComboWasCompleted = false;
			ExecutedComboEvent.Broadcast();
		}

		ForceStopAnimEvent.Broadcast();
	}
	else
	{
		ExecuteCombo();
	}
}