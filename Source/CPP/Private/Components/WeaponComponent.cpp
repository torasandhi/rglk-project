// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponComponent.h"

#include "NiagaraComponent.h"
#include "rglkCharacter.h"
#include "CPP/CPP.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/rglkPlayerCharacter.h"


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	BaseDamage = GetOwner<ArglkCharacter>()->BaseDamage;
}

void UWeaponComponent::TriggerSlashEffect()
{
	// only for player character as of now
	if (ArglkPlayerCharacter* PCharacter = GetOwner<ArglkPlayerCharacter>())
	{
		if (UNiagaraComponent* SlashEffect = PCharacter->SlashEffect)
		{
			SlashEffect->SetCustomTimeDilation(2.f);
			SlashEffect->Activate();
		}
	}
}


void UWeaponComponent::PerformAttack()
{
	ArglkCharacter* Owner = GetOwner<ArglkCharacter>();
	if (!Owner) return;

	USceneComponent* FirePoint = Owner->FirePointComponent;
	FVector Start = FirePoint ? FirePoint->GetComponentLocation() : Owner->GetActorLocation();
	FVector Forward = FirePoint ? FirePoint->GetForwardVector() : Owner->GetActorForwardVector();
	FVector End = Start + (Forward * AttackRange);

	FHitResult Hit;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		End,
		20.f,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		Hit,
		true
	);
	
	if (bHit && Hit.GetActor())
	{
		UGameplayStatics::ApplyDamage(
			Hit.GetActor(),
			BaseDamage,
			Owner->GetInstigatorController(),
			Owner,
			UDamageType::StaticClass()
		);
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
	}

	TriggerSlashEffect();
}
