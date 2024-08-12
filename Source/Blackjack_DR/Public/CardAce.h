// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card.h"
#include "CardAce.generated.h"

/**
 * 
 */
UCLASS()
class BLACKJACK_DR_API ACardAce : public ACard
{
	GENERATED_BODY()
	
	virtual bool ReduceValue();
};
