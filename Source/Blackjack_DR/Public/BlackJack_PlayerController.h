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

	//Data table with cards
	UPROPERTY(EditAnywhere)
	TArray<class ACard*> CardDeck;

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

	//UPROPERTY(BlueprintAssignable)
	//FAllDiscard FAllDiscard;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	//virtual void InitGame();

	//void AllDiscard();

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
