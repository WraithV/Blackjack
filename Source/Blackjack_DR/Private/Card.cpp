// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"

// Sets default values
ACard::ACard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

//Hide the card value until it is FaceUp
int ACard::GetCardValue()
{
	if (FaceUp)
	{
		return CardValue;
	}

	return 0;
}

//Initialise card values
void ACard::SetupCardValues(int NewValue, FString NewName, bool IsCardFaceup)
{
	CardValue = NewValue;
	CardName = NewName;
	FaceUp = IsCardFaceup;
}

