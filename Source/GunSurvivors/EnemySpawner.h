// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Enemy.h"
#include "Engine/TimerHandle.h"

#include "TopDownCharacter.h"

#include "GunSurvivorGameMode.h"

#include "EnemySpawner.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime = 1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDistance = 400.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;

	FTimerHandle SpawnTimer;

	AEnemySpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void OnSpawnTimerElapsed(void);

	void StartSpawning(void);
	void StopSpawning(void);

	void SpawnEnemy(void);

	void SetupEnemy(AEnemy * enemy);

	UFUNCTION()
	void OnEnemyDead();

private:
	float m_SpawnRotationRads = 0.0f;
	ATopDownCharacter * m_Player;
	uint32 m_SpawnCount = 0;
	AGunSurvivorGameMode* m_GSGameMode;

};
