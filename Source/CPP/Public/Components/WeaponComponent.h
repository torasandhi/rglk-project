// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void PerformAttack();
protected:
	virtual void BeginPlay() override;
	void TriggerSlashEffect();
	UPROPERTY(EditAnywhere, Category = "Combat")
	float BaseDamage = 10.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange = 150.f;
};
