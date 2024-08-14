// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IBlackjackActions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBlackjackActions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLACKJACK_DR_API IBlackjackActions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PlayerHit();
	virtual void PlayerStand();
	virtual void DealerEnds();
	virtual void StartGame();
	virtual void EndTurn();
};
