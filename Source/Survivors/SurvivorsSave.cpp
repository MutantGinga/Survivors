// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorsSave.h"

USurvivorsSave::USurvivorsSave()
{
	CurrentCoins = 0;
	DaggerCountUpgradeLevel = 1;
	DaggerCountUpgradeCost = 50;
}

void USurvivorsSave::SetCoins(int CoinTotal)
{
	CurrentCoins = CoinTotal;
}

int USurvivorsSave::GetCoins()
{
	return CurrentCoins;
}

int USurvivorsSave::GetDaggerCountUpgradeLevel()
{
	return DaggerCountUpgradeLevel;
}

int USurvivorsSave::GetDaggerCountUpgradeCost()
{
	return DaggerCountUpgradeCost;
}
