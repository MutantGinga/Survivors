// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.h"
#include "AmmoPool.generated.h"


UCLASS()
class SURVIVORS_API UAmmoPool : public UActorComponent
{
	GENERATED_BODY()

public:
	APooledObject* SpawnPooledObject();

private:
	// Sets default values for this component's properties
	UAmmoPool();

	UFUNCTION(BlueprintCallable, Category = "Ammo Pool")
	TArray<APooledObject*> GetAllObjects() const; // method to get all objects in the pool as an array

	UFUNCTION()
	void OnPooledObjectDespawn(APooledObject* PoolActor);

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	TSubclassOf<class APooledObject> PooledObjectSubclass;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	int PoolSize = 20; // default pool size

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float PooledObjectLifeSpan = 0.0f; // default life span of pooled objects

	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<APooledObject*> ObjectPool; // array to store all pooled objects
	TArray<int> SpawnedPoolIndexes; // array to store indexes of spawned objects

};
