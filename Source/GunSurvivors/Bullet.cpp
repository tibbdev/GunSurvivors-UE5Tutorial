
#include "Bullet.h"

#include "Enemy.h"

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

	CollisionSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OverlapBegin);
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

			FVector new_location = location + FVector(move_distance.X, 0, move_distance.Y);

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

void ABullet::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if ((NULL != Enemy) && Enemy->IsAlive)
	{
		DisableBullet();
		Enemy->Die();
	}
}

void ABullet::DisableBullet(void)
{
	if (IsDisabled) return;

	IsDisabled = true;

	CollisionSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BulletSprite->DestroyComponent();

}
