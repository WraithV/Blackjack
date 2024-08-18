// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Card.h"
#include "CardAce.h"

#include "PlayerHand.h"
#include "PlayerStatus.h"
//#include "BlackJack_PlayerController.h"

#include "BlackJack_Pawn.generated.h"

UCLASS()
class BLACKJACK_DR_API ABlackJack_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABlackJack_Pawn();

	const int WinScore = 21;
	const int DealerMin = 17;
	//Dealer Variables
	bool IsDealer = false;

	EPlayerStatus CurrentStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class ACard*> PlayerHand; //Card Actors

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int PlayerScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString PlayerName;

	//Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UPlayerHand> HandWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerHand* WPlayerHandWidget; //Widget connected to this player

	


private:

protected:

	virtual bool CalculatePlayerScore();

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetupDelegates();

	virtual void StartPlaying();
	virtual void resetPlayer();
	virtual void PlayerAddCard(ACard* NewCard, bool IsFaceUp);
	virtual void PlayerStands();
	virtual void PlayerBust();
	virtual void DealerPlay();

	virtual void SendStatus(EPlayerStatus status);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
