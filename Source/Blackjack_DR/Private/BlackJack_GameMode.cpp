// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackJack_GameMode.h"

//Spawn Card Actor and initialize values
void ABlackJack_GameMode::SpawnCardActor(int Value, FString Name)
{
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	ACard* NewCard;


	if (Name == "A") //If is ace card
	{
		NewCard = GetWorld()->SpawnActor<ACardAce>(Location, Rotation, SpawnInfo);
	}
	else 
	{
		NewCard = GetWorld()->SpawnActor<ACard>(Location, Rotation, SpawnInfo);
	}

	NewCard->SetupCardValues(Value, Name, false);

	CardDeck.Add(NewCard);

}

//
void ABlackJack_GameMode::CreateCardDeck()
{
	//Spawn numbered cards from each suit
	for (int suits = 0; suits < 3; suits++)
	{
		for (int numberedCards = 2; numberedCards < 11; numberedCards++)
		{
			SpawnCardActor(numberedCards, FString::FromInt(numberedCards));
		}

	}
	//TODO: finish this
	//TArray<FString*> SpecialCards;
	//DeckSpecialCards.DataTable->GetAllRows(ContextString:"", [&]SpecialCards);
	//for (const auto SpecialCard: SpecialCards)
	//{

	//}


	//process data table
	//spawn AceCard if A
	//otherwise spawn card with 10 value and 

}

void ABlackJack_GameMode::DealCards()
{
	for (int i = 0; i < 1; i++)
	{
		for (ABlackJack_Pawn* Player : PlayerList)
		{
			bool faceUp = true;

			//Dealers second card is face down
			if (Player->IsDealer && i == 1)
			{
				faceUp = false;
			}

			Player->PlayerAddCard(CardDeck[CurrentCard], faceUp);
			CurrentCard++;
		}
	}
}


void ABlackJack_GameMode::DetermineWinners()
{
	int DealerScore = DealerPawn->PlayerScore;
	for (ABlackJack_Pawn* Player : PlayerList)
	{
		if (Player->IsDealer) 
		{
			return; //don't update dealers status
		}
		if (Player->PlayerScore < 0)
		{
			//player is bust, don't update
			return;
		}

		//Calculate score for players still in
		if (Player->PlayerScore > DealerScore)
		{
			//TODO: update feedback
			//player WINS
		}
		else if (Player->PlayerScore == DealerScore)
		{
			//player TIES
		}
		else
		{
			//player loses
		}
	}

}

//Player hits, give them another card
void ABlackJack_GameMode::PlayerHit()
{
	CurrentPlayer->PlayerAddCard(CardDeck[CurrentCard], true);
	CurrentCard++;
}

void ABlackJack_GameMode::PlayerStand()
{
	EndTurn();
	//end turn
}

void ABlackJack_GameMode::DealerEnds()
{
	DetermineWinners();

	//TODO update widgets
	//update widget

}

//reset
void ABlackJack_GameMode::StartGame()
{
	CurrentCard = 0;
	//TODO: event dispatcher to clear hand
	//Shuffle Array
	DealCards();
	CurrentPlayerIndex = -1;
	EndTurn();
}

//
void ABlackJack_GameMode::EndTurn()
{
	if (CurrentPlayerIndex > NumPlayers) //don't execute if dealer player calls, do we need this?
	{
		return;
	}
	CurrentPlayerIndex++;

	CurrentPlayer = PlayerList[CurrentPlayerIndex];
	if (!CurrentPlayer->IsDealer)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		controller->UnPossess();
		controller->Possess(CurrentPlayer);

		//TODO: update widget/message that its this players turn
	}
	else
	{
		CurrentPlayer->DealerPlay();
	}

}




//Blackjack Actions Interface Implementations