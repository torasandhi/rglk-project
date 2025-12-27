#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "rglkCharacter.generated.h"

class UWeaponComponent;

UCLASS()
class CPP_API ArglkCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ArglkCharacter();

	virtual void Tick(float DeltaTime) override;

	// --- SHARED STATS --- //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MoveSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWeaponComponent* WeaponComp;

	// --- SHARED ACTIONS --- //

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Attack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;
};
