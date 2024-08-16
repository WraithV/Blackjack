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

//TODO:Unused remove
void ABlackJack_Pawn::SetupDelegates()
{
	//APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//ABlackJack_PlayerController* BJ_PC = Cast<ABlackJack_PlayerController>(controller);

	//BJ_PC->FAllDiscard.AddUObject(this, &ABlackJack_Pawn::resetPlayer);

	////MyDel.BindUFunction(this, FName("DoSomething"));

	////BJ_PC->AllPlayersDiscard.AddDynamic(this, &ABlackJack_Pawn::resetPlayer); //Bind to Delegate

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bound delegate for %s"), *PlayerName));
}

/*Add up all non ace cards then reduce ace card value until the score is under WinScore or out of ace cards
* Returns false if at or over 21, true if the player is able to continue playing
*/
bool ABlackJack_Pawn::CalculatePlayerScore()
{
	int TempScore = 0;
	int AceScore = 0;

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
	//reduce ace values till below 21 or out of aces
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
	PlayerScore = TempScore + AceScore;

	WPlayerHandWidget->UpdatePlayerScore(PlayerScore);

	if (PlayerScore > WinScore) //score still over, player busts
	{
		PlayerBust();

		return false;
	}
	else if (PlayerScore == WinScore) //score equals winscore, automatically stands
	{
		PlayerStands();

		return false;
	}

	return true;
}

// Called every frame
//void ABlackJack_Pawn::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ABlackJack_Pawn::StartPlaying()
{
	SendStatus(EPlayerStatus::Playing);

	CalculatePlayerScore();

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
*/
void ABlackJack_Pawn::PlayerAddCard(ACard* NewCard, bool IsFaceUp)
{
	PlayerHand.Add(NewCard);

	NewCard->FaceUp = IsFaceUp;

	WPlayerHandWidget->AddCardToHand(NewCard->CardName, IsFaceUp);

	if (IsFaceUp)
	{
		CalculatePlayerScore();
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

	WPlayerHandWidget->UpdatePlayerScore(PlayerScore);

	SendStatus(EPlayerStatus::Bust);

	IBlackjackActions::Execute_EndTurn(this->GetController());
}

//Dealer continues hitting until over DealerMin or Busts
void ABlackJack_Pawn::DealerPlay()
{
	if (!WPlayerHandWidget)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("no pawn for dealer")));
	}
	WPlayerHandWidget->FlipCards();

	if (!CalculatePlayerScore() || PlayerScore >= DealerMin) //End turn if dealer is over DealerMin score
	{

		IBlackjackActions::Execute_DealerEnds(this->GetController());

	}
	//deal until dealer hits his max or busts
	for (int i = 0; i < 10; i++)
	{
		IBlackjackActions::Execute_PlayerHit(this->GetController());

		if (!CalculatePlayerScore() || PlayerScore >= DealerMin)
		{
			break;
		}
	}

	IBlackjackActions::Execute_DealerEnds(this->GetController());
}

//Update Player status text field on widget
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

