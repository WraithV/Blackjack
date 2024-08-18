// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"

// Sets default values
ACard::ACard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

//If the card is FaceUp return its value, otherwise the card is face down and returns 0
int ACard::GetCardValue()
{
	if (FaceUp)
	{
		return CardValue;
	}

	return 0;
}

/* Initialise card values
* @param NewValue: card value
* @param Newname: Single character representing the cards Name
* @param IsCardFaceup: whether the card is dealt face up
*/
void ACard::SetupCardValues(int NewValue, FString NewName, bool IsCardFaceup)
{
	CardValue = NewValue;
	CardName = NewName;
	FaceUp = IsCardFaceup;
}

