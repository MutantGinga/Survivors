// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Player_Character.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Engine.h>
#include "SurvivorsGameModeBase.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; // allows the enemy to tick

	// the below sets up the sprite, collider, and overlap trigger for the enemy
	MySprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyBodySprite"));
	MyBodyCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("EnemyBodyCapsule"));
	OverlapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Detector"));
	OverlapTrigger->SetBoxExtent(FVector(32, 32, 32));
	RootComponent = MyBodyCollider;
	MySprite->SetupAttachment(RootComponent);
	OverlapTrigger->SetupAttachment(RootComponent);
}

void ABaseEnemy::IdentifyAnimStates()
{
	OldAnimationState = CurrentAnimationState;

	float CurrentSpeed = GetVelocity().Size(); // Get the speed of the enemy
	static bool bLastDirectionWasRight = true; // enemy stars facing right

	if (CurrentSpeed > 1) // If the enemy is moving
	{
		FVector VelocityDirection = GetVelocity().GetSafeNormal(); // Get the direction of the enemy
		if (VelocityDirection.X > 0) // If the enemy is moving right
		{
			CurrentAnimationState = EnemyAnimationState::AS_WALKING_RIGHT; // Set the animation state to walking right
			bLastDirectionWasRight = true; // Set the last direction to right
		}
		else if (VelocityDirection.X < 0) // If the enemy is moving left
		{
			CurrentAnimationState = EnemyAnimationState::AS_WALKING_LEFT; // Set the animation state to walking left
			bLastDirectionWasRight = false;	// Set the last direction to left
		}
		else if (VelocityDirection.Z != 0) // If the enemy is moving up or down - there is no animation state for facing up or down
		{
			if (bLastDirectionWasRight) // If the last direction was right
			{
				CurrentAnimationState = EnemyAnimationState::AS_WALKING_RIGHT; // Set the animation state to walking right
			}
			else // If the last direction was left
			{
				CurrentAnimationState = EnemyAnimationState::AS_WALKING_LEFT; // Set the animation state to walking left
			}
		}
	}
	else
	{
		CurrentAnimationState = EnemyAnimationState::AS_IDLE;
	}
}

void ABaseEnemy::ProcessAnimStateMachine()
{
	if (CurrentAnimationState == OldAnimationState)
	{
		return;
	}

	switch (CurrentAnimationState)
	{
		case EnemyAnimationState::AS_IDLE:
		{
			MySprite->SetFlipbook(Flipbook_Idle);
			break;
		}
		case EnemyAnimationState::AS_WALKING_RIGHT:
		{
			MySprite->SetFlipbook(Flipbook_WalkingRight);
			MySprite->SetRelativeRotation(FRotator(0.f, 0.f, 0.f)); // default value to make animation state look to the right
			break;
		}
		case EnemyAnimationState::AS_WALKING_LEFT:
		{
			MySprite->SetFlipbook(Flipbook_WalkingRight);
			MySprite->SetRelativeRotation(FRotator(0.f, 180.f, 0.f)); // Flip the sprite to face left
			break;
		}
	}
}

void ABaseEnemy::Damage(float DamageAmount)
{
	EnemyHealth -= DamageAmount; // Subtract the damage amount from the enemy health
	if (EnemyHealth <= 0) // If the enemy health is less than or equal to 0
	{
		KillEnemy(); // Kill the enemy
	}
}

void ABaseEnemy::KillEnemy()
{
	FVector Location = {GetActorLocation().X, 20.0f, GetActorLocation().Z}; // Get the location of the enemy
	SpawnTransform.SetLocation(Location); // Set the location of the spawn transform to the location of the enemy
	AActor* XP = GetWorld()->SpawnActor<AActor>(XPBPClass, SpawnTransform); // Spawn an XP Gem at the location of the enemy
	Destroy(); // Destroy the enemy
}

void ABaseEnemy::Tick(float DeltaTime) // Called every frame
{
	Super::Tick(DeltaTime);

	GetPlayerPosition();
	FVector Direction = MyCharacterPosition - GetActorLocation(); // Calculate direction to player
	Direction.Normalize(); // Normalize to get direction vector

	if (!isColliding) // Only move if not colliding with player
	{
		MyBodyCollider->SetPhysicsLinearVelocity(Direction * EnemyMaxSpeed); // Move the enemy towards the player
	}

	IdentifyAnimStates();
	ProcessAnimStateMachine();
}

void ABaseEnemy::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(APlayer_Character::StaticClass())) // If the enemy collides with the player
	{
		Cast<APlayer_Character>(OtherActor)->Damage(DamageValue); // Damage the player
		isColliding = true; // Set to true when colliding with player
	}
}

void ABaseEnemy::OnCollisionEnd(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(APlayer_Character::StaticClass())) // If the enemy stops colliding with the player
	{
		isColliding = false; // Set to false
	}
}

void ABaseEnemy::BeginPlay() // Called when the game starts or when spawned
{
	Super::BeginPlay();

	MyBodyCollider->OnComponentHit.AddDynamic(this, &ABaseEnemy::OnCollision);
	MyBodyCollider->OnComponentHit.AddDynamic(this, &ABaseEnemy::OnCollisionEnd);

	CurrentAnimationState = EnemyAnimationState::AS_IDLE;
	OldAnimationState = EnemyAnimationState::AS_EMPTY;
	ProcessAnimStateMachine();
}

void ABaseEnemy::GetPlayerPosition()
{
	MyCharacterPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(); // Get the location of the player
}