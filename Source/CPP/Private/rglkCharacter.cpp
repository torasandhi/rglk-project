// Fill out your copyright notice in the Description page of Project Settings.


#include "rglkCharacter.h"

#include "Components/WeaponComponent.h"
#include "CPP/CPP.h"
#include "GameFramework/CharacterMovementComponent.h"

ArglkCharacter::ArglkCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configure standard movement for both AI and Player
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

void ArglkCharacter::Attack()
{
	WeaponComp->PerformAttack();
}

void ArglkCharacter::Die()
{
	PRINT_DEBUG_MESSAGE(GetName() + " has Died");
}

float ArglkCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser);

	CurrentHealth -= ActualDamage;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.f,
			FColor::Green,
			FString::Printf(
				TEXT("%s | CurrentHealth: %.2f"),
				*GetName(),
				CurrentHealth
			)
		);
	}

	if (CurrentHealth <= 0)
	{
		Die();
		CurrentHealth = MaxHealth;
	}

	return ActualDamage;
}
