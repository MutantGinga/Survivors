// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include <Engine.h>
#include "BaseEnemy.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned

void ASpawner::BeginPlay()
{
    Super::BeginPlay();

    // Get a reference to the player pawn (NOT CHARACTER)
    PlayerCharacter = Cast<APlayer_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    int32 NumPlayerCharacters = UGameplayStatics::GetNumPlayerControllers(GetWorld());


    // Check if the player character is valid
   if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter is null"));
        return;
    }

    // Get the camera from the player character
    GameCameraComponent = PlayerCharacter->MyCamera;

    // Check if the camera is valid
    if (!GameCameraComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameCameraComponent is null"));
        return;
    }
    // Spawn the first wave immediately
    SpawnWave();
    // Set a timer to spawn a new wave every 30 seconds
    GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &ASpawner::SpawnWave, 30.0f, true);

}

void ASpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

    Super::EndPlay(EndPlayReason);

    // Clears timers when the game ends
    GetWorld()->GetTimerManager().ClearTimer(WaveTimer);
    GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
    GetWorld()->GetTimerManager().ClearTimer(SpawnTimerB);
}

void ASpawner::SpawnWave()
{
    // The number of enemies to spawn in this wave
    NumEnemiesToSpawn = WaveNumber * 100;

    CameraWidth = (GameCameraComponent->OrthoWidth);
    MaxSpawnDistance = (CameraWidth / 2.f) + 100; // Spawn enemies just off screen

    if (WaveNumber == 1)
    {
        // Set a timer to spawn enemies every second
        GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::SpawnBatsRandom, 1.0f, true); // 30 flies
    }

    else if (WaveNumber == 2)
    {
        // Set a timer to spawn enemies every second
        SpawnBatsCircleTimer();
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::SpawnBatsCircleTimer, 10.0f, true); // 48 flies
    }

    else if (WaveNumber == 3)
    {
        // spawn five EnemyBPClass every second, and spawn a swarm of EnemyBPClass that flies across the screen towards and past the player every 10 seconds
        GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::Wave3TimerA, 2.0f, true); // 30 flies
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerB, this, &ASpawner::Wave3TimerB, 5.0f, true); // 24 flies
    }

    else if (WaveNumber == 4)
    {
        // spawn a square of skeletons (2 daggers to kill) around the player every 10 seconds
        SpawnSkeletonCircleTimer();
        GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::SpawnSkeletonCircleTimer, 10.0f, true); // 20 ghouls
    }

    else if (WaveNumber == 5)
    {
        // spawn five bats and five skeletons every second
        GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::Wave3TimerA, 1.0f, true); // 30 flies
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerB, this, &ASpawner::Wave5TimerA, 2.0f, true); // 15 ghouls
    }

    else if (WaveNumber == 6)
    {
        // spawn fast bats in swarms every 5 seconds and continue to spawn five skeletons every second
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerB, this, &ASpawner::Wave5TimerA, 1.0f, true); // 30 ghouls
        GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::SpawnFastBatSwarm, 10.0f, true); // 45 fast flies
    }

    else if (WaveNumber == 7)
    {
        // spawn a square of goblins (three daggers to kill) around the player every 10 seconds
        SpawnGoblinCircleTimer();
        GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::SpawnGoblinCircleTimer, 10.0f, true); // 20 slayers
    }

    else if (WaveNumber == 8)
    {
        // spawn five skeletons and five goblins every second
        GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::Wave5TimerA, 1.0f, true); // 30 ghouls
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerB, this, &ASpawner::Wave8TimerA, 3.0f, true); // 10 slayers
    }

    else if (WaveNumber == 9)
	{
        // spawn 10 bats and 10 goblins every second
        GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::Wave9TimerA, 1.0f, true); // 60 flies
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerB, this, &ASpawner::Wave9TimerB, 3.0f, true); // 10 slayers
	}

	else if (WaveNumber == 10)
	{
        // spawn 10 bats, 10 skeletons, and 10 goblins every second
        GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::Wave9TimerA, 1.0f, true); // 60 flies
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerB, this, &ASpawner::Wave9TimerB, 3.0f, true); // 10 slayers
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerB, this, &ASpawner::Wave10TimerA, 1.0f, true); // 30 ghouls
	}

    else
    {
        GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawner::SpawnBatsRandom, 1.0f, true);
    }

    // Increase the wave number for the next wave
    ++WaveNumber;
}

