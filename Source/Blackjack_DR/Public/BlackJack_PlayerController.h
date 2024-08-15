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
	UPROPERTY(EditAnywhere)
	int CurrentPlayerIndex; //TODO: do we need this as well as currentPlayer?
	UPROPERTY(EditAnywhere)
	int CurrentCard = 0;

	UPROPERTY(EditAnywhere)
	TArray<class ABlackJack_Pawn*> PlayerList;

	UPROPERTY(EditAnywhere)
	TArray<class ACard*> CardDeck;

	UPROPERTY(EditAnywhere)
	UDataTable* CardDataTable; //TODO: set this in UE class

	//Data table deck
	UPROPERTY(EditAnywhere)
	ABlackJack_Pawn* CurrentPlayer;
	UPROPERTY(EditAnywhere)
	ABlackJack_Pawn* DealerPawn;

	//Main widget
	//list of player hand widgets - TODO: move this elsewhere
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


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
