#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "UPoolableInterface.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class CPP_API AProjectile : public AActor, public IPoolableInterface
{
	GENERATED_BODY()

public:
	AProjectile();

	virtual void BeginPlay() override;
	virtual void Activate(const FVector& InDirection, AActor* Owner);
	virtual void OnSpawnFromPool_Implementation() override;
	virtual FOnReturnedToPool& OnReturnedToPool() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void ReturnActorToPool();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* SphereCollider;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float BaseDamage = 10.f;
	UPROPERTY(EditDefaultsOnly, Category="Projectile Settings")
	float MoveSpeed = 6000.f;
	UPROPERTY(EditDefaultsOnly, Category="Projectile Settings")
	float LifeTime = 2.f;

protected:
	FOnReturnedToPool ReturnToPool;
	FTimerHandle LifeTimerHandle;
};
