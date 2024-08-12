// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"

// Sets default values
ACard::ACard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACard::BeginPlay()
{
	Super::BeginPlay();
	
}

//Hide the card value until it is revealed
int ACard::GetCardValue()
{
	if (FaceUp)
	{
		return CardValue;
	}

	return 0;
}

// Called every frame - not needed for this class. pls remove
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Need to disable tick

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("%s"), *CardName));
	//}

}

//Initialise card values
void ACard::SetupCardValues(int NewValue, FString NewName, bool IsCardFaceup)
{
	CardValue = NewValue;
	CardName = NewName;
	FaceUp = IsCardFaceup;
}

