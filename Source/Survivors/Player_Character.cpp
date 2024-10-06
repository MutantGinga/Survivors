// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Character.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Engine.h>
#include "SurvivorsGameModeBase.h"
#include "DaggerPO.h"
#include "SurvivorsSave.h"

void APlayer_Character::MovePlayer(float inputX, float inputZ)
{
	direction = FVector(inputX, 0, inputZ).GetSafeNormal(); // Normalize the vector to prevent faster diagonal movement
	if (direction != FVector(0, 0, 0)) // If the player is moving
	{
		LastDirection = direction; // Set the last direction to the current direction
	}
	MyBodyCollider->SetPhysicsLinearVelocity(direction * PlayerMaxSpeed); // Move the player
}

void APlayer_Character::MovePlayerX(float Value)
{
	InputX = Value; // Set the input value to the value passed in
}

void APlayer_Character::MovePlayerZ(float Value)
{
	InputZ = Value; // Set the input value to the value passed in
}

void APlayer_Character::IdentifyAnimStates()
{
	OldAnimationState = CurrentAnimationState;

	float CurrentSpeed = GetVelocity().Size(); // Get the speed of the player
	static bool bLastDirectionWasRight = true;

	if (CurrentSpeed > 1) // If the player is moving
	{
		FVector VelocityDirection = GetVelocity().GetSafeNormal(); // Get the direction of the player's velocity
		if (VelocityDirection.X > 0) // If the player is moving right
		{
			CurrentAnimationState = KnightAnimationState::AS_WALKING_RIGHT; // Set the animation state to walking right
			bLastDirectionWasRight = true; // Set the last direction to right
		}
		else if (VelocityDirection.X < 0) // If the player is moving left
		{
			CurrentAnimationState = KnightAnimationState::AS_WALKING_LEFT; // Set the animation state to walking left
			bLastDirectionWasRight = false; // Set the last direction to left
		}
		else if (VelocityDirection.Z != 0) // If the player is moving up or down, we need to find the last direction so that the animation state is correct - there is no animation for moving up or down
		{
			if (bLastDirectionWasRight)
			{
				CurrentAnimationState = KnightAnimationState::AS_WALKING_RIGHT; // Set the animation state to walking right if the last direction was right
			}
			else
			{
				CurrentAnimationState = KnightAnimationState::AS_WALKING_LEFT; // Set the animation state to walking left if the last direction was left
			}
		}
	}
	else
	{
		CurrentAnimationState = KnightAnimationState::AS_IDLE;
	}
}

void APlayer_Character::ProcessAnimStateMachine()
{
	if (CurrentAnimationState == OldAnimationState)
	{
		return;
	}

	switch (CurrentAnimationState)
	{
	case KnightAnimationState::AS_IDLE:
	{
		MySprite->SetFlipbook(Flipbook_Idle);
		break;
	}
	case KnightAnimationState::AS_WALKING_RIGHT:
	{
		MySprite->SetFlipbook(Flipbook_WalkingRight);
		MySprite->SetRelativeRotation(FRotator(0.f, 0.f, 0.f)); // Set the rotation of the sprite to face right
		break;
	}
	case KnightAnimationState::AS_WALKING_LEFT:
	{
		MySprite->SetFlipbook(Flipbook_WalkingRight);
		MySprite->SetRelativeRotation(FRotator(0.f, 180.f, 0.f)); // Set the rotation of the sprite to face left
		break;
	}
	case KnightAnimationState::AS_DEAD:
	{
		UGameplayStatics::OpenLevel(GetWorld(), "L_GameOver"); // Open the game over level if the player dies
		break;
	}
	case KnightAnimationState::AS_WON:
	{
		UGameplayStatics::OpenLevel(GetWorld(), "L_Win"); // Open the win level if the player wins
	}
	}
}

APlayer_Character::APlayer_Character()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// the below sets up the sprite, collider, spring arm, and camera for the player

	MySprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PlayerBodySprite")); 
	MyBodyCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PlayerBodyCapsule")); 
	MySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerSpringArm"));
	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerBodyCamera"));
	RootComponent = MyBodyCollider;
	MySprite->SetupAttachment(RootComponent);
	MySpringArm->SetupAttachment(RootComponent);
	MyCamera->SetupAttachment(MySpringArm);

	// the below sets up the AmmoPool for the player
	AmmoPool = CreateDefaultSubobject<UAmmoPool>(TEXT("AmmoPool"));
}

//Called every frame
void APlayer_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlayer(InputX, InputZ); // Move the player based on the input values

	if (PlayerHealth == 0 || PlayerHealth < 0) // If the player's health is 0 or less, kill the player
	{
		KillPlayer();
	}

	FVector MyUpdatedLocation = GetActorLocation();

	if (MyUpdatedLocation.X <= 7598)
	{
		MyUpdatedLocation.X = 7598;
	}

	else if (MyUpdatedLocation.X >= 13969)
	{
		MyUpdatedLocation.X = 13969;
	}

	if (MyUpdatedLocation.Z <= -1886)
	{
		MyUpdatedLocation.Z = -1886;
	}

	else if (MyUpdatedLocation.Z >= 4477)
	{
		MyUpdatedLocation.Z = 4477;
	}

	SetActorLocation(MyUpdatedLocation);
	IdentifyAnimStates();
	ProcessAnimStateMachine();
}

