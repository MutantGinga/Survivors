#include "SurvivorsGameModeBase.h"
#include "SurvivorsSave.h"

void ASurvivorsGameModeBase::ResetCoins()
{
	CoinTotal = 0;
}

void ASurvivorsGameModeBase::AddCoins(int CoinsToAdd)
{
	CoinTotal += CoinsToAdd;
	UE_LOG(LogTemp, Warning, TEXT("Coin Total: %i."), CoinTotal); // Log the player's current coins
}

int ASurvivorsGameModeBase::GetCoinTotal()
{
	return CoinTotal;
}

void ASurvivorsGameModeBase::SetCoinTotal(int Coins)
{
	CoinTotal = Coins;
	UE_LOG(LogTemp, Warning, TEXT("Coin Total: %i."), CoinTotal); // Log the player's current coins
	USurvivorsSave* SaveGame = Cast<USurvivorsSave>(UGameplayStatics::CreateSaveGameObject(USurvivorsSave::StaticClass())); // Create a save game object
	SaveGame->SetCoins(CoinTotal); // Set the save game's coins to the player's coins
	UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("MySaveSlot"), 0); // Save the save game to the save slot
}

void ASurvivorsGameModeBase::IncreaseUpgradeLevel(int UpgradeType)
{
	USurvivorsSave* SaveGame = Cast<USurvivorsSave>(UGameplayStatics::CreateSaveGameObject(USurvivorsSave::StaticClass()));

	if (CurrentDaggerCountUpgrade < 4)
	{
		CurrentDaggerCountUpgrade++;
		CoinTotal -= DaggerCountUpgradeCost;
		SaveGame->DaggerCountUpgradeLevel = CurrentDaggerCountUpgrade;
		SaveGame->SetCoins(CoinTotal);
		UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("MySaveSlot"), 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dagger count upgrade level is already at maximum."));
	}
}

void ASurvivorsGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (UGameplayStatics::DoesSaveGameExist(TEXT("MySaveSlot"), 0))
	{
		USurvivorsSave* LoadGame = Cast<USurvivorsSave>(UGameplayStatics::LoadGameFromSlot(TEXT("MySaveSlot"), 0)); // Load the save game from the save slot
		CoinTotal = LoadGame->GetCoins(); // Set the player's coins to the coins in the save game
		CurrentDaggerCountUpgrade = LoadGame->GetDaggerCountUpgradeLevel();
		UE_LOG(LogTemp, Warning, TEXT("Current coins: %i."), CoinTotal); // Log the player's current coins
		UE_LOG(LogTemp, Warning, TEXT("Dagger count upgrade level: %i."), LoadGame->GetDaggerCountUpgradeLevel());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save game does not exist.")); // Log a warning if the save game does not exist
	}
}

int ASurvivorsGameModeBase::GetDaggerCountUpgradeCost()
{
	return DaggerCountUpgradeCost;
}

int ASurvivorsGameModeBase::GetDaggerCountUpgradeLevel()
{
	return CurrentDaggerCountUpgrade;
}

void ASurvivorsGameModeBase::SetDaggerCountUpgradeLevel(int Level)
{
	CurrentDaggerCountUpgrade = Level;
}

void ASurvivorsGameModeBase::ResetAllUpgrades()
{
	USurvivorsSave* SaveGame = Cast<USurvivorsSave>(UGameplayStatics::LoadGameFromSlot(TEXT("MySaveSlot"), 0)); // Load the save game from the save slot
	CurrentDaggerCountUpgrade = 1;
	DaggerCountUpgradeCost = 50;
	SaveGame->DaggerCountUpgradeLevel = CurrentDaggerCountUpgrade;
	SaveGame->DaggerCountUpgradeCost = DaggerCountUpgradeCost;
	UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("MySaveSlot"), 0);
}

void ASurvivorsGameModeBase::SetAllUpgrades(int MaxHealth, int HealthRegen, int Armor, int Damage, int DaggerCount)
{
	USurvivorsSave* SaveGame = Cast<USurvivorsSave>(UGameplayStatics::LoadGameFromSlot(TEXT("MySaveSlot"), 0)); // Load the save game from the save slot
	CurrentDaggerCountUpgrade = 4;
	SaveGame->DaggerCountUpgradeLevel = CurrentDaggerCountUpgrade;
	UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("MySaveSlot"), 0);
}