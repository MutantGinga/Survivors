#include "XPGem.h"
#include "Player_Character.h"
#include <PaperSpriteComponent.h>
#include "Components/BoxComponent.h"
#include <Engine.h>
#include "SurvivorsGameModeBase.h"

// Sets default values
AXPGem::AXPGem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// creates the sprite component and overlap trigger for the XP Gem
	MySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("XPSprite"));
	OverlapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Detector"));
	OverlapTrigger->SetBoxExtent(FVector(32, 32, 32));
	RootComponent = MySprite;
	OverlapTrigger->SetupAttachment(RootComponent);
}

int AXPGem::GetCoinValue()
{
	return CoinValue;
}

// Called when the game starts or when spawned
void AXPGem::BeginPlay()
{
	Super::BeginPlay();
	OverlapTrigger->OnComponentBeginOverlap.AddDynamic(this, &AXPGem::OnOverlap);
	PlayerCharacter = Cast<APlayer_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AXPGem::Tick(float DeltaTime) // Called every frame - does an XP gem need to tick?
{
	Super::Tick(DeltaTime);
}

void AXPGem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	if (OtherActor->IsA(APlayer_Character::StaticClass())) // if the player overlaps with the XP gem, the player gains XP and the gem is destroyed
	{
		ASurvivorsGameModeBase* GameMode = Cast<ASurvivorsGameModeBase>(GetWorld()->GetAuthGameMode());
		GameMode->AddCoins(GetCoinValue());
		Destroy();
	}

}

