// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"
#include "PooledObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledObjectDespawn, APooledObject*, PoolActor);

UCLASS()
class SURVIVORS_API APooledObject : public AActor
{
	GENERATED_BODY()

public:

	bool IsActive();
	void SetLifeSpan(float LifeTime);
	virtual void SetActive(bool IsActive);
	int GetPoolIndex();
	void Deactivate();
	FOnPooledObjectDespawn OnPooledObjectDespawn; // Event to be called when the object is despawned
	void SetPoolIndex(int Index);

protected:

	APooledObject();

private:

	bool Active;
	float LifeSpan = 0.0f;
	int PoolIndex;
	FTimerHandle LifeSpanTimer;
};