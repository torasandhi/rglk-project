#include "Enemy/rglkEnemyCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "ObjectPoolSubsystem.h"
#include "CPP/CPP.h"

ArglkEnemyCharacter::ArglkEnemyCharacter()
{
	// 1. Configure Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); // Turn speed

	MoveSpeed = 400.0f;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	// 2. Configure Collision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}

IPoolableInterface::FOnReturnedToPool& ArglkEnemyCharacter::OnReturnedToPool()
{
	return ReturnToPool;
}

void ArglkEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	FindTarget();
	SetState(EEnemyState::Chasing);
}

void ArglkEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateState(DeltaTime);
}

void ArglkEnemyCharacter::FindTarget()
{
	TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ArglkEnemyCharacter::ChaseTarget()
{
	if (!TargetActor) return;

	float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());
	if (Distance > StopDistance)
	{
		FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		AddMovementInput(Direction, 1.0f);
	}
	else
	{
	}
}

bool ArglkEnemyCharacter::TimerManager(const FTimerHandle MyTimerHandle) const
{
	FTimerManager& TimerManager = GetWorldTimerManager();

	if (TimerManager.IsTimerActive(MyTimerHandle))
		return true;
	else
		return false;
}


void ArglkEnemyCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                    bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                                    const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other)
	{
		FVector PushDir = GetActorLocation() - Other->GetActorLocation();
		PushDir.Z = 0; // Don't push up/down
		PushDir.Normalize();

		if (Other->ActorHasTag("Player"))
		{
			// Big push from player
			SeparationForce += (PushDir * 2.0f);
		}
		else if (Other->ActorHasTag("Enemy"))
		{
			SeparationForce += (PushDir * 0.5f);
		}
	}
}

void ArglkEnemyCharacter::Die()
{
	Super::Die();

	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		Pool->ReturnActorToPool(this);
		ReturnToPool.Broadcast(this);
	}
}

void ArglkEnemyCharacter::Attack()
{
	Super::Attack();
	AttackTimer.Invalidate();
}

void ArglkEnemyCharacter::OnSpawnFromPool_Implementation()
{
	CurrentHealth = MaxHealth;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	SeparationForce = FVector::ZeroVector;
	FindTarget();

	// Ensure Physics is on
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}


// Refactor to 1 class per state
// -- Handle States --

void ArglkEnemyCharacter::SetState(EEnemyState NewState)
{
	if (CurrentState == NewState) return;

	ExitState(CurrentState);
	CurrentState = NewState;
	EnterState(CurrentState);
}

void ArglkEnemyCharacter::EnterState(EEnemyState State)
{
	switch (State)
	{
	case EEnemyState::Chasing:

		break;

	case EEnemyState::Attacking:
		if (!WeaponComp)
			SetState(EEnemyState::Chasing);
		break;
	}
}

void ArglkEnemyCharacter::ExitState(EEnemyState State)
{
	switch (State)
	{
	case EEnemyState::Attacking:

		break;

	default:
		break;
	}
}

void ArglkEnemyCharacter::UpdateState(float DeltaTime)
{
	switch (CurrentState)
	{
	case EEnemyState::Chasing:
		UpdateChase(DeltaTime);
		break;

	case EEnemyState::Attacking:
		UpdateAttack(DeltaTime);
		break;
	}
}

void ArglkEnemyCharacter::UpdateChase(float DeltaTime)
{
	float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());

	if (TargetActor)
		ChaseTarget();
	else
		FindTarget();

	// basic seperation logic
	if (!SeparationForce.IsZero())
	{
		AddMovementInput(SeparationForce, 1.0f);
		SeparationForce = FMath::VInterpTo(
			SeparationForce,
			FVector::ZeroVector,
			DeltaTime,
			5.0f);
	}

	if (Distance < StopDistance)
		SetState(EEnemyState::Attacking);
}

void ArglkEnemyCharacter::UpdateAttack(float DeltaTime)
{
	if (TimerManager(AttackTimer)) return;
	GetWorldTimerManager().SetTimer(
		AttackTimer,
		this,
		&ArglkEnemyCharacter::Attack,
		2.f,
		false
	);

	float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());
	if (Distance > StopDistance)
		SetState(EEnemyState::Chasing);
}

float ArglkEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHealth -= ActualDamage;
	if (CurrentHealth <= 0)
	{
		Die();
	}
	
	return ActualDamage;
}
