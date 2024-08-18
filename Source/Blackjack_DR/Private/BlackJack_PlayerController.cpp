// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackJack_PlayerController.h"

#include "FCardData.h"
#include "PlayerStatus.h"

#include "BlackJackBoard.h"
#include "Blueprint/UserWidget.h"


/* Spawn Pawns for each player up to a maximum of 3 players (excluding the dealer)
* Also setup the last player as the dealer
*/
void ABlackJack_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	

	for(int i = 0; i < (NumPlayers + 1); i++)
	{
		ABlackJack_Pawn* NewPawn = GetWorld()->SpawnActor<ABlackJack_Pawn>(Location, Rotation, SpawnInfo);

		PlayerList.Add(NewPawn);

		if (i == NumPlayers) //Setup last player as the dealer
		{
			NewPawn->IsDealer = true;
			NewPawn->PlayerName = "Dealer";
			DealerPawn = NewPawn;
		}
		else
		{
			int j = i + 1;
			NewPawn->PlayerName = FString::Printf(TEXT("Player %i"), j);

		}

	}

	if (!CardDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Card Data table not setup in PlayerController... Exiting"));
		this->ConsoleCommand("quit");
	}
	if (!UIClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIClass not setup in PlayerController... Exiting"));
		this->ConsoleCommand("quit");
	}

	CreateCardDeck();

	GameBoard = CreateWidget<UBlackJackBoard>(this, UIClass);
	check(GameBoard);
	GameBoard->AddToPlayerScreen();

	for (int i = 0; i < PlayerList.Num(); i++)
	{
		GameBoard->CreatePlayerHandWidgets(PlayerList[i], i);
	}

}

/*Spawn ACard Actors and ACardAce actors if the Name is equal to "A"
* @param Value: value of the card
* @param Name: Name on the Card
*/
void ABlackJack_PlayerController::SpawnCardActor(int Value, FString Name)
{
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	ACard* NewCard;


	if (Name == "A") //If ace card
	{
		NewCard = GetWorld()->SpawnActor<ACardAce>(Location, Rotation, SpawnInfo);

		AceCards.Add(Cast<ACardAce>(NewCard));

	}
	else
	{
		NewCard = GetWorld()->SpawnActor<ACard>(Location, Rotation, SpawnInfo);
	}

	NewCard->SetupCardValues(Value, Name, false);

	CardDeck.Add(NewCard);

}

//Loop through CardDataTable once per suit (4x) and spawn the ACard Actors
void ABlackJack_PlayerController::CreateCardDeck()
{
	//Spawn numbered cards from each suit
	for (int suits = 0; suits < 4; suits++)
	{

		for (auto& data : CardDataTable->GetRowMap())
		{
			FName name = data.Key;

			FCardData* MyStruct = reinterpret_cast<FCardData*>(data.Value);

			SpawnCardActor(MyStruct->CardValue, MyStruct->CardName);
		}
	}

}

//Deal two cards to all players, the dealers second card value is hidden (face down)
void ABlackJack_PlayerController::DealCards()
{
	for (int i = 0; i < 2; i++)
	{
		for (ABlackJack_Pawn* CurrentPawn : PlayerList)
		{
			bool faceUp = true;

			//Dealers second card is face down
			if (CurrentPawn->IsDealer && i == 1)
			{
				faceUp = false;
			}
			UE_LOG(LogTemp, Warning, TEXT("card num %i"), CurrentCard);
			CurrentPawn->PlayerAddCard(CardDeck[CurrentCard], faceUp);
			CurrentCard++;
		}
	}
}

/* Compare player scores against the dealer to determine winners in this round
* Don't update the dealer player or any players who have bust
* Update status on remaining players
*/
void ABlackJack_PlayerController::DetermineWinners()
{
	UE_LOG(LogTemp, Warning, TEXT("The Game ends"));

	int DealerScore = DealerPawn->PlayerScore;
	for (ABlackJack_Pawn* players : PlayerList)
	{
		if (players->IsDealer)
		{
			continue; //don't update dealers status
			
		}
		if (players->PlayerScore < 0)
		{
			//player is bust, don't update
			continue;
		}

		//Calculate score for players still in
		if (players->PlayerScore > DealerScore)
		{
			players->SendStatus(EPlayerStatus::Winner);

		}
		else if (players->PlayerScore == DealerScore)
		{
			players->SendStatus(EPlayerStatus::Tie);
		}
		else
		{
			players->SendStatus(EPlayerStatus::Loser);
		}
	}

}

/* Randomly shuffle the deck of cards
* @param CardArray: Array of ACard Actors
*/
void ABlackJack_PlayerController::ShuffleArray(TArray<ACard*>& CardArray)
{
	if (CardArray.Num() > 0)
	{
		int32 LastIndex = CardArray.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				CardArray.Swap(i, Index);
			}
		}
	}
}

/*Blackjack Actions Interface Implementations*/

//Player hits, give them another card
void ABlackJack_PlayerController::PlayerHit_Implementation()
{
	CurrentPlayer->PlayerAddCard(CardDeck[CurrentCard], true);

	CurrentCard++;
}

//Update player status to 'Stand' and end the players turn
void ABlackJack_PlayerController::PlayerStand_Implementation()
{
	CurrentPlayer->PlayerStands();

	EndTurn_Implementation();
	//end turn
}

//Called by the dealer player hitting their limit or bust, determine winner of the rounds and update the UI so players can begin a new round
void ABlackJack_PlayerController::DealerEnds_Implementation()
{
	DetermineWinners();

	GameBoard->RoundFinished();

}

//Reset players, shuffle cards, deal cards and possess first Player Pawn
void ABlackJack_PlayerController::StartGame_Implementation()
{
	CurrentCard = 0;

	for (int i = 0; i < PlayerList.Num() ;i++) 
	{
		PlayerList[i]->resetPlayer();
	}

	for (ACardAce* Aces : AceCards) //Reset any ace cards which were reduced in the previous round
	{
		Aces->ResetAceValue();
	}
	
	ShuffleArray(CardDeck);
	CurrentPlayerIndex = 0;

	CurrentPlayer = PlayerList[CurrentPlayerIndex];

	this->UnPossess();
	this->Possess(CurrentPlayer);
	CurrentPlayer->StartPlaying();

	DealCards();
}

/*Possess the next player in queue and update their status to 'playing'
* if called by the dealer player, do nothing
*/
void ABlackJack_PlayerController::EndTurn_Implementation()
{
	if (CurrentPlayer->IsDealer)
	{
		return;
	}
	CurrentPlayerIndex++;

	CurrentPlayer = PlayerList[CurrentPlayerIndex];

	this->UnPossess();
	this->Possess(CurrentPlayer);

	CurrentPlayer->StartPlaying();

}
