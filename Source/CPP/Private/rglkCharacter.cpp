// Fill out your copyright notice in the Description page of Project Settings.


#include "rglkCharacter.h"

#include "Components/WeaponComponent.h"
#include "CPP/CPP.h"
#include "CPP/Data/CharacterStatsData.h"
#include "GameFramework/CharacterMovementComponent.h"

ArglkCharacter::ArglkCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configure standard  movement (AI and Player)
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	WeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
}

void ArglkCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void ArglkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ArglkCharacter::ApplyBaseStats(const TCHAR* DebugString)
{
	if (const FCharacterStats* Stats =
		CharacterStats.GetRow<FCharacterStats>(DebugString))
	{
		MaxHealth = Stats->Health;
		BaseDamage = Stats->Damage;

		CurrentHealth = MaxHealth;

		// if (GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Actor : %s"), *GetName()));
		// 	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
		// 	                                 FString::Printf(TEXT("Health: %d"), static_cast<int>(CurrentHealth)));
		// 	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
		// 	                                 FString::Printf(TEXT("BaseDamage: %d"), static_cast<int>(BaseDamage)));
		// }
	}
}

void ArglkCharacter::Attack()
{
	WeaponComp->PerformAttack();
}

void ArglkCharacter::Die()
{
	if (bIsDead) return;
}

void ArglkCharacter::PlayAttackEffects() {}

float ArglkCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser);

	CurrentHealth -= ActualDamage;
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(
	// 		-1,
	// 		2.f,
	// 		FColor::Green,
	// 		FString::Printf(
	// 			TEXT("%s | CurrentHealth: %.2f"),
	// 			*GetName(),
	// 			CurrentHealth
	// 		)
	// 	);
	// }

	if (CurrentHealth <= 0)
	{
		Die();
	}

	return ActualDamage;
}
