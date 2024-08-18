// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PlayerHand.generated.h"

/** Widget representing the players hand on the table, All functions are implemented in UMG/Widgets but called by c++ functions
 * 
 */
UCLASS(Abstract)
class BLACKJACK_DR_API UPlayerHand : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BlackJack")
	void AddCardToHand(UPARAM(ref) FString& NameString, UPARAM(ref) bool& IsFaceUp);
	UFUNCTION(BlueprintImplementableEvent, Category = "BlackJack")
	void FlipCards();
	UFUNCTION(BlueprintImplementableEvent, Category = "BlackJack")
	void Resethand();
	UFUNCTION(BlueprintImplementableEvent, Category = "BlackJack")
	void UpdatePlayerStatus(UPARAM(ref) EPlayerStatus& NewStatus);

	UFUNCTION(BlueprintImplementableEvent, Category = "BlackJack")
	void UpdatePlayerScore(int NewScore);
};
