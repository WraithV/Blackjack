// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BlackJack_Pawn.h"


#include "Blueprint/UserWidget.h"
#include "PlayerHand.h"

#include "BlackJackBoard.generated.h"
/**
 * 
 */
UCLASS(Abstract)
class BLACKJACK_DR_API UBlackJackBoard : public UUserWidget
{
	GENERATED_BODY()

public:
	//Game Board Class
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHand> HandClass;

	UPROPERTY(BlueprintReadOnly)
	TArray<UPlayerHand*> HandWidgets;


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game Board")
	void CreatePlayerHandWidgets(ABlackJack_Pawn* CurrentPlayer, UPARAM(ref) int& index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game Board")
	void RoundFinished();

};
