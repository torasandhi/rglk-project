// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Projectile.h"
#include "ObjectPoolSubsystem.h"

AProjectile::AProjectile()
{
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovement")
	);

	PrimaryActorTick.bCanEverTick = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::SetObjectOwner(FString newOwner)
{
	owner = newOwner;
}

void AProjectile::InitVelocity(const FVector& Dir)
{
	ProjectileMovement->Velocity = Dir * speed;
}

void AProjectile::ReturnSelfToPool()
{
	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		Pool->ReturnActorToPool(this);
	}
}

void AProjectile::OnSpawnFromPool_Implementation()
{
	GetWorldTimerManager().ClearTimer(ProjectileTimer);

	GetWorldTimerManager().SetTimer(
		ProjectileTimer,
		this,
		&AProjectile::ReturnSelfToPool,
		LifeTime,
		false
	);
}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
