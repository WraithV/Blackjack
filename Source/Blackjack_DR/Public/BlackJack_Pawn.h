// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Card.h"
#include "CardAce.h"

#include "PlayerHand.h"
#include "PlayerStatus.h"

#include "BlackJack_Pawn.generated.h"

/** Represents a player in the game
 * Can be a dealer player if IsDealer is true
 */
UCLASS()
class BLACKJACK_DR_API ABlackJack_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABlackJack_Pawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int WinScore = 21;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int DealerMin = 17;
	//Dealer Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDealer = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	virtual bool PlayerContinueTurn(int UpdatedScore);

	virtual int ProcessScore();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class ABlackJack_PlayerController* BlackJackController; //Forward Declaration to save on compile time

public:	
	virtual void StartPlaying();

	UFUNCTION(BlueprintCallable)
	virtual void resetPlayer();

	virtual void PlayerAddCard(ACard* NewCard, bool IsFaceUp);
	virtual void PlayerStands();
	virtual void PlayerBust();
	virtual void DealerPlay();

	virtual void SendStatus(EPlayerStatus status);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
