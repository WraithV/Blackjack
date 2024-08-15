// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Card.generated.h"

UCLASS()
class BLACKJACK_DR_API ACard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACard();

	UPROPERTY(EditAnywhere)
	int CardValue = 0;
	UPROPERTY(EditAnywhere)
	FString CardName = "";
	UPROPERTY(EditAnywhere)
	bool FaceUp = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupCardValues(int NewValue, FString NewName, bool IsCardFaceup);
	virtual int GetCardValue();

};
