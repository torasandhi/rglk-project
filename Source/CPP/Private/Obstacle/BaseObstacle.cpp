// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle/BaseObstacle.h"

#include "ObjectPoolSubsystem.h"

// Sets default values
ABaseObstacle::ABaseObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseObstacle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector forwardDir = GetActorForwardVector();

	FVector deltaLocation = forwardDir * moveSpeed;
	AddActorWorldOffset(deltaLocation);
}

void ABaseObstacle::OnSpawnFromPool_Implementation()
{
	GetWorldTimerManager().ClearTimer(ReturnToPoolTImer);

	GetWorldTimerManager().SetTimer(
		ReturnToPoolTImer,
		this,
		&ABaseObstacle::ReturnSelfToPool,
		LifeTime,
		false
	);
}

void ABaseObstacle::ReturnSelfToPool()
{
	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		Pool->ReturnActorToPool(this);
	}
}
