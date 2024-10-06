// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player_Character.h"
#include <Camera/CameraComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "SurvivorsGameModeBase.generated.h"

UCLASS()

class SURVIVORS_API ASurvivorsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ResetCoins();
	UFUNCTION()
	static void AddCoins(int CoinsToAdd);
	UFUNCTION(BlueprintCallable)
	int GetCoinTotal();
	UFUNCTION(BlueprintCallable)
	void SetCoinTotal(int Coins);
	UFUNCTION(BlueprintCallable)
	void IncreaseUpgradeLevel(int UpgradeType);

	void BeginPlay();

	UFUNCTION(BlueprintCallable)
	int GetDaggerCountUpgradeCost();
	UFUNCTION(BlueprintCallable)
	int GetDaggerCountUpgradeLevel();
	UFUNCTION(BlueprintCallable)
	void SetDaggerCountUpgradeLevel(int Level);

	UFUNCTION(BlueprintCallable)
	void ResetAllUpgrades();

	UFUNCTION(BlueprintCallable)
	void SetAllUpgrades(int MaxHealth, int HealthRegen, int Armor, int Damage, int DaggerCount);


private:
	inline static int CoinTotal = 0;

	int CurrentDaggerCountUpgrade = 1;
	int DaggerCountUpgradeCost = 50;
};
