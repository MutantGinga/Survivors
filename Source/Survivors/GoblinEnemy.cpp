// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinEnemy.h"

AGoblinEnemy::AGoblinEnemy()
{
	EnemyHealth = 30;
	DamageValue = 0.3f;
	EnemyAcceleration = 50.0f;
	EnemyMaxSpeed = 30.0f;
}

void AGoblinEnemy::SpawnFlies(AActor* Goblin, int NumberOfFlies, float Radius, TSubclassOf<AActor> FlyClass)
{
	if (FlyClass != NULL)
	{
		for (int i = 0; i < NumberOfFlies; i++)
		{
			float Angle = i * (360 / NumberOfFlies);
			FVector GoblinLocation = GetActorLocation();
			FVector BatLocation = GoblinLocation + Radius * FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), 0, FMath::Sin(FMath::DegreesToRadians(Angle)));
			FActorSpawnParameters SpawnParams;
			GetWorld()->SpawnActor<AActor>(FlyClass, BatLocation, FRotator::ZeroRotator, SpawnParams);
		}
	}
}

void AGoblinEnemy::SpawnFliesTimer()
{
	SpawnFlies(this, 5, 100, EnemyBPClass);
}

void AGoblinEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Set the timer to spawn flies
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AGoblinEnemy::SpawnFliesTimer, 6.0f, true);
}