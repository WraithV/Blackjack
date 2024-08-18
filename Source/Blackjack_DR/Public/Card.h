// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Card.generated.h"

/** Actor that represents a physical card, values populated from a data table
 * Cards that are not FaceUp return a value of 0 (value is hidden)
 */
UCLASS()
class BLACKJACK_DR_API ACard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACard();

	int CardValue = 0;
	FString CardName = "";
	bool FaceUp = true;

public:	
	virtual void SetupCardValues(int NewValue, FString NewName, bool IsCardFaceup);
	virtual int GetCardValue();

};
