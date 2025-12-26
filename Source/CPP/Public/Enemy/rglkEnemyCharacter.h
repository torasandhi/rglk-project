#pragma once

#include "CoreMinimal.h"
#include "rglkCharacter.h"
#include "UPoolableInterface.h"
#include "rglkEnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Chasing				UMETA(DisplayName="Chasing"),
	Attacking			UMETA(DisplayName="Attacking")
};


UCLASS()
class CPP_API ArglkEnemyCharacter : public ArglkCharacter, public IPoolableInterface
{
	
	GENERATED_BODY()

public:
	ArglkEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Override NotifyHit to keep your "Bumping" logic
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
	                       bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
	                       const FHitResult& Hit) override;
	virtual void Die() override;
	virtual void Attack() override;

	// Interface Implementation
	virtual void OnSpawnFromPool_Implementation() override;

private:
	void FindTarget();
	void ChaseTarget();
	bool TimerManager(const FTimerHandle MyTimerHandle) const;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	AActor* TargetActor;
	UPROPERTY(EditAnywhere, Category = "AI")
	float StopDistance = 100.0f;
	
	FVector SeparationForce;
	FTimerHandle AttackTimer;


	// handle current state
	
	UPROPERTY(VisibleAnywhere, Category = "AI")
	EEnemyState CurrentState = EEnemyState::Chasing;
	
	void SetState(EEnemyState NewState);

	void EnterState(EEnemyState State);
	void ExitState(EEnemyState State);
	void UpdateState(float DeltaTime);

	void UpdateChase(float DeltaTime);
	void UpdateAttack(float DeltaTime);
};
