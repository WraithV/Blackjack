// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "IBlackjackActions.h"
#include "Card.h"
#include "CardAce.h"
#include "BlackJack_Pawn.h"

#include "Kismet/GameplayStatics.h"

#include "BlackJack_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLACKJACK_DR_API ABlackJack_PlayerController : public APlayerController, public IBlackjackActions
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int NumPlayers = 3;
	int CurrentPlayerIndex; //TODO: do we need this as well as currentPlayer?
	int CurrentCard = 0;

	TArray<class ABlackJack_Pawn*> PlayerList;
	TArray<class ACard*> CardDeck;

	FDataTableRowHandle DeckSpecialCards; //TODO: need to set this in UE?

	//Data table deck

	ABlackJack_Pawn* CurrentPlayer;
	ABlackJack_Pawn* DealerPawn;

	//Main widget
	//list of player hand widgets - TODO: move this elsewhere

public:
	//virtual void InitGame();

	virtual void SpawnCardActor(int Value, FString Name);

	virtual void CreateCardDeck();
	virtual void DealCards();
	virtual void DetermineWinners();

	virtual void PlayerHit() override;
	virtual void PlayerStand() override;
	virtual void DealerEnds() override;
	virtual void StartGame() override;
	virtual void EndTurn() override;
	
};
