// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SurvivorsSave.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORS_API USurvivorsSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	USurvivorsSave();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int CurrentCoins;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int DaggerCountUpgradeLevel;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int DaggerCountUpgradeCost;

	UFUNCTION(BlueprintCallable)
	void SetCoins(int CoinTotal);

	UFUNCTION(BlueprintCallable)
	int GetCoins();

	UFUNCTION(BlueprintCallable)
	int GetDaggerCountUpgradeLevel();

	UFUNCTION(BlueprintCallable)
	int GetDaggerCountUpgradeCost();
};
