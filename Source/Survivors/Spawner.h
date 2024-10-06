#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player_Character.h"
#include <Camera/CameraComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Spawner.generated.h"

UCLASS()
class SURVIVORS_API ASpawner : public AActor
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	// Sets default values for this actor's properties
	ASpawner();
	UPROPERTY(EditAnywhere, Category = "Development")
	int WaveNumber = 1;
	int MinEnemyCount;
	FTimerHandle WaveTimer;
	FTimerHandle SpawnTimer;
	FTimerHandle SpawnTimerB;

	void EndPlay(const EEndPlayReason::Type EndPlayReason);
	UFUNCTION(BlueprintCallable)
	void SpawnWave();

	void SpawnBatsRandom();
	void SpawnBatsCircle(AActor* Player, int NumberOfBats, float Radius, TSubclassOf<AActor> BatClass);

	void SpawnBatsCircleTimer();

	void Wave3TimerA();

	void Wave3TimerB();

	void SpawnSkeletonCircle(AActor* Player, int NumberOfSkeletons, float Radius, TSubclassOf<AActor> SkeletonClass);

	void SpawnSkeletonCircleTimer();

	void SpawnGoblinCircleTimer();

	void Wave5TimerA();

	void SpawnFastBatSwarm();

	void Wave8TimerA();

	void Wave9TimerA();

	void Wave9TimerB();

	void Wave10TimerA();

	void UpdateEnemyCount();

	APlayer_Character* PlayerCharacter;
	AActor* GameCamera;
	UCameraComponent* GameCameraComponent;
	float CameraWidth;
	float MaxSpawnDistance;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties");
	TSubclassOf<AActor> EnemyBPClass;
	UPROPERTY(EditAnywhere, Category = "Enemy Properties");
	TSubclassOf<AActor> FastBatBPClass;
	UPROPERTY(EditAnywhere, Category = "Enemy Properties");
	TSubclassOf<AActor> SkeletonBPClass;
	UPROPERTY(EditAnywhere, Category = "Enemy Properties");
	TSubclassOf<AActor> GoblinBPClass;

	int32 NumEnemiesToSpawn;
	int32 CurrentEnemyCount;

};
