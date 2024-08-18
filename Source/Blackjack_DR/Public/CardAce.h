// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card.h"
#include "CardAce.generated.h"

/** Child of ACard which represents an Ace card and has additional functionality
 * Ace cards have a value of 11 by default but are reduced to 1 if the total of the players hand (including aces) exceeds 21
 */
UCLASS()
class BLACKJACK_DR_API ACardAce : public ACard
{
	GENERATED_BODY()

	public:
	int CardValue = 11;
	FString CardName = "A";
	
	public:
	virtual bool ReduceValue();
};
