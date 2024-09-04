// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSurvivorGameMode.h"

AGunSurvivorGameMode::AGunSurvivorGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGunSurvivorGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AGunSurvivorGameMode::IncreaseScore(void)
{
	GameScore += GameScoreIncrementer;
	GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Yellow, FString::FromInt(GameScore));
}

void AGunSurvivorGameMode::ResetScore(void)
{
	GameScore = 0;
}