void APlayer_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	InputComponent->BindAxis("Horizontal", this, &APlayer_Character::MovePlayerX);
	InputComponent->BindAxis("Vertical", this, &APlayer_Character::MovePlayerZ);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector APlayer_Character::GetLastDirection() const
{
	return LastDirection;
}

void APlayer_Character::Damage(float DamageValue)
{
	PlayerHealth -= DamageValue; // Subtract the damage value from the player's health
}

float APlayer_Character::GetHealth()
{
	return PlayerHealth;
}

float APlayer_Character::GetMaxHealth()
{
	return MaxPlayerHealth;
}

void APlayer_Character::ResetHealth()
{
	PlayerHealth = MaxPlayerHealth;
}

void APlayer_Character::SpawnDagger()
{
	ASurvivorsGameModeBase* GameMode = Cast<ASurvivorsGameModeBase>(GetWorld()->GetAuthGameMode());
	USurvivorsSave* LoadGame = Cast<USurvivorsSave>(UGameplayStatics::LoadGameFromSlot(TEXT("MySaveSlot"), 0)); // Load the save game from the save slot
	if (AmmoPool != nullptr)
	{
		APooledObject* Dagger = AmmoPool->SpawnPooledObject(); // Spawn a dagger from the AmmoPool
		FVector FirstDaggerLocation = Dagger->GetActorLocation(); // Get the location of the first dagger

		if (LoadGame->GetDaggerCountUpgradeLevel() >= 2) // If the dagger has been upgraded at least once
		{
			FVector SecondDaggerLocation = FirstDaggerLocation + FVector(0, 0, 10); // Create a new location for the second dagger slightly above the first one
			APooledObject* ExtraDagger = AmmoPool->SpawnPooledObject(); // Spawn a dagger from the AmmoPool
			ExtraDagger->SetActorLocation(SecondDaggerLocation); // Set the location of the second dagger
		}
		if (LoadGame->GetDaggerCountUpgradeLevel() >= 3) // If the dagger has been upgraded at least twice
		{
			FVector ThirdDaggerLocation = FirstDaggerLocation + FVector(0, 0, -10); // Create a new location for the third dagger slightly below the first one
			APooledObject* Extra2Dagger = AmmoPool->SpawnPooledObject(); // Spawn a dagger from the AmmoPool
			Extra2Dagger->SetActorLocation(ThirdDaggerLocation); // Set the location of the third dagger
		}
		if (LoadGame->GetDaggerCountUpgradeLevel() >= 4) // If the dagger has been upgraded at least three times
		{
			FVector FourthDaggerLocation = FirstDaggerLocation + FVector(0, 0, -20); // Create a new location for the fourth dagger slightly below the third one
			APooledObject* Extra3Dagger = AmmoPool->SpawnPooledObject(); // Spawn a dagger from the AmmoPool
			Extra3Dagger->SetActorLocation(FourthDaggerLocation); // Set the location of the dagger
		}
	}
}

void APlayer_Character::KillPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Player is Dead"));
	CurrentAnimationState = KnightAnimationState::AS_DEAD;
	ProcessAnimStateMachine();

	USurvivorsSave* SaveGame = Cast<USurvivorsSave>(UGameplayStatics::CreateSaveGameObject(USurvivorsSave::StaticClass()));
	ASurvivorsGameModeBase* GameMode = Cast<ASurvivorsGameModeBase>(GetWorld()->GetAuthGameMode());
	SaveGame->CurrentCoins = GameMode->GetCoinTotal();
	UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("MySaveSlot"), 0); // Save the player's coins to the save slot
}

void APlayer_Character::WinGame()
{

	UE_LOG(LogTemp, Warning, TEXT("Player has won!"));
	CurrentAnimationState = KnightAnimationState::AS_WON;
	ProcessAnimStateMachine();

	USurvivorsSave* SaveGame = Cast<USurvivorsSave>(UGameplayStatics::CreateSaveGameObject(USurvivorsSave::StaticClass()));
	ASurvivorsGameModeBase* GameMode = Cast<ASurvivorsGameModeBase>(GetWorld()->GetAuthGameMode());
	SaveGame->CurrentCoins = GameMode->GetCoinTotal();
	UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("MySaveSlot"), 0); // Save the player's coins to the save slot
}

// Called when the game starts or when spawned
void APlayer_Character::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Player_Character BeginPlay called."));

	USurvivorsSave* LoadGame = Cast<USurvivorsSave>(UGameplayStatics::LoadGameFromSlot(TEXT("MySaveSlot"), 0)); // Load the save game from the save slot

	GetWorld()->GetTimerManager().SetTimer(WinTimer, this, &APlayer_Character::WinGame, 300.0f, false);

	CurrentAnimationState = KnightAnimationState::AS_IDLE;
	OldAnimationState = KnightAnimationState::AS_EMPTY;
	ProcessAnimStateMachine();

	ASurvivorsGameModeBase* GameMode = Cast<ASurvivorsGameModeBase>(GetWorld()->GetAuthGameMode()); // Get the game mode
	ResetHealth(); // Reset the player's health
	LastDirection = FVector(1, 0, 0); // Set the last direction to right
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayer_Character::SpawnDagger, 1.0f, true); // Set a timer to spawn a dagger every second
}