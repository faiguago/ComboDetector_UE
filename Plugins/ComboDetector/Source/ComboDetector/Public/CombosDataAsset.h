// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombosDataAsset.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class EComboInput : uint8
{
	Button_1,
	Button_2,
	Button_3,
	Button_4,
	Button_5,
	Button_6,
};

USTRUCT(BlueprintType)
struct FComboWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EComboInput Inputs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* Animation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Duration = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PlayRate = 1.0f;
};

USTRUCT(BlueprintType)
struct FCombo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	TArray<FComboWrapper> Combo;
};

/**
 *
 */
UCLASS(BlueprintType)
class COMBODETECTOR_API UCombosDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FCombo> AllCombos;
};
