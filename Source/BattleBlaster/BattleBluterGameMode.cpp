// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBluterGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "BattleBlasterGameInstance.h"

#include "Tower.h"

void ABattleBluterGameMode::BeginPlay()
{
	Super::BeginPlay();


	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();

	UE_LOG(LogTemp, Display, TEXT("Number of towers: %d"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Display, TEXT("GameMode: Failed to find the tank actor!"));
		}

		int32 i = 0;
		while (i < TowerCount)
		{
			AActor* TowerActor = Towers[i++];
			if (TowerActor)
			{
				ATower* Tower = Cast<ATower>(TowerActor);
				if (Tower && Tank)
				{
					Tower->Tank = Tank;
					UE_LOG(LogTemp, Display, TEXT("%s setting the tank variable!"), *Tower->GetActorNameOrLabel());
				}
			}
		}
	}
}

void ABattleBluterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameover = false;

	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		IsGameover = true;
	}
	else
	{
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower) {
			TowerCount--;

			if (TowerCount == 0)
			{
				IsGameover = true;
				IsVictory = true;
			}

			DeadTower->HandleDestruction();
		}
	}

	if (IsGameover)
	{
		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBluterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}
}

void ABattleBluterGameMode::OnGameOverTimerTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance)
		{
			if (IsVictory)
			{
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}
}
