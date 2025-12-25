// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPoolableInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class CPP_API AProjectile : public AActor, public IPoolableInterface
{
	GENERATED_BODY()

public:
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetObjectOwner(FString owner);
	
private:
	UPROPERTY(EditAnywhere, Category = "Stats")
	float speed = 10.f;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float LifeTime = 5.f;
	UProjectileMovementComponent* ProjectileMovement;
	FTimerHandle ProjectileTimer;

	FString  owner;
	void InitVelocity(const FVector& Dir);
	void ReturnSelfToPool();
	virtual void OnSpawnFromPool_Implementation() override;
};
