// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "GoblinEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORS_API AGoblinEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	
private:
	AGoblinEnemy();
	UPROPERTY(EditAnywhere, Category = "Enemy Properties");
	TSubclassOf<AActor> EnemyBPClass;
	void SpawnFlies(AActor* Goblin, int NumberOfFlies, float Radius, TSubclassOf<AActor> FlyClass);
	void SpawnFliesTimer();
	void BeginPlay();
	FTimerHandle SpawnTimer;
};
