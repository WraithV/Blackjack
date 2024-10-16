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

//Process player score, update the widget UI and determine if the player is able to continue playing
void ABlackJack_Pawn::UpdatePlayerScore()
{
	PlayerScore = ProcessScore();
	WPlayerHandWidget->UpdatePlayerScore(PlayerScore);
	PlayerContinueTurn(PlayerScore);
}


//Bind Delegates on begin play
void ABlackJack_Pawn::BeginPlay()
{

	Super::BeginPlay();

	BlackJackController = Cast<ABlackJack_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (BlackJackController)
	{
		//Bind ResetGameDelgate to resetPlayer Function
		BlackJackController->ResetGame.AddDynamic(this, &ABlackJack_Pawn::resetPlayer);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("bj pawn cpp controller not found"));
	}
}


/*Determine if this player can continue their turn
* Players turn ends if at or above the max score
* @param Score: value used to determine if the player can continue their turn
* @return true if players score is less than the winScore
*/
bool ABlackJack_Pawn::PlayerContinueTurn(int score)
{
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

	//UpdatePlayerScore();

	if (IsDealer)
	{
		//flip cards
		UE_LOG(LogTemp, Warning, TEXT("Dealer: flips Cards"));
		PlayerHand[1]->FaceUp = true;
		WPlayerHandWidget->FlipCards(); //Make flipped cards value visible on the cards widget

		UpdatePlayerScore();

		DealerPlay();
	}
}
//Reset Player in preparation for a new round
void ABlackJack_Pawn::resetPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset Player"));

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
		UpdatePlayerScore();
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

//Dealer Checks score and hits if below a minimum score
void ABlackJack_Pawn::DealerPlay()
{
	UE_LOG(LogTemp, Log, TEXT("Dealer play start"));

	//Check for Dealer win condition
	if (PlayerScore >= DealerMin && PlayerScore <= WinScore)
	{
		if (this->GetController() != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Dealer Stands"));
			PlayerStands();
		}
	}

	if (CurrentStatus == EPlayerStatus::Playing)
	{
		UE_LOG(LogTemp, Log, TEXT("Continue playing"));
		GetWorld()->GetTimerManager().SetTimer(DealerDelayTimer, this, &ABlackJack_Pawn::LoopingHit, 1.2f, false);
	}
	else
	{
		IBlackjackActions::Execute_DealerEnds(this->GetController());
	}
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

//Called by DealerPlay on a timer, if the dealer has not stood or bust it will run DealerPlay again
void ABlackJack_Pawn::LoopingHit()
{
	UE_LOG(LogTemp, Warning, TEXT("Dealer Hits"));

	IBlackjackActions::Execute_PlayerHit(this->GetController());

	if (CurrentStatus == EPlayerStatus::Playing)
	{
		DealerPlay();
	}
}
