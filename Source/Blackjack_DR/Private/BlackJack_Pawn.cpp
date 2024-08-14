// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackJack_Pawn.h"

// Sets default values
ABlackJack_Pawn::ABlackJack_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABlackJack_Pawn::BeginPlay()
{
	Super::BeginPlay();

	//TODO: bind to event dispatcher to clear player hand
	
}



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
	//TODO: update widget

	if (PlayerScore > WinScore) //score still over, player busts
	{
		PlayerBust();

		return false;
	}
	else if (PlayerScore = WinScore) //score equals winscore, automatically stands
	{
		PlayerStands();

		return false;
	}

	return true;
}

// Called every frame
void ABlackJack_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlackJack_Pawn::resetPlayer()
{
	PlayerScore = 0;
	PlayerHand.Empty();
	//TODO: reset widgets
}

void ABlackJack_Pawn::PlayerHit(ACard* NewCard, bool IsFaceUp)
{
	PlayerHand.Add(NewCard);
	//TODO: add card widget

	if (IsFaceUp)
	{
		CalculatePlayerScore();
	}

}

//Player stands
void ABlackJack_Pawn::PlayerStands()
{
	//TODO: widget update player status: Stand
	//TODO: call interface ending turn
}

//Player score over winScore and has bust
void ABlackJack_Pawn::PlayerBust()
{
	//TODO: widget update player status: BUST!
	PlayerScore = -1;
	//TODO: call interface ending turn
}

void ABlackJack_Pawn::DealerPlay()
{
	//TODO: Flipcard

	if (!CalculatePlayerScore())
	{
		//TODO: call dealer ends interface
	}
	if (PlayerScore >= DealerMin)
	{
		//TODO: call dealer ends interface
	}
	//deal until dealer hits his max or busts
	for (int i = 0; i < 10; i++)
	{
		//TODO: call player hit interface
		if (!CalculatePlayerScore || PlayerScore >= DealerMin)
		{
			break;
		}
	}
	//TODO: call dealer ends interface
}

// Called to bind functionality to input
void ABlackJack_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

