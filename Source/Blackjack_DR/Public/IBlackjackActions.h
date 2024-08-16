// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IBlackjackActions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Blackjack Interface")
	void PlayerHit();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Blackjack Interface")
	void PlayerStand();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Blackjack Interface")
	void DealerEnds();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Blackjack Interface")
	void StartGame();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Blackjack Interface")
	void EndTurn();
};
