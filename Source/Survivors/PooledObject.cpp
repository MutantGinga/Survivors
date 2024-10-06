// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledObject.h"

// Sets default values
APooledObject::APooledObject()
{
 	// empty constructor
}

void APooledObject::Deactivate()
{
	SetActive(false); // set object to inactive
	GetWorldTimerManager().ClearAllTimersForObject(this); // clear all timers
	OnPooledObjectDespawn.Broadcast(this); // broadcast despawn event
}

void APooledObject::SetActive(bool IsActive)
{
	Active = IsActive;
	SetActorHiddenInGame(!IsActive); 
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &APooledObject::Deactivate, LifeSpan, false); // set timer to deactivate object after lifespan
}

void APooledObject::SetLifeSpan(float LifeTime)
{
	LifeSpan = LifeTime;
}

void APooledObject::SetPoolIndex(int Index)
{
	PoolIndex = Index;
}

bool APooledObject::IsActive()
{
	return Active;
}

int APooledObject::GetPoolIndex()
{
	return PoolIndex;
}