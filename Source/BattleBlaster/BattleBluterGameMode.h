// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Tank.h"

#include "BattleBluterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBluterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ATank* Tank;
	int32 TowerCount;

	UPROPERTY(EditAnywhere)
	float GameOverDelay = 2.0f;

	UPROPERTY(EditAnywhere)
	int32 CountdownDelay = 3;
	int32 CountdownSeconds;

	FTimerHandle CountdownTimerHandle;

	UPROPERTY(VisibleAnywhere)
	bool IsVictory = false;

	void ActorDied(AActor* DeadActor);
	void OnGameOverTimerTimeout();
	void OnCountdownTimerTimeout();
};
