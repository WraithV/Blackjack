// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Card.h"
#include "CardAce.h"

#include "BlackJack_Pawn.generated.h"

UCLASS()
class BLACKJACK_DR_API ABlackJack_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABlackJack_Pawn();

	UPROPERTY(EditAnywhere)
	int PlayerScore;
	int WinScore = 21;
	FString PlayerName;
	FString PlayerStatus;

	TArray<class ACard*> PlayerHand; //Card Actors

	TSubclassOf<UUserWidget> WPlayerHand; //Widget connected to this player

	//Dealer Variables
	bool IsDealer = false;
	int DealerMin = 17;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool CalculatePlayerScore();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void resetPlayer();
	virtual void PlayerHit(ACard* NewCard, bool IsFaceUp);
	virtual void PlayerStands();
	virtual void PlayerBust();

	virtual void DealerPlay();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
