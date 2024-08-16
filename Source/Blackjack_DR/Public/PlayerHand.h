// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PlayerHand.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BLACKJACK_DR_API UPlayerHand : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BlackJack")
	void AddCardToHand(UPARAM(ref) FString& NameString, UPARAM(ref) bool& IsFaceUp);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BlackJack")
	void FlipCards();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BlackJack")
	void Resethand();
	UFUNCTION(BlueprintNativeEvent, Category = "BlackJack")
	void UpdatePlayerStatus(UPARAM(ref)FString& NewStatus);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BlackJack")
	void UpdatePlayerScore(int NewScore);
};
