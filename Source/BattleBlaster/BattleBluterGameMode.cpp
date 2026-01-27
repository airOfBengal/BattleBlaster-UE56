// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBluterGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "Tower.h"

void ABattleBluterGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
	}

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

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText(FString::Printf(TEXT("Level %d\nGet Ready!"), BattleBlasterGameInstance->CurrentLevelIndex));
		}
	}

	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBluterGameMode::OnCountdownTimerTimeout, 1.0f, true);
}

void ABattleBluterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds--;
	if (CountdownSeconds > 0)
	{
		//UE_LOG(LogTemp, Display, TEXT("Countdown: %d"), CountdownSeconds);
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if(CountdownSeconds == 0)
	{
		//UE_LOG(LogTemp, Display, TEXT("Go!"));
		ScreenMessageWidget->SetMessageText("Go!");
		Tank->SetPlayerEnabled(true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		//UE_LOG(LogTemp, Display, TEXT("Clear timer"));
		//ScreenMessageWidget->SetMessageText("");
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
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
		FString GameOverString = IsVictory ? "Victory!" : "Defeat!";
		ScreenMessageWidget->SetMessageText(GameOverString);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBluterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}
}

void ABattleBluterGameMode::OnGameOverTimerTimeout()
{
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
