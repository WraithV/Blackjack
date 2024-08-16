// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	Waiting,
	Playing,
	Stand,
	Bust,
	Winner,
	Loser,
	Tie
};