void ASpawner::SpawnBatsRandom()
{
    // Only spawn enemies if CurrentEnemyCount is less than NumEnemiesToSpawn
    if (CurrentEnemyCount < NumEnemiesToSpawn)
    {
        // Calculate a random position around the player, just off screen
        float DistanceFromPlayer = FMath::RandRange(CameraWidth / 2.f, MaxSpawnDistance);
        float Angle = FMath::RandRange(0.f, 2.f * PI);
        FVector SpawnPosition = PlayerCharacter->GetActorLocation() + DistanceFromPlayer * FVector(FMath::Cos(Angle), 0.f, FMath::Sin(Angle));

        // Spawn the enemy at the calculated position
        GetWorld()->SpawnActor<ABaseEnemy>(EnemyBPClass, SpawnPosition, FRotator::ZeroRotator);

        // Increase the CurrentEnemyCount
        ++CurrentEnemyCount;
    }
}

void ASpawner::SpawnBatsCircle(AActor* Player, int NumberOfBats, float Radius, TSubclassOf<AActor> BatClass)
{
    if (!Player || !BatClass)
	{
		return;
	}

    for (int i = 0; i < NumberOfBats; i++)
    {
        float Angle = i * ((2.0f * PI) / NumberOfBats);
        FVector PlayerLocation = Player->GetActorLocation();
        FVector BatLocation = PlayerLocation + Radius * FVector(FMath::Cos(Angle), 0, FMath::Sin(Angle));
        // Spawn the bat
        FActorSpawnParameters SpawnParams;
        GetWorld()->SpawnActor<AActor>(BatClass, BatLocation, FRotator::ZeroRotator, SpawnParams);
    }
}

void ASpawner::SpawnBatsCircleTimer()
{
    SpawnBatsCircle(PlayerCharacter, 12, 100.0f, EnemyBPClass);
}

void ASpawner::Wave3TimerA()
{
    for (int i = 0; i < 2; i++)
    {
        float DistanceFromPlayer = FMath::RandRange(CameraWidth / 2.f, MaxSpawnDistance);
        float Angle = FMath::RandRange(0.f, 2.f * PI);
        FVector SpawnPosition = PlayerCharacter->GetActorLocation() + DistanceFromPlayer * FVector(FMath::Cos(Angle), 0.f, FMath::Sin(Angle));

        // Spawn the enemy at the calculated position
        GetWorld()->SpawnActor<ABaseEnemy>(EnemyBPClass, SpawnPosition, FRotator::ZeroRotator);
    }

    // Increase the CurrentEnemyCount
    CurrentEnemyCount += 2;
}

void ASpawner::Wave3TimerB()
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector SpawnPosition = PlayerLocation + FVector(MaxSpawnDistance, 0.f, 0.f); // Spawn on the right side of the player
    FVector Direction = (PlayerLocation - SpawnPosition).GetSafeNormal(); // Direction towards the player

    int GridRows = 2;
    int GridColumns = 2;
    float GridSpacing = 50.0f;

    for (int i = 0; i < GridRows; i++)
    {
        for (int j = 0; j < GridColumns; j++)
        {
            FVector GridPosition = SpawnPosition + FVector(i * GridSpacing, 0.0f, j * GridSpacing);
            GetWorld()->SpawnActor<ABaseEnemy>(EnemyBPClass, GridPosition, Direction.Rotation()); // Offset each bat vertically
        }
    }

    CurrentEnemyCount += 4;
}

void ASpawner::SpawnSkeletonCircle(AActor* Player, int NumberOfSkeletons, float Radius, TSubclassOf<AActor> SkeletonClass)
{
    for (int i = 0; i < NumberOfSkeletons; i++)
    {
        float Angle = i * (360.0f / NumberOfSkeletons);
        FVector PlayerLocation = Player->GetActorLocation();
        FVector SkeletonLocation = PlayerLocation + Radius * FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), 0, FMath::Sin(FMath::DegreesToRadians(Angle)));
        // Spawn the Skeleton
        FActorSpawnParameters SpawnParams;
        GetWorld()->SpawnActor<AActor>(SkeletonClass, SkeletonLocation, FRotator::ZeroRotator, SpawnParams);
    }
}

void ASpawner::SpawnSkeletonCircleTimer()
{
    SpawnSkeletonCircle(PlayerCharacter, 5, 100.0f, SkeletonBPClass);
}

void ASpawner::SpawnGoblinCircleTimer()
{
    SpawnSkeletonCircle(PlayerCharacter, 5, 100.0f, GoblinBPClass);
}

void ASpawner::Wave5TimerA()
{
    for (int i = 0; i < 1; i++)
    {
		float DistanceFromPlayer = FMath::RandRange(CameraWidth / 2.f, MaxSpawnDistance);
		float Angle = FMath::RandRange(0.f, 2.f * PI);
		FVector SpawnPosition = PlayerCharacter->GetActorLocation() + DistanceFromPlayer * FVector(FMath::Cos(Angle), 0.f, FMath::Sin(Angle));

		// Spawn the enemy at the calculated position
		GetWorld()->SpawnActor<ABaseEnemy>(SkeletonBPClass, SpawnPosition, FRotator::ZeroRotator);
	}

	// Increase the CurrentEnemyCount
	CurrentEnemyCount += 1;
}

