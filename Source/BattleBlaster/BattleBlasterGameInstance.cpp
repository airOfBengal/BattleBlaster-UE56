// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UBattleBlasterGameInstance::ChangeLevel(int32 Index)
{
	if (Index > 0 && Index <= LastLevelIndex)
	{
		FString LevelName = FString::Printf(TEXT("Level_%d"), Index);
		UGameplayStatics::OpenLevel(GetWorld(), *LevelName);
	}
}

void UBattleBlasterGameInstance::LoadNextLevel()
{
	if (CurrentLevelIndex + 1 > LastLevelIndex)
	{
		CurrentLevelIndex = 1;
	}
	else
	{
		CurrentLevelIndex++;
	}
	ChangeLevel(CurrentLevelIndex);
}


void UBattleBlasterGameInstance::RestartCurrentLevel()
{
	ChangeLevel(CurrentLevelIndex);
}

void UBattleBlasterGameInstance::RestartGame()
{
	ChangeLevel(1);
}