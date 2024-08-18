// Fill out your copyright notice in the Description page of Project Settings.


#include "CardAce.h"


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
}
