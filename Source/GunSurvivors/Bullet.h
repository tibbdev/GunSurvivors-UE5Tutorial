// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/TimerHandle.h"

#include "Bullet.generated.h"

UCLASS()
class GUNSURVIVORS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* CollisionSphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* BulletSprite;

	UPROPERTY(BlueprintReadWrite)
	FVector2D MovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsLaunched;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletLifetime = 10.0f;

	FTimerHandle DeleteTimer;

	// Sets default values for this actor's properties
	ABullet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Launch(float speed, FVector2D direction);

	void OnDeleteTimerTimeout(void);
};
