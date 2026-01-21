// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	BaseComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseComp->SetupAttachment(CapsuleComp);

	TurretComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretComp->SetupAttachment(BaseComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretComp);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretComp->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.0f, VectorToTarget.Rotation().Yaw, 0.0f);

	FRotator InterpolatedRotation =  FMath::RInterpTo(
		TurretComp->GetComponentRotation(), 
		LookAtRotation, 
		GetWorld()->GetDeltaSeconds(), 
		10.0f);

	TurretComp->SetWorldRotation(InterpolatedRotation);
}

void ABasePawn::Fire()
{
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	DrawDebugSphere(GetWorld(), SpawnLocation, 30.0f, 20, FColor::Red, false, 2.0f);
}

