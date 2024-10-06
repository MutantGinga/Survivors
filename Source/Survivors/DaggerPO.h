#pragma once

#include "CoreMinimal.h"
#include "PooledObject.h"
#include "Player_Character.h"
#include "DaggerPO.generated.h"

UCLASS()
class SURVIVORS_API ADaggerPO : public APooledObject
{
	GENERATED_BODY()

private:

	ADaggerPO();

	// Upgrade methods

	virtual void Tick(float DeltaTime) override; // Called every frame
	void MoveDagger(); // Moves the dagger in the direction it was thrown
	virtual void SetActive(bool IsActive) override; // Sets the dagger to active or inactive
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); // Called when the dagger overlaps with something

	class APlayer_Character* Player; // Reference to the player character
	FVector direction; // Direction the dagger is thrown

	UPROPERTY(VisibleAnywhere, Category = "Sprites")
	class UPaperSpriteComponent* MySprite; // The sprite of the dagger
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class UCapsuleComponent* MyBodyCollider; // The collider of the dagger
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* OverlapTrigger; // The overlap trigger for the dagger

	UPROPERTY(EditAnywhere, Category = "Stats")
	int DaggerDamage = 10; // The damage the dagger does
	UPROPERTY(EditAnywhere, Category = "Stats")
	int DaggerSpeed = 1000; // The speed the dagger moves
};