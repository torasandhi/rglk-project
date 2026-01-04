#include "Items/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	RootComponent = SphereCollider;
	SphereCollider->SetWorldScale3D(FVector(3.f, 3.f, 3.f));

	// Collision setup
	SphereCollider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SphereCollider;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = MoveSpeed;
	ProjectileMovement->MaxSpeed = MoveSpeed;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
}

void AProjectile::Activate(const FVector& InDirection, AActor* InOwner)
{
	SetOwner(InOwner);
	SetActorHiddenInGame(false);
	SphereCollider->ClearMoveIgnoreActors();
	if (InOwner)
	{
		SphereCollider->IgnoreActorWhenMoving(InOwner, true);
	}

	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = InDirection * MoveSpeed;
		ProjectileMovement->UpdateComponentVelocity();
		ProjectileMovement->Activate();
	}

	GetWorld()->GetTimerManager().SetTimer(LifeTimerHandle, this, &AProjectile::ReturnActorToPool, LifeTime, false);
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                            bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetOwner()) return;
	if (OtherActor->IsA(AProjectile::StaticClass())) return;
	if (!OtherActor || OtherActor == GetOwner()) return;
	if (OtherActor->GetOwner() == GetOwner()) return;

	bool bShooterIsEnemy = GetOwner() && GetOwner()->ActorHasTag("Enemy");
	bool bHitIsEnemy = OtherActor->ActorHasTag("Enemy");
	if (bShooterIsEnemy && bHitIsEnemy)
	{
		ReturnActorToPool();
		return;
	}

	UGameplayStatics::ApplyDamage(
		OtherActor,
		BaseDamage,
		GetOwner() ? GetOwner()->GetInstigatorController() : nullptr,
		this,
		UDamageType::StaticClass()
	);

	ReturnActorToPool();
}

void AProjectile::ReturnActorToPool()
{
	GetWorld()->GetTimerManager().ClearTimer(LifeTimerHandle);

	SetOwner(nullptr);
	SetActorHiddenInGame(true);
	SphereCollider->ClearMoveIgnoreActors();

	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Deactivate();
	}

	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		Pool->ReturnActorToPool(this);
	}
}

void AProjectile::OnSpawnFromPool_Implementation()
{
	IPoolableInterface::OnSpawnFromPool_Implementation();
}

IPoolableInterface::FOnReturnedToPool& AProjectile::OnReturnedToPool()
{
	return ReturnToPool;
}
