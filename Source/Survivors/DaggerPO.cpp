

#include "DaggerPO.h"
#include <PaperSpriteComponent.h>
#include <Components/CapsuleComponent.h>
#include <Engine.h>
#include "BaseEnemy.h"

ADaggerPO::ADaggerPO()
{
	PrimaryActorTick.bCanEverTick = true;

	// the below creates the sprite, collider, and overlap trigger for the dagger
	MySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DaggerSprite"));
	MyBodyCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DaggerCollider"));
	OverlapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Detector"));
	OverlapTrigger->SetBoxExtent(FVector(32, 32, 32));
	RootComponent = MyBodyCollider;
	MySprite->SetupAttachment(RootComponent);
	OverlapTrigger->SetupAttachment(RootComponent);
}

void ADaggerPO::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveDagger();
}

void ADaggerPO::MoveDagger()
{
	MyBodyCollider->SetPhysicsLinearVelocity(FVector(direction) * DaggerSpeed); // get the player's direction and then move in that direction at the dagger's speed
}

void ADaggerPO::SetActive(bool IsActive)
{
	Super::SetActive(IsActive);
	direction = Player->GetLastDirection(); // get the player's last direction to see where the dagger was thrown

	// the below rotates the dagger based on the direction it was thrown
	if (direction.X > 0 && direction.Z == 0) // if the dagger is facing right
	{
		SetActorRotation(FRotator(315, 0, 0)); // rotate the dagger to face right
	}
	else if (direction.X > 0 && direction.Z > 0) // if the dagger is facing up and right
	{
		SetActorRotation(FRotator(0, 0, 0));
	}
	else if (direction.X == 0 && direction.Z > 0) // if the dagger is facing up
	{
		SetActorRotation(FRotator(45, 0, 0));
	}
	else if (direction.X < 0 && direction.Z > 0) // if the dagger is facing up and left
	{
		SetActorRotation(FRotator(90, 0, 0));
	}
	else if (direction.X < 0 && direction.Z == 0) // if the dagger is facing left
	{
		SetActorRotation(FRotator(135, 0, 0));
	}
	else if (direction.X < 0 && direction.Z < 0) // if the dagger is facing down and left
	{
		SetActorRotation(FRotator(180, 0, 0));
	}
	else if (direction.X == 0 && direction.Z < 0) // if the dagger is facing down
	{
		SetActorRotation(FRotator(225, 0, 0)); 
	}
	else if (direction.X > 0 && direction.Z < 0) // if the dagger is facing down and right
	{
		SetActorRotation(FRotator(270, 0, 0));
	}
}

void ADaggerPO::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("DaggerPO BeginPlay called."));
	OverlapTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADaggerPO::OnOverlap); // bind the OnOverlap method to the overlap trigger

	Player = Cast<APlayer_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)); // get the player character
}

void ADaggerPO::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ABaseEnemy::StaticClass())) // if the dagger hits an enemy, deal damage to the enemy and deactivate the dagger
	{
		Cast<ABaseEnemy>(OtherActor)->Damage(DaggerDamage);
		Deactivate();
	}
}
