// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent2.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent2::UHealthComponent2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent2::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Health = MaxHealth;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent2::OnDamageTaken);

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode)
	{
		BattleBlusterGameMode = Cast<ABattleBluterGameMode>(GameMode);		
	}
}


// Called every frame
void UHealthComponent2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent2::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage > 0.0f)
	{
		Health -= Damage;
		if (Health <= 0.0f) 
		{
			if (BattleBlusterGameMode)
			{
				BattleBlusterGameMode->ActorDied(DamagedActor);
			}
		}
	}
}

