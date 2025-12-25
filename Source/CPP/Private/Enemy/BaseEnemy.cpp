// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

#include "ObjectPoolSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

class UObjectPoolSubsystem;
// Sets default values
ABaseEnemy::ABaseEnemy() : stopDistance(10.0f), Target(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	CapsuleComponent->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	EnemyMesh->SetupAttachment(RootComponent);
	EnemyMesh->SetCollisionProfileName(TEXT("NoCollision")); // Let the Capsule handle collision!
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseEnemy::GetTarget()
{
	// 1. Just grab the player directly. No tags, no searching.
	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ABaseEnemy::LookAtTarget(float DeltaTime)
{
	if (!Target)
	{
		UE_LOG(LogTemp, Error, TEXT("target is null"));
		return;
	}

	FVector start = GetActorLocation();
	FVector destination = Target->GetActorLocation();
	FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, destination);

	FRotator newRotation = FMath::RInterpConstantTo(
		GetActorRotation(),
		lookAtRotation,
		DeltaTime,
		TurnSpeed
	);

	SetActorRotation(newRotation);
}

void ABaseEnemy::MoveTowardsTarget(float DeltaTime)
{
	if (!Target) return;

	CurrentPushVelocity = FMath::VInterpTo(CurrentPushVelocity, FVector::ZeroVector, DeltaTime, PushFriction);

	float distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());

	if (distance > stopDistance)
	{
		FVector forwardDirection = GetActorForwardVector();
		FVector deltaLocation = (forwardDirection * MoveSpeed) + (CurrentPushVelocity * DeltaTime);

		AddActorWorldOffset(deltaLocation, true);
	}
	else
	{
		if (!CurrentPushVelocity.IsNearlyZero())
		{
			AddActorWorldOffset(CurrentPushVelocity * DeltaTime, true);
		}
	}
}

void ABaseEnemy::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
                           bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                           const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (Other)
	{
		// 1. Calculate Push Direction
		FVector PushDir = GetActorLocation() - Other->GetActorLocation();
		PushDir.Z = 0;
		PushDir.Normalize();

		// 2. WEIGHTED PUSH
		if (Other->ActorHasTag("Player"))
		{
			SeparationDirection += (PushDir * 40.0f);
		}
		else if (Other->ActorHasTag("Enemy"))
		{
			SeparationDirection += (PushDir * 1.0f);
		}
	}
}


void ABaseEnemy::OnDeath()
{
	UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	if (Pool)
	{
		Pool->ReturnActorToPool(this);
	}
	else
	{
		Destroy();
	}
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		LookAtTarget(DeltaTime);
		MoveTowardsTarget(DeltaTime);
	}
	else
	{
		GetTarget();
	}

	if (!SeparationDirection.IsZero())
	{
		bool bIsUrgent = SeparationDirection.SizeSquared() > 25.0f; // 5*5 = 25
		float EscapeSpeed = bIsUrgent ? 800.0f : 200.0f;
		AddActorWorldOffset(SeparationDirection.GetSafeNormal() * EscapeSpeed * DeltaTime, true);
		SeparationDirection = FVector::ZeroVector;
	}
}
