#include "TopDownCharacter.h"

#include "Kismet//KismetMathLibrary.h"

ATopDownCharacter::ATopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	CharacterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
	CharacterFlipbook->SetupAttachment(CapsuleComp);

	GunParent = CreateDefaultSubobject<USceneComponent>(TEXT("GunParent"));
	GunParent->SetupAttachment(CapsuleComp);

	GunSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSprite"));
	GunSprite->SetupAttachment(GunParent);

	BulletSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPoint"));
	BulletSpawnPoint->SetupAttachment(GunSprite);

	//BulletActorToSpawn = Create<TSubclassOf<ABullet>> (TEXT("BulletActor"));
	
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* player_controller = Cast<APlayerController>(Controller);

	if (NULL != player_controller)
	{
		player_controller->SetShowMouseCursor(true);

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

	// Move Player
	if (CanMove)
	{
		double move_length = MovementDirection.Length();
		FVector location = GetActorLocation();
		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::White, location.ToString());
		
		if (move_length) // if length is anything > 0
		{
			if (1 < move_length)
			{
				MovementDirection.Normalize();
			}

			FVector2D move_distance = DeltaTime * MovementSpeed * MovementDirection;

			//FVector location = GetActorLocation();
			FVector new_location = location + FVector(move_distance.X, 0, move_distance.Y);

			if (!IsInMapBounds(new_location.X, &HorizontalLimits))
			{
				new_location.X = location.X;
			}

			if (!IsInMapBounds(new_location.Z, &VerticalLimits))
			{
				new_location.Z = location.Z;
			}

			SetActorLocation(new_location);
		}
	}

	// Rotate Gun
	APlayerController* player_controller = Cast<APlayerController>(Controller);
	if (NULL != player_controller)
	{
		FVector world_location, world_direction;
		player_controller->DeprojectMousePositionToWorld(world_location, world_direction);

		FVector current_location = GetActorLocation();
		FVector start = FVector(current_location.X, 0, current_location.Z);
		FVector target = FVector(world_location.X, 0, world_location.Z);


		FRotator gun_rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
		GunParent->SetRelativeRotation(gun_rotator);
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

	GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Yellow, movement_value.ToString());

	if (CanMove)
	{
		MovementDirection = movement_value;
		CharacterFlipbook->SetFlipbook(RunFlipbook);

		if (movement_value.X)
		{
			FVector fb_scale = CharacterFlipbook->GetComponentScale();

			if (fb_scale.X != movement_value.X)
			{
				fb_scale.X = movement_value.X;
				CharacterFlipbook->SetWorldScale3D(fb_scale);
			}
		}
	}
}

void ATopDownCharacter::MoveCompleted(const FInputActionValue& Value)
{
	//FVector2D movement_value = Value.Get<FVector2D>();

	GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Green, TEXT("move end"));

	MovementDirection = FVector2D( 0, 0 );

	CharacterFlipbook->SetFlipbook(IdleFlipbook);
}

void ATopDownCharacter::Shoot(const FInputActionValue& Value)
{
	if(CanShoot)
	{
		CanShoot = false;

		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Bang!"));

		// Spawn Bullet
		ABullet * bullet = GetWorld()->SpawnActor<ABullet>(BulletActorToSpawn, BulletSpawnPoint->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));

		check(bullet);

		// Get mouse world location
		APlayerController* player_controller = Cast<APlayerController>(Controller);
		check(player_controller);
		FVector world_location, world_direction;
		player_controller->DeprojectMousePositionToWorld(world_location, world_direction);

		// Calculate bullet direction
		FVector current_location = GetActorLocation();
		FVector2D bullet_direction = FVector2D(world_location.X - current_location.X, world_location.Z - current_location.Z);
		bullet_direction.Normalize();

		// Send it!
		float bullet_speed = 300.0f;
		bullet->Launch(bullet_speed, bullet_direction);


		GetWorldTimerManager().SetTimer(CooldownTimer, this, &ATopDownCharacter::OnCooldownTimerElapsed, 1.0f, false, CooldownDurationSecs);
	}
}

bool ATopDownCharacter::IsInMapBounds(float position, FVector2D const* const limits)
{
	if ((limits->X < position) && (limits->Y > position))
	{
		return true;
	}
	return false;
}

void ATopDownCharacter::OnCooldownTimerElapsed(void)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Emerald, TEXT("!gnaB"));
	CanShoot = true;
}