void ASpawner::SpawnFastBatSwarm()
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector SpawnPosition = PlayerLocation + FVector(MaxSpawnDistance, 0.f, 0.f); // Spawn on the right side of the player
    FVector Direction = (PlayerLocation - SpawnPosition).GetSafeNormal(); // Direction towards the player

    int GridRows = 5;
    int GridColumns = 3;
    float GridSpacing = 50.0f;

    for (int i = 0; i < GridRows; i++)
    {
        for (int j = 0; j < GridColumns; j++)
        {
            FVector GridPosition = SpawnPosition + FVector(i * GridSpacing, 0.0f, j * GridSpacing);
            GetWorld()->SpawnActor<ABaseEnemy>(FastBatBPClass, GridPosition, Direction.Rotation()); // Offset each bat vertically
        }
    }

    CurrentEnemyCount += 15;
}

void ASpawner::Wave8TimerA()
{
    for (int i = 0; i < 1; i++)
    {
        float DistanceFromPlayer = FMath::RandRange(CameraWidth / 2.f, MaxSpawnDistance);
        float Angle = FMath::RandRange(0.f, 2.f * PI);
        FVector SpawnPosition = PlayerCharacter->GetActorLocation() + DistanceFromPlayer * FVector(FMath::Cos(Angle), 0.f, FMath::Sin(Angle));

        // Spawn the enemy at the calculated position
        GetWorld()->SpawnActor<ABaseEnemy>(GoblinBPClass, SpawnPosition, FRotator::ZeroRotator);
    }

    // Increase the CurrentEnemyCount
    CurrentEnemyCount += 1;
}

void ASpawner::Wave9TimerA()
{
    for (int i = 0; i < 2; i++)
    {
        float DistanceFromPlayer = FMath::RandRange(CameraWidth / 2.f, MaxSpawnDistance);
        float Angle = FMath::RandRange(0.f, 2.f * PI);
        FVector SpawnPosition = PlayerCharacter->GetActorLocation() + DistanceFromPlayer * FVector(FMath::Cos(Angle), 0.f, FMath::Sin(Angle));

        // Spawn the enemy at the calculated position
        GetWorld()->SpawnActor<ABaseEnemy>(EnemyBPClass, SpawnPosition, FRotator::ZeroRotator);
    }

    // Increase the CurrentEnemyCount
    CurrentEnemyCount += 2;
}

void ASpawner::Wave9TimerB()
{
    for (int i = 0; i < 1; i++)
    {
        float DistanceFromPlayer = FMath::RandRange(CameraWidth / 2.f, MaxSpawnDistance);
        float Angle = FMath::RandRange(0.f, 2.f * PI);
        FVector SpawnPosition = PlayerCharacter->GetActorLocation() + DistanceFromPlayer * FVector(FMath::Cos(Angle), 0.f, FMath::Sin(Angle));

        // Spawn the enemy at the calculated position
        GetWorld()->SpawnActor<ABaseEnemy>(GoblinBPClass, SpawnPosition, FRotator::ZeroRotator);
    }

    // Increase the CurrentEnemyCount
    CurrentEnemyCount += 1;
}

void ASpawner::Wave10TimerA()
{
    for (int i = 0; i < 1; i++)
    {
        float DistanceFromPlayer = FMath::RandRange(CameraWidth / 2.f, MaxSpawnDistance);
        float Angle = FMath::RandRange(0.f, 2.f * PI);
        FVector SpawnPosition = PlayerCharacter->GetActorLocation() + DistanceFromPlayer * FVector(FMath::Cos(Angle), 0.f, FMath::Sin(Angle));

        // Spawn the enemy at the calculated position
        GetWorld()->SpawnActor<ABaseEnemy>(SkeletonBPClass, SpawnPosition, FRotator::ZeroRotator);
    }

    // Increase the CurrentEnemyCount
    CurrentEnemyCount += 2;
}

void ASpawner::UpdateEnemyCount()
{

    //UE_LOG(LogTemp, Warning, TEXT("UpdateEnemyCount() called"));
    // Get all actors of class ABaseEnemy
    TArray<AActor*> FoundEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), FoundEnemies);

    // Update CurrentEnemyCount
    CurrentEnemyCount = FoundEnemies.Num();
}


// Called every frame
void ASpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update CurrentEnemyCount
    UpdateEnemyCount();
}