#include "TopDownCharacter.h"

ATopDownCharacter::ATopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	CharacterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
	CharacterFlipbook->SetupAttachment(CapsuleComp);
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* player_controller = Cast<APlayerController>(Controller);

	if (NULL != player_controller)
	{
		UEnhancedInputLocalPlayerSubsystem * subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(player_controller->GetLocalPlayer());

		if (NULL != subsystem)
		{
			subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMove)
	{
		double move_length = MovementDirection.Length();
		
		if (move_length) // if length is anything > 0
		{
			if (1 < move_length)
			{
				MovementDirection.Normalize();
			}

			FVector2D move_distance = DeltaTime * MovementSpeed * MovementDirection;

			FVector location = GetActorLocation();
			location += FVector(move_distance.X, 0, move_distance.Y);

			SetActorLocation(location);
		}
	}
}

void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input_component = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (NULL != input_component)
	{
		input_component->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Shoot);
		input_component->BindAction(ShootAction, ETriggerEvent::Started, this, &ATopDownCharacter::Shoot);

		input_component->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::MoveTriggered);
		input_component->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopDownCharacter::MoveCompleted);
		input_component->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ATopDownCharacter::MoveCompleted);
	}
}

void ATopDownCharacter::MoveTriggered(const FInputActionValue& Value)
{
	FVector2D movement_value = Value.Get<FVector2D>();

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, movement_value.ToString());

	if (CanMove)
	{
		MovementDirection = movement_value;
	}
}

void ATopDownCharacter::MoveCompleted(const FInputActionValue& Value)
{
	//FVector2D movement_value = Value.Get<FVector2D>();

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("move end"));

	MovementDirection = FVector2D( 0, 0 );
}

void ATopDownCharacter::Shoot(const FInputActionValue& Value)
{
	//bool shoot_value = Value.Get<bool>();

	//if (shoot_value)
	//{
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Bang!"));
	//}
}

