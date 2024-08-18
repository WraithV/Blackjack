// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackJack_PlayerController.h"

#include "FCardData.h"
#include "PlayerStatus.h"

#include "BlackJackBoard.h"
#include "Blueprint/UserWidget.h"

//Spawn Players
void ABlackJack_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	ABlackJack_Pawn* NewPawn;

	for(int i = 0; i < (NumPlayers + 1); i++)
	{
		NewPawn = GetWorld()->SpawnActor<ABlackJack_Pawn>(Location, Rotation, SpawnInfo);

		PlayerList.Add(NewPawn);

		if (i == NumPlayers) //Setup last player as the dealer
		{
			NewPawn->IsDealer = true;
			NewPawn->PlayerName = "Dealer";
			DealerPawn = NewPawn;
		}
		else
		{
			NewPawn->PlayerName = FString::Printf(TEXT("Player %i"), i);

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

//Spawn Card Actor and initialize values
void ABlackJack_PlayerController::SpawnCardActor(int Value, FString Name)
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

			CurrentPawn->PlayerAddCard(CardDeck[CurrentCard], faceUp);
			CurrentCard++;
		}
	}
}


void ABlackJack_PlayerController::DetermineWinners()
{
	int DealerScore = DealerPawn->PlayerScore;
	for (ABlackJack_Pawn* CurrentPawn : PlayerList)
	{

		if (CurrentPawn->IsDealer)
		{
			return; //don't update dealers status
		}
		if (CurrentPawn->PlayerScore < 0)
		{
			//player is bust, don't update
			return;
		}

		//Calculate score for players still in
		if (CurrentPawn->PlayerScore > DealerScore)
		{
			CurrentPawn->SendStatus(EPlayerStatus::Winner);

		}
		else if (CurrentPawn->PlayerScore == DealerScore)
		{
			CurrentPawn->SendStatus(EPlayerStatus::Tie);
		}
		else
		{
			CurrentPawn->SendStatus(EPlayerStatus::Loser);
		}
	}

}

//Shuffle the Cards
void ABlackJack_PlayerController::ShuffleArray(TArray<ACard*>& myArray)
{
	if (myArray.Num() > 0)
	{
		int32 LastIndex = myArray.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				myArray.Swap(i, Index);
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

void ABlackJack_PlayerController::PlayerStand_Implementation()
{
	CurrentPlayer->PlayerStands();

	EndTurn_Implementation();
	//end turn
}

void ABlackJack_PlayerController::DealerEnds_Implementation()
{
	DetermineWinners();

	GameBoard->RoundFinished();

}

//reset
void ABlackJack_PlayerController::StartGame_Implementation()
{
	CurrentCard = 0;

	for (int i = 0; i < PlayerList.Num() ;i++) 
	{
		PlayerList[i]->resetPlayer();
	}


	//DiscardCards.Broadcast();

	//CALL DELEGATE

	//TODO: Declare delegate
	// 
	//TODO: event dispatcher to clear hand
	
	ShuffleArray(CardDeck);
	CurrentPlayerIndex = 0;

	CurrentPlayer = PlayerList[CurrentPlayerIndex];

	this->UnPossess();
	this->Possess(CurrentPlayer);
	CurrentPlayer->StartPlaying();

	DealCards();
}

//
void ABlackJack_PlayerController::EndTurn_Implementation()
{
	if (CurrentPlayer->IsDealer) //don't execute if dealer player calls, do we need this?
	{
		return;
	}
	CurrentPlayerIndex++;

	CurrentPlayer = PlayerList[CurrentPlayerIndex];

	this->UnPossess();
	this->Possess(CurrentPlayer);

	CurrentPlayer->StartPlaying();

}
