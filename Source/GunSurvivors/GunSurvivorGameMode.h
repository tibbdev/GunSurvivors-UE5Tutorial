// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GunSurvivorGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GUNSURVIVORS_API AGunSurvivorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int64 GameScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 GameScoreIncrementer = 10;

	AGunSurvivorGameMode();

	virtual void BeginPlay() override;

	void IncreaseScore(void);
	void ResetScore(void);
	
};
