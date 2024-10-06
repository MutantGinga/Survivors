// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "BaseEnemy.generated.h"

// the below enum is used to define the different states of the enemy animation state machine
UENUM()
enum class EnemyAnimationState : uint8
{
	AS_WALKING_RIGHT,
	AS_WALKING_LEFT,
	AS_IDLE,
	AS_EMPTY
};

UCLASS()
class SURVIVORS_API ABaseEnemy : public APawn
{
	GENERATED_BODY()

	void IdentifyAnimStates();
	void ProcessAnimStateMachine();

public:

	void Damage(float DamageAmount);

private:
	
	void KillEnemy();

	virtual void Tick(float DeltaTime) override; // Called every frame

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit); // Called when the player collides with the enemy
	UFUNCTION()
	void OnCollisionEnd(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit); // Called when the player stops colliding with the enemy

	UPROPERTY(VisibleAnywhere, Category = "Sprites")
	class UPaperFlipbookComponent* MySprite;
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class UCapsuleComponent* MyBodyCollider;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* OverlapTrigger;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	class UPaperFlipbook* Flipbook_WalkingRight;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	class UPaperFlipbook* Flipbook_Idle;

	UPROPERTY(VisibleAnywhere, Category = "Animations")
	EnemyAnimationState CurrentAnimationState;
	UPROPERTY(VisibleAnywhere, Category = "Animations")
	EnemyAnimationState OldAnimationState;

	FVector MyCharacterPosition;
	bool isColliding;

	UPROPERTY(EditAnywhere, Category = "Drops");
	TSubclassOf<AActor> XPBPClass;

	FTransform SpawnTransform;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void GetPlayerPosition();
	// Sets default values for this pawn's properties
	ABaseEnemy();
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	int EnemyHealth;
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float DamageValue = 0.1f;
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float EnemyAcceleration = 50;
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float EnemyMaxSpeed = 50;
};
