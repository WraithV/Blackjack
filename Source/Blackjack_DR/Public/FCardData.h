// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCardData.generated.h"


/** Struct representing 
 * 
 */

USTRUCT(BlueprintType)
struct BLACKJACK_DR_API FCardData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString CardName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CardValue = 0;
};
