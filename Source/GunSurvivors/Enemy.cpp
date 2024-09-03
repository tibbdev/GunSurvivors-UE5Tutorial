#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	SetRootComponent(CapsuleComp);

	EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("EnemyFlipbook");
	EnemyFlipbook->SetupAttachment(RootComponent);

	EnemyFlipbook->SetFlipbook(RunningFlipbook);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();


	if (NULL == Player)
	{
		AActor * PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopDownCharacter::StaticClass());
		if (NULL != PlayerActor)
		{
			Player = Cast<ATopDownCharacter>(PlayerActor);
			CanFollow = true;
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((NULL != Player))
	{
		if(IsAlive)
		{
			FVector player_location = Player->GetActorLocation();
			FVector current_location = GetActorLocation();

			MovementDirection = { player_location.X - current_location.X, player_location.Z - current_location.Z };
			FVector2D poop = MovementDirection;

			CanFollow = (StopDistance < MovementDirection.Length());

			MovementDirection.Normalize();

			FVector2D move_distance = DeltaTime * MovementSpeed * MovementDirection;

			//GEngine->AddOnScreenDebugMessage(10, 0.5f, (CanFollow ? FColor::Green : FColor::Red), poop.ToString());

			poop.X = move_distance.X;
			poop.Y = 0.0f;
			poop.Normalize();

			SetActorRelativeScale3D(FVector(poop.X, 1.0f, 1.0f));

			if (CanFollow)
			{
				SetActorLocation(FVector(current_location.X + move_distance.X, 0.0f, current_location.Z + move_distance.Y));
			}
		}
	}

	if (!IsAlive)
	{
		EnemyFlipbook->SetFlipbook(DeadFlipbook);
	}
}
