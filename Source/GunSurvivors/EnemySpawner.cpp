#include "EnemySpawner.h"
#include "Kismet/GameplayStatics.h"

constexpr float  SPAWN_TIMER_REDUCE_BY				= 0.05f; // Seconds
constexpr float  TIMER_MINIMUM_INTERVAL				= 0.4f;  // Seconds
constexpr uint32 SPAWN_DIFFICULTY_INCREASE_INTERVAL = 10;    // Enemy Count

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (NULL == m_Player)
	{
		AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopDownCharacter::StaticClass());
		if (NULL != PlayerActor)
		{
			m_Player = Cast<ATopDownCharacter>(PlayerActor);
		}
	}

	StartSpawning();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::StartSpawning(void)
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::OnSpawnTimerElapsed, SpawnTime, true, SpawnTime);
}

void AEnemySpawner::StopSpawning(void)
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void AEnemySpawner::OnSpawnTimerElapsed(void)
{
	SpawnEnemy();
	m_SpawnCount++;
	if (0 == (m_SpawnCount % SPAWN_DIFFICULTY_INCREASE_INTERVAL))
	{
		StopSpawning();
		if (TIMER_MINIMUM_INTERVAL < SpawnTime)
		{
			SpawnTime -= SPAWN_TIMER_REDUCE_BY;
		}
		else
		{
			SpawnTime = TIMER_MINIMUM_INTERVAL;
		}
		StartSpawning();
	}
}

void AEnemySpawner::SpawnEnemy(void)
{
	SpawnLocation = FMath::VRand();
	SpawnLocation.Y = 0.0f;
	SpawnLocation.Normalize();

	AEnemy* enemy_to_spawn = GetWorld()->SpawnActor<AEnemy>(EnemyActorToSpawn, GetActorLocation() +  SpawnLocation * SpawnDistance, FRotator::ZeroRotator);

	enemy_to_spawn->Player = m_Player;
	enemy_to_spawn->CanFollow = true;
}

