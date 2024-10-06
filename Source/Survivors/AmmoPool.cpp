// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPool.h"

// Sets default values for this component's properties
UAmmoPool::UAmmoPool()
{
	// empty constructor
}

APooledObject* UAmmoPool::SpawnPooledObject() // spawn an object from the pool
{
	for (APooledObject* PoolableActor : ObjectPool) // for each object in the pool
	{
		if (PoolableActor != nullptr && !PoolableActor->IsActive()) // find an inactive object
		{
			PoolableActor->TeleportTo(FVector(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()), FRotator(0, 0, 0)); // teleport the object to the player
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan); // set the lifespan of the object
			PoolableActor->SetActive(true); // set the object to active
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex()); // add the object to the spawned pool

			return PoolableActor;
		}
	}

	// if there are no inactive objects in the pool

	if (SpawnedPoolIndexes.Num() > 0) // if there are objects in the spawned pool
	{
		int PooledObjectIndex = SpawnedPoolIndexes[0]; // get the first object in the spawned pool
		SpawnedPoolIndexes.Remove(PooledObjectIndex); // remove the object from the spawned pool
		APooledObject* PoolableActor = ObjectPool[PooledObjectIndex];  // get the object from the object pool

		if (PoolableActor != nullptr)
		{
			PoolableActor->SetActive(false); // deactivate the object
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan); // reset the lifespan of the object
			PoolableActor->SetActive(true); // set the object to active again
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex()); // add the object to the spawned pool

			return PoolableActor;
		}
	}

	return nullptr;
}

TArray<APooledObject*> UAmmoPool::GetAllObjects() const // method to get all objects in the pool
{
	TArray<APooledObject*> PooledObjects; // create an array named pooled objects

	for (APooledObject* Object : ObjectPool)
	{
		PooledObjects.Add(Object); // add each object in the object pool to the pooled objects array
	}

	return PooledObjects;
}


void UAmmoPool::OnPooledObjectDespawn(APooledObject* PoolActor) // method to despawn an object
{
	SpawnedPoolIndexes.Remove(PoolActor->GetPoolIndex()); // remove the object from the spawned pool
	PoolActor->SetActive(false); // set the object to inactive
	PoolActor->SetActorLocation(FVector(0, 100, 0)); // set the object's location to the default location
}

// Called when the game starts
void UAmmoPool::BeginPlay()
{
	Super::BeginPlay();

	if (PooledObjectSubclass != nullptr)
	{
		UWorld* const World = GetWorld(); // get the world
		if (World != nullptr)
		{
			for (int i = 0; i < PoolSize; i++) // for each object in the pool
			{
				APooledObject* PoolableActor = World->SpawnActor<APooledObject>(PooledObjectSubclass, FVector(0, 100, 0), FRotator::ZeroRotator); // spawn the object

				if (PoolableActor != nullptr)
				{
					PoolableActor -> SetActive(false); // set the object to inactive
					PoolableActor -> SetPoolIndex(i); // set the object's pool index
					PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &UAmmoPool::OnPooledObjectDespawn); // bind the object to the despawn method
					ObjectPool.Add(PoolableActor); // add the object to the object pool
				}
			}
		}
	}
	
}