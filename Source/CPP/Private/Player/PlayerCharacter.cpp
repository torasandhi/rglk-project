// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter(): InputMappingContext(nullptr), IA_Move(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	TargetDirection = FVector::ZeroVector;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
    
	RootComponent = CapsuleComponent;

	CapsuleComponent->InitCapsuleSize(40.0f, 90.0f); // Radius, Half-Height
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn")); // Standard collision profile

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh->SetupAttachment(RootComponent);
	PlayerMesh->SetCollisionProfileName(TEXT("NoCollision")); // Let the Capsule handle collision!
}

float APlayerCharacter::GetSpeed() const { return MoveSpeed; }


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!TargetDirection.IsZero())
	{
		FVector NewLocation = GetActorLocation() + (TargetDirection * MoveSpeed * DeltaTime);
		SetActorLocation(NewLocation, true);

		FRotator TargetRotation = TargetDirection.Rotation();

		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, TurnSpeed);
		SetActorRotation(NewRotation);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Triggered: Updates direction while holding key
		EnhancedInputComponent->BindAction(
			IA_Move,
			ETriggerEvent::Triggered,
			this,
			&APlayerCharacter::InputMove
			);

		// Completed: Resets direction when key is released so we stop moving
		EnhancedInputComponent->BindAction(
			IA_Move,
			ETriggerEvent::Completed,
			this,
			&APlayerCharacter::InputMove
			);
	}
}

void APlayerCharacter::InputMove(const FInputActionValue& value)
{
	// 1. Read Input (X = Left/Right, Y = Forward/Backward)
	FVector2D InputVector = value.Get<FVector2D>();

	if (Controller && !InputVector.IsZero())
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0);

		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		TargetDirection = (ForwardDir * InputVector.Y) + (RightDir * InputVector.X);
		TargetDirection.Normalize();
	}
	else
	{
		TargetDirection = FVector::ZeroVector;
	}
}

