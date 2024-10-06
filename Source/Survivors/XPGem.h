// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player_Character.h"
#include "XPGem.generated.h"

UCLASS()
class SURVIVORS_API AXPGem : public AActor
{
	GENERATED_BODY()
	
public:	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	AXPGem(); // Sets default values for this actor's properties
	int GetCoinValue();

	virtual void Tick(float DeltaTime) override; // Called every frame
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Sprites")
	class UPaperSpriteComponent* MySprite;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* OverlapTrigger;
	class APlayer_Character* PlayerCharacter; // Reference to the player character
	UPROPERTY(EditAnywhere, Category = "Stats")
	int CoinValue = 1;
};
