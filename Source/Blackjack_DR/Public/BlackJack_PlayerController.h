// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "IBlackjackActions.h"
#include "Card.h"
#include "CardAce.h"
#include "BlackJack_Pawn.h"

#include "Kismet/GameplayStatics.h"

#include"BlackJackBoard.h"

#include "BlackJack_PlayerController.generated.h"

//UDELEGATE()
//DECLARE_MULTICAST_DELEGATE(FAllDiscard);

/** Responsible for setting up and managing most game aspects including
* The deck, turn order and scoring
* This single player controller possess pawns in turn
* Implements IBlackjackActions to execute gameplay functionality
*/
UCLASS()
class BLACKJACK_DR_API ABlackJack_PlayerController : public APlayerController, public IBlackjackActions
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	int NumPlayers = 3; //Max Players 3
	UPROPERTY(BlueprintReadOnly)
	int CurrentPlayerIndex; //TODO: do we need this as well as currentPlayer?
	UPROPERTY(EditAnywhere)
	TArray<class ABlackJack_Pawn*> PlayerList;
	//Card Variables
	UPROPERTY(BlueprintReadOnly)
	int CurrentCard = 0;
	UPROPERTY(BlueprintReadOnly)
	TArray<class ACard*> CardDeck; //list of cards, will be shuffled	
	UPROPERTY(BlueprintReadOnly)
	TArray<class ACardAce*> AceCards;

	UPROPERTY(EditAnywhere)
	UDataTable* CardDataTable;

	
	UPROPERTY(EditAnywhere)
	ABlackJack_Pawn* CurrentPlayer;
	UPROPERTY(EditAnywhere)
	ABlackJack_Pawn* DealerPawn;

	//Game Board Class
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UBlackJackBoard> UIClass;

	UPROPERTY()
	UBlackJackBoard* GameBoard;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void SpawnCardActor(int Value, FString Name);

	virtual void CreateCardDeck();
	virtual void DealCards();
	virtual void DetermineWinners();

	virtual void ShuffleArray(TArray<ACard*>& myArray);

	void PlayerHit_Implementation() override;
	void PlayerStand_Implementation() override;
	void DealerEnds_Implementation() override;
	void StartGame_Implementation() override;
	void EndTurn_Implementation() override;
	
};
