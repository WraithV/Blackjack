// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackJack_Pawn.h"

#include "IBlackjackActions.h"
//#include "GameFramework/PlayerController.h"

#include "BlackJack_PlayerController.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ABlackJack_Pawn::ABlackJack_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ABlackJack_Pawn::ClearHand()
{
}

//// Called when the game starts or when spawned
//void ABlackJack_Pawn::BeginPlay()
//{
//	
//}

void ABlackJack_Pawn::SetupDelegates()
{
	//APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//ABlackJack_PlayerController* BJ_PC = Cast<ABlackJack_PlayerController>(controller);

	//BJ_PC->FAllDiscard.AddUObject(this, &ABlackJack_Pawn::resetPlayer);

	////MyDel.BindUFunction(this, FName("DoSomething"));

	////BJ_PC->AllPlayersDiscard.AddDynamic(this, &ABlackJack_Pawn::resetPlayer); //Bind to Delegate

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bound delegate for %s"), *PlayerName));
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Tempscore = %i"), TempScore));
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

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%s Score = %i"), *PlayerName, PlayerScore));

	return true;
}

// Called every frame
void ABlackJack_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlackJack_Pawn::StartPlaying()
{
	SendStatus("Playing");

	CalculatePlayerScore();

	if (IsDealer)
	{
		DealerPlay();
	}
}

void ABlackJack_Pawn::resetPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("fired delegate 2")));

	SendStatus("Waiting");

	PlayerScore = 0;
	PlayerHand.Empty();

	WPlayerHandWidget->Resethand();

}

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

//Player stands
void ABlackJack_Pawn::PlayerStands()
{
	SendStatus("Stand");

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%s Stands"), *PlayerName));

}






//Player score over winScore and has bust
void ABlackJack_Pawn::PlayerBust()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerScore = -1;

	WPlayerHandWidget->UpdatePlayerScore(PlayerScore);

	SendStatus("BUST!");

	IBlackjackActions::Execute_EndTurn(this->GetController());

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%s Busts"), *PlayerName));
}

void ABlackJack_Pawn::DealerPlay()
{
	if (!WPlayerHandWidget)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("no pawn for dealer")));
	}
	WPlayerHandWidget->FlipCards();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Dealer Plays")));

	//APlayerController* controller = this->GetController()

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
void ABlackJack_Pawn::SendStatus(FString message)
{
	WPlayerHandWidget->UpdatePlayerStatus(message);

}

// Called to bind functionality to input
void ABlackJack_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

