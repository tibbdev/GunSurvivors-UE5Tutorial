
#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphereComp = CreateDefaultSubobject<USphereComponent>("CollisionSphereComp");
	SetRootComponent(CollisionSphereComp);

	BulletSprite = CreateDefaultSubobject<UPaperSpriteComponent>("BulletSprite");
	BulletSprite->SetupAttachment(CollisionSphereComp);

	MovementDirection = FVector2D(1.0f, 0.0f);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	Launch(MovementSpeed, MovementDirection);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLaunched)
	{
		double move_length = MovementDirection.Length();
		FVector location = GetActorLocation();


		if (move_length) // if length is anything > 0
		{
			if (1 < move_length)
			{
				MovementDirection.Normalize();
			}

			FVector2D move_distance = DeltaTime * MovementSpeed * MovementDirection;

			// FVector location = GetActorLocation();
			FVector new_location = location + FVector(move_distance.X, 0, move_distance.Y);

			// if (!IsInMapBounds(new_location.X, &HorizontalLimits))
			// {
			// 	new_location.X = location.X;
			// }
			// 
			// if (!IsInMapBounds(new_location.Z, &VerticalLimits))
			// {
			// 	new_location.Z = location.Z;
			// }

			SetActorLocation(new_location);
		}
	}
}

void ABullet::Launch(float speed, FVector2D direction)
{
	if (IsLaunched) return;

	// Launch that Bullet!
	IsLaunched = true;

	MovementDirection = direction;
	MovementSpeed = speed;

	GetWorldTimerManager().SetTimer(DeleteTimer, this, &ABullet::OnDeleteTimerTimeout, 1.0f, false, BulletLifetime);
}

void ABullet::OnDeleteTimerTimeout(void)
{
	Destroy();
}

