// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackJack_Pawn.h"

#include "IBlackjackActions.h"

#include "BlackJack_PlayerController.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ABlackJack_Pawn::ABlackJack_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

/*Add up all non ace cards then reduce ace card value until the score is under WinScore or out of ace cards
* @return ProcessedScore: the new player score
*/
int ABlackJack_Pawn::ProcessScore()
{
	int TempScore = 0;
	int AceScore = 0;
	int ProcessedScore = 0;

	TArray<class ACardAce*> AceArray;

	//Add value of cards, put Aces in array to be adjusted after
	for (ACard* Card : PlayerHand)
	{
		ACardAce* CardAsAce = Cast<ACardAce>(Card);
		if (CardAsAce)
		{
			AceScore += CardAsAce->GetCardValue();
			AceArray.Add(CardAsAce);
		}
		else
		{
			TempScore += Card->GetCardValue();
		}


	}
	//reduce ace values till below the WinScore or out of aces
	if (TempScore + AceScore > WinScore)
	{
		for (ACardAce* AceItem : AceArray)
		{
			if (AceItem->ReduceValue())
			{
				AceScore -= 10;
			}
			
			if (TempScore + AceScore <= WinScore)
			{
				break;
			}
		}
	}

	ProcessedScore = TempScore + AceScore;

	return ProcessedScore;
}


/*Determine if this player can continue playing
* Dealer player will stand (not continue) if over 17 and not bust
* @param Score: value used to determine if the player can continue their turn
* @return true if players score is less than the winScore
*/
bool ABlackJack_Pawn::PlayerContinueTurn(int score)
{
	if (IsDealer)
	{
		if (score >= DealerMin && score <= WinScore)
		{
			if (this->GetController() != nullptr)
			{
				IBlackjackActions::Execute_PlayerStand(this->GetController());
			}
			return false;
		}
	}

	if (score > WinScore) //score over WinScore, player busts
	{
		PlayerBust();

		return false;
	}
	else if (score == WinScore) //score equals winscore, automatically stands
	{
		if (this->GetController() != nullptr)
		{
			IBlackjackActions::Execute_PlayerStand(this->GetController());
		}
		return false;
	}
	return true;
}


//Update player status if playing, if Dealer then execute dealer Play
void ABlackJack_Pawn::StartPlaying()
{
	SendStatus(EPlayerStatus::Playing);

	if (IsDealer)
	{
		DealerPlay();
	}
}
//Reset Player in preparation for a new round
void ABlackJack_Pawn::resetPlayer()
{
	SendStatus(EPlayerStatus::Waiting);

	PlayerScore = 0;
	PlayerHand.Empty();

	WPlayerHandWidget->Resethand();
}

/*Add New card actor to the players hand either face down (value hidden) or face up
* @param NewCard: Pointer to the Cards Actor
* @param IsFaceUp: true if card is dealt face up
*/
void ABlackJack_Pawn::PlayerAddCard(ACard* NewCard, bool IsFaceUp)
{
	PlayerHand.Add(NewCard);

	NewCard->FaceUp = IsFaceUp;

	WPlayerHandWidget->AddCardToHand(NewCard->CardName, IsFaceUp);

	if (IsFaceUp)
	{
		PlayerScore = ProcessScore();
		WPlayerHandWidget->UpdatePlayerScore(PlayerScore);
		bool continueplaying = PlayerContinueTurn(PlayerScore);
	}

}

//Update the players status to Stand
void ABlackJack_Pawn::PlayerStands()
{
	SendStatus(EPlayerStatus::Stand);
}

//Player score is > winScore and ends their turn with a loss
void ABlackJack_Pawn::PlayerBust()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerScore = -1;

	SendStatus(EPlayerStatus::Bust);

	IBlackjackActions::Execute_EndTurn(this->GetController());
}

//Dealer continues hitting until over DealerMin or Busts
void ABlackJack_Pawn::DealerPlay()
{
	bool continuePlaying = true;

	//Flip all cards in the dealers hand
	for (ACard* card : PlayerHand)
	{
		card->FaceUp = true;
	}

	WPlayerHandWidget->FlipCards(); //Make flipped cards value visible on the cards widget

	
	while (continuePlaying)
	{
		PlayerScore = ProcessScore();
		WPlayerHandWidget->UpdatePlayerScore(PlayerScore);
		continuePlaying = PlayerContinueTurn(PlayerScore);
		if (continuePlaying)
		{
			IBlackjackActions::Execute_PlayerHit(this->GetController());
		}
		
	}

	IBlackjackActions::Execute_DealerEnds(this->GetController());
}


/*Update Player status text field on widget
* @Param status: New status to update on the Pawns widget
*/
void ABlackJack_Pawn::SendStatus(EPlayerStatus status)
{
	CurrentStatus = status;

	WPlayerHandWidget->UpdatePlayerStatus(status);

}


// Called to bind functionality to input
void ABlackJack_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}