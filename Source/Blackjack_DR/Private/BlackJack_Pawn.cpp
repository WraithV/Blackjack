// Fill out your copyright notice in the Description page of Project Settings.

#include "IBlackjackActions.h"
//#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Tempscore = %i"), TempScore));
	PlayerScore = TempScore + AceScore;



	//ACardAce* CardAsAce = Cast<HandWidgetClass>(WPlayerHandWidget);

	//TODO: update widget

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

void ABlackJack_Pawn::resetPlayer()
{
	PlayerScore = 0;
	PlayerHand.Empty();
	//TODO: reset widgets
}

void ABlackJack_Pawn::PlayerAddCard(ACard* NewCard, bool IsFaceUp)
{
	PlayerHand.Add(NewCard);

	NewCard->FaceUp = IsFaceUp;

	WPlayerHandWidget->AddCardToHand(NewCard->CardName, IsFaceUp);

	//TODO: add card widget

	if (IsFaceUp)
	{
		CalculatePlayerScore();
	}

}

//Player stands
void ABlackJack_Pawn::PlayerStands()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (controller->GetClass()->ImplementsInterface(UBlackjackActions::StaticClass()))
	{
		IBlackjackActions::Execute_PlayerStand(controller);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%s Stands"), *PlayerName));

	//TODO: widget update player status: Stand
}

//Player score over winScore and has bust
void ABlackJack_Pawn::PlayerBust()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//TODO: widget update player status: BUST!

	PlayerScore = -1;

	if (controller->GetClass()->ImplementsInterface(UBlackjackActions::StaticClass()))
	{
		IBlackjackActions::Execute_EndTurn(controller);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%s Busts"), *PlayerName));
}

void ABlackJack_Pawn::DealerPlay()
{
	//TODO: widget Flipcard

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Dealer Plays")));

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!CalculatePlayerScore()) //End turn if dealer is over DealerMax
	{
		if (controller->GetClass()->ImplementsInterface(UBlackjackActions::StaticClass()))
		{
			IBlackjackActions::Execute_DealerEnds(controller);
		}

	}
	if (PlayerScore >= DealerMin)
	{
		if (controller->GetClass()->ImplementsInterface(UBlackjackActions::StaticClass()))
		{
			IBlackjackActions::Execute_DealerEnds(controller);
		}
	}
	//deal until dealer hits his max or busts
	for (int i = 0; i < 10; i++)
	{
		if (controller->GetClass()->ImplementsInterface(UBlackjackActions::StaticClass()))
		{
			IBlackjackActions::Execute_PlayerHit(controller);
		}

		if (!CalculatePlayerScore() || PlayerScore >= DealerMin)
		{
			break;
		}
	}

	if (controller->GetClass()->ImplementsInterface(UBlackjackActions::StaticClass()))
	{
		IBlackjackActions::Execute_DealerEnds(controller);
	}
}

// Called to bind functionality to input
void ABlackJack_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

