// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombosDataAsset.h"
#include "ComboDetectorComponent.generated.h"

class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FExecuteComboDelegate, UAnimMontage*, AnimToPlay, float, PlayRate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FForceStopAnimDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExecutedComboDelegate);

struct FCurrentCombo
{
	FCurrentCombo()
		: NextNode(-1), ComboToExecute(FComboWrapper()) {}

	FCurrentCombo(int32 InNextNode, FComboWrapper InComboToExecute)
		: NextNode(InNextNode), ComboToExecute(InComboToExecute) {}

	int32 NextNode;

	FComboWrapper ComboToExecute;
};

typedef TMap<uint8, FCurrentCombo> FKeyNode;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMBODETECTOR_API UComboDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComboDetectorComponent();

protected:

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UPROPERTY(EditAnywhere)
	UCombosDataAsset* CombosData;

	UFUNCTION(BlueprintCallable, Category = ComboDetector)
	void DetectCombo(EComboInput In);

	UFUNCTION(BlueprintCallable, Category = ComboDetector)
	void Init();

	UPROPERTY(BlueprintAssignable, Category = ComboDetector)
	FExecuteComboDelegate ExecuteComboEvent;

	UPROPERTY(BlueprintAssignable, Category = ComboDetector)
	FForceStopAnimDelegate ForceStopAnimEvent;

	UPROPERTY(BlueprintAssignable, Category = ComboDetector)
	FExecutedComboDelegate ExecutedComboEvent;

private:

	UPROPERTY(BlueprintReadOnly, Category = ComboDetector, meta = (AllowPrivateAccess = "true"))
	bool bIsExecutingCombo;
	
	bool bPauseInputs, bWasInitiated, bComboWasCompleted;

	int32 CurrentNode;

	void CreateNodes();

	TArray<FKeyNode> Nodes;

	TQueue<FCurrentCombo> ComboQueue;

	void ExecuteCombo();

	FTimerHandle DequeNextComboHandle;

	UFUNCTION()
	void ExecuteNextCombo();
};
