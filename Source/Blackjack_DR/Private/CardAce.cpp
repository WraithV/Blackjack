// Fill out your copyright notice in the Description page of Project Settings.


#include "CardAce.h"

#include "BlackJack_PlayerController.h"


void ACardAce::BeginPlay()
{
	Super::BeginPlay();

	BlackJackController = Cast<ABlackJack_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//Bind ResetGameDelgate to ResetAceValue Function
	BlackJackController->ResetGame.AddDynamic(this, &ACardAce::ResetAceValue);

	
}

//Reduce Value of Ace card if it has not been done so already
bool ACardAce::ReduceValue()
{
	if (CardValue == 1)
	{
		return false;
	}

	CardValue = 1;
	return true;
}

void ACardAce::ResetAceValue()
{
	CardValue = 11;

	//UE_LOG(LogTemp, Warning, TEXT("Reset Ace Value"));

}


//On create subscribe to delegate and execute resetAceValue