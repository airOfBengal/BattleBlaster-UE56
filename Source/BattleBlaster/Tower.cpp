// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Tank)
	{
		FVector TankLocation = Tank->GetActorLocation();
		float DistanceToTank = FVector::Dist(TankLocation, GetActorLocation());

		if (DistanceToTank <= FireRange)
		{
			RotateTurret(Tank->GetActorLocation());
		}
	}
	
}
