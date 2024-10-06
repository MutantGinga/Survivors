// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AmmoPool.h"
#include "Player_Character.generated.h"

class AMaxHealth_Upgrade;

//UENUMs tie animation states to integers, so that we can easily switch between them

UENUM()
enum class KnightAnimationState : uint8
{
	AS_WALKING_RIGHT,
	AS_WALKING_LEFT,
	AS_IDLE,
	AS_EMPTY,
	AS_DEAD,
	AS_WON
};

class ADaggerWeapon; // Forward declaration so we can use the class in the header file

UCLASS()
class SURVIVORS_API APlayer_Character : public APawn
{
	GENERATED_BODY()

public:

	// Ways to get the player's direction

	FVector LastDirection;
	FVector GetLastDirection() const;
	
	// Health functions
	void Damage(float DamageValue); // Function to damage the player

	UFUNCTION(BlueprintCallable) 
	float GetHealth(); // Function to get the player's health
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth(); // Function to get the player's max health
	UFUNCTION(BlueprintCallable)
	void ResetHealth(); // Function to reset the player's health

	// The camera for the player
	UPROPERTY(VisibleAnywhere, Category = "Camera Setup")
	class UCameraComponent* MyCamera; // made public because it is used by the Spawner class

private:

	// Health variables - these were originally used for upgrades, but now they are just used to make the health bar work
	UPROPERTY(EditAnywhere, Category = "Player Properties")
	float PlayerHealth = 100.0f; // The player's health
	UPROPERTY(EditAnywhere, Category = "Player Properties")
	float MaxPlayerHealth = 100.0f; // The player's max health

	// Movement variables
	UPROPERTY(EditAnywhere, Category = "Player Properties")
	float PlayerAcceleration = 1000; // The player's acceleration
	UPROPERTY(EditAnywhere, Category = "Player Properties")
	float PlayerMaxSpeed = 32; // The player's max speed

	float InputX; // The player's input on the X axis
	float InputZ; // The player's input on the Z axis

	// The sprite for the player
	UPROPERTY(VisibleAnywhere, Category = "Sprites")
	class UPaperFlipbookComponent* MySprite;

	// The collider for the player
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class UCapsuleComponent* MyBodyCollider;

	// The spring arm for the camera
	UPROPERTY(VisibleAnywhere, Category = "Camera Setup")
	class USpringArmComponent* MySpringArm;

	// Flipbooks for the player's animations
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	class UPaperFlipbook* Flipbook_WalkingRight;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	class UPaperFlipbook* Flipbook_Idle;

	UPROPERTY(VisibleAnywhere, Category = "Animations")
	KnightAnimationState CurrentAnimationState;
	UPROPERTY(VisibleAnywhere, Category = "Animations")
	KnightAnimationState OldAnimationState;

	// Ammo Pool and Dagger weapons in the pool
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAmmoPool* AmmoPool; // The ammo pool for the player
	APooledObject* DaggerToUpgrade; // artefact from old code, as the dagger itself is no longer upgraded
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<APooledObject*> AllDaggers; // Array of all the daggers in the pool

	// A way for the player to spawn a dagger
	UFUNCTION()
	void SpawnDagger();

	FTimerHandle TimerHandle; // Timer handle for the player's dagger cooldown
	
	FVector direction;
	FTimerHandle WinTimer;
	FTimerHandle HealthRegenTimer;

	// Movement functions
	void MovePlayer(float inputX, float inputY); // Function to move the player
	void MovePlayerX(float Value); // Function to move the player on the X axis
	void MovePlayerZ(float Value); // Function to move the player on the Z axis

	// Animation functions
	void IdentifyAnimStates(); // Function to identify the animation states
	void ProcessAnimStateMachine(); // Function to process the animation state machine

	// Constructor
	APlayer_Character();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void KillPlayer(); // Called when the player runs out of health
	void WinGame(); // Called when the player survives the full 5 minutes

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
