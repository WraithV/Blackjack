// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BlackJack_Pawn.h"


#include "Blueprint/UserWidget.h"
#include "PlayerHand.h"

#include "BlackJackBoard.generated.h"

/**Widget representing the GameBoard, Implemented in UMG/Widgets
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
	UFUNCTION(BlueprintImplementableEvent, Category = "Game Board")
	void CreatePlayerHandWidgets(ABlackJack_Pawn* CurrentPlayer, UPARAM(ref) int& index);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Board")
	void RoundFinished();

};
