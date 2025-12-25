// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCapsuleComponent;
class UStaticMeshComponent;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EPlayerState: uint8
{
	Idle,
	Running,
	Attacking,
	Dashing
};


UCLASS()
class CPP_API APlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	float GetSpeed() const;

protected:
	UPROPERTY(EditAnywhere, Category = "References")
	FVector firePos;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 600.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed = 15.f;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Attack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlayerMesh;

	virtual void BeginPlay() override;
	void InputMove(const FInputActionValue& value);
	void Attack(const FInputActionValue& value);

private:
	FVector TargetDirection;
	EPlayerState CurrentState = EPlayerState::Idle;
};
