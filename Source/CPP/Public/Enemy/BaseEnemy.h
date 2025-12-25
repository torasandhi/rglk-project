// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPoolableInterface.h"
#include "GameFramework/Pawn.h"
#include "BaseEnemy.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Chasing,
	Attacking,
};

UCLASS()
class CPP_API ABaseEnemy : public APawn, public IPoolableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEnemy();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetTarget();
	virtual void LookAtTarget(float DeltaTime);
	virtual void MoveTowardsTarget(float DeltaTime);
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
	                       bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
	                       const FHitResult& Hit) override;
	virtual void OnDeath();

private:
	UPROPERTY(VisibleAnywhere, Category= "Components")
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, Category= "Components")
	UStaticMeshComponent* EnemyMesh;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 CurrentHealth = 600.f;
	UPROPERTY(EditAnywhere, Category = "BaseStat")
	int32 BaseHealth = 600.f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float MoveSpeed = 10.f;
	UPROPERTY(EditAnywhere, Category = "BaseStat")
	float BaseMoveSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float Damage = 10.f;
	UPROPERTY(EditAnywhere, Category = "BaseStat")
	float BaseDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float TurnSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "BaseStat")
	float stopDistance;

	UPROPERTY(VisibleAnywhere, Category = "BaseStat")
	AActor* Target;
	
	EEnemyState CurrentState = EEnemyState::Chasing;
	FVector CurrentPushVelocity;
	FVector SeparationDirection;
	float PushFriction;
};
