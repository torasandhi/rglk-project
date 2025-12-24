// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Microsoft/COMPointer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
AItem::AItem(): IMC_Controls(nullptr), IA_Move(nullptr), forwardValue(0), rightValue(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	InitializePlayerControls();
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::InitializePlayerControls() const
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(IMC_Controls, 0);
			}
		}
	}
}


void AItem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EIC =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(
			IA_Move,
			ETriggerEvent::Triggered,
			this,
			&AItem::Move);
	}
}

void AItem::SpawnCube()
{
	if (CubeClass == nullptr)
		UE_LOG(LogTemp, Error, TEXT("Cube not found"));

	FVector cubeLocation = GetActorLocation() + FVector(200.f, 0.f,0.f);
	FRotator cubeRotation = FRotator::ZeroRotator;

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* spawnedCube = GetWorld()->SpawnActor<AActor>(
		CubeClass,
		cubeLocation,
		cubeRotation,
		spawnParams
	);

	if (spawnedCube != nullptr)
	{
		spawnedCubes.Add(spawnedCube);
	}
}

void AItem::DestroyAllCubes()
{
	for (int32 i = spawnedCubes.Num() - 1; i >= 0; i--)
	{
		if (IsValid(spawnedCubes[i]))
		{
			spawnedCubes[i]->Destroy();
		}

		spawnedCubes.RemoveAt(i);
	}
}

void AItem::Move(const FInputActionValue& Value)
{
	FVector2D moveInput = Value.Get<FVector2D>();

	FVector Direction =
		GetActorForwardVector() * moveInput.Y + 
		GetActorRightVector() * moveInput.X;

	AddActorWorldOffset(
		Direction * 400.f * GetWorld()->GetDeltaSeconds(),
		true
	);
}




