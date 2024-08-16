// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackJack_PlayerController.h"

#include "FCardData.h"

#include "BlackJackBoard.h"
#include "Blueprint/UserWidget.h"


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
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Setup Dealer");
		}
		else
		{
			NewPawn->PlayerName = FString::Printf(TEXT("Player %i"), i);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Setup Player %i"), i));
		}

		
		//NewPawn->SetupDelegates();


	}



	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("created %i"), PlayerList.Num()));

	CreateCardDeck();

	GameBoard = CreateWidget<UBlackJackBoard>(this, UIClass);
	check(GameBoard);
	GameBoard->AddToPlayerScreen();


	//ABlackJack_Pawn* CurrentPawn : PlayerList

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

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Setup Player %s"), *Name.ToString()));

	NewCard->SetupCardValues(Value, Name, false);

	CardDeck.Add(NewCard);

}

//
void ABlackJack_PlayerController::CreateCardDeck()
{
	//Spawn numbered cards from each suit
	for (int suits = 0; suits < 4; suits++)
	{

		if (!CardDataTable)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("no data table")));
			continue;
		}

		for (auto& data : CardDataTable->GetRowMap())
		{
			FName name = data.Key;

			FCardData* MyStruct = reinterpret_cast<FCardData*>(data.Value);

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Create Card %s"), *MyStruct->CardName));
			

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
			CurrentPawn->SendStatus("");
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
			CurrentPawn->SendStatus("WINNER!");

		}
		else if (CurrentPawn->PlayerScore == DealerScore)
		{
			CurrentPawn->SendStatus("Tie");
		}
		else
		{
			CurrentPawn->SendStatus("Bad Luck");
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("DEALER CALLED ENDTURN")));
		return;
	}
	CurrentPlayerIndex++;

	CurrentPlayer = PlayerList[CurrentPlayerIndex];

	this->UnPossess();
	this->Possess(CurrentPlayer);

	CurrentPlayer->StartPlaying();

}
