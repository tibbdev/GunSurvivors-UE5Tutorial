#include "TopDownCharacter.h"

ATopDownCharacter::ATopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	CharacterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp");;
	CharacterFlipbook->SetupAttachment(RootComponent);
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

