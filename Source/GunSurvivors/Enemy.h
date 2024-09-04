// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"

#include "TopDownCharacter.h"

#include "Engine/TimerHandle.h"

#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeadDelegate);

UCLASS()
class GUNSURVIVORS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* EnemyFlipbook;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// UPaperFlipbook* IdleFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* DeadFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* RunningFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ATopDownCharacter* Player;

	UPROPERTY(BlueprintReadWrite)
	FVector2D MovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanFollow = false;

	FTimerHandle DeadTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeadTime = 10.0f;

	FEnemyDeadDelegate EnemyDeadDelegate;

	// Sets default values for this actor's properties
	AEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Die(void);

	void OnDieTimerElapsed(void);
};
