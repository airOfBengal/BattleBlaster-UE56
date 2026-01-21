// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBluterGameMode.h"

#include "Kismet/GameplayStatics.h"
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
