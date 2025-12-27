#include "Player/rglkPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "ObjectPoolSubsystem.h"

ArglkPlayerCharacter::ArglkPlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	
	CameraBoom->bUsePawnControlRotation = false; 
	CameraBoom->bInheritPitch = false; 
	CameraBoom->bInheritYaw = false;   
	CameraBoom->bInheritRoll = false;
	
	CameraBoom->SetRelativeRotation(FRotator(-50.0f, 0.0f, 0.0f));
	CameraBoom->TargetArmLength = 800.0f; // Distance from player
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ArglkPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void ArglkPlayerCharacter::Attack()
{
	Super::Attack();
}

void ArglkPlayerCharacter::Die()
{
	Super::Die();
}

void ArglkPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ArglkPlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (!WeaponComp) return;	
	
	Attack();	
}

float ArglkPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser);

	CurrentHealth -= ActualDamage;

	if (CurrentHealth <= 0)
	{
		Die();
		CurrentHealth = MaxHealth;
	}
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	return ActualDamage;
}

float ArglkPlayerCharacter::GetHealthPercent() const
{
	return CurrentHealth / MaxHealth;
}
