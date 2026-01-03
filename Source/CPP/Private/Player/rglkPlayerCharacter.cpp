#include "Player/rglkPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "ObjectPoolSubsystem.h"
#include "NiagaraComponent.h"
#include "Components/RangedWeaponComponent.h"
#include "CPP/CPP.h"

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

	AimingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("AimingComponent"));
	AimingComponent->SetupAttachment(RootComponent);
	AimingComponent->SetUsingAbsoluteRotation(true);

	FirePointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComponent"));
	FirePointComponent->SetupAttachment(AimingComponent);
	FirePointComponent->SetRelativeLocation(FVector(67.67f, 0.0f, 0.0f));

	RangedComp = CreateDefaultSubobject<URangedWeaponComponent>(TEXT("RangedWeaponComponent"));

	SlashEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SlashEffect"));
	SlashEffect->SetupAttachment(FirePointComponent);
	SlashEffect->bAutoActivate = false;
}

void ArglkPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	ApplyBaseStats(TEXT("Player"));
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void ArglkPlayerCharacter::Attack()
{
	switch (AttackType)
	{
	default:

		break;

	case EAttackType::Melee:
		if (!WeaponComp) return;
		if (SlashEffect && !SlashEffect->IsActive())
		{
			WeaponComp->PerformAttack();
		}
		break;
	case EAttackType::Ranged:
		if (!RangedComp) return;
		RangedComp->PerformAttack();
		break;
	}
}

void ArglkPlayerCharacter::Die()
{
	Super::Die();
}


void ArglkPlayerCharacter::Execute_Move(const FInputActionValue& Value)
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

void ArglkPlayerCharacter::Execute_AimStick(const FInputActionValue& Value)
{
	const FVector2D Aim = Value.Get<FVector2D>();
	if (Aim.IsNearlyZero()) return;

	const float Angle = FMath::Atan2(Aim.Y, Aim.X);
	const FRotator Rot(0.f, FMath::RadiansToDegrees(Angle), 0.f);

	AimingComponent->SetRelativeRotation(Rot);
}

void ArglkPlayerCharacter::Execute_AimAtMousePos(const FVector& WorldPos)
{
	const FVector Dir = (WorldPos - AimingComponent->GetComponentLocation()).GetSafeNormal();
	const FRotator Rot = Dir.Rotation();

	AimingComponent->SetWorldRotation(FRotator(0.f, Rot.Yaw, 0.f));
}


void ArglkPlayerCharacter::Execute_Attack(const FInputActionValue& Value)
{
	if (!WeaponComp) return;;

	Attack();
}

void ArglkPlayerCharacter::Execute_Swap(const FInputActionValue& Value)
{
	if (TypeCount > 0)
	{
		CurrentTypeIndex++;

		switch (CurrentTypeIndex)
		{
		default:
			AttackType = EAttackType::Melee;
			PRINT_DEBUG_MESSAGE("MELEE");
			break;
		case 1:
			AttackType = EAttackType::Melee;
			PRINT_DEBUG_MESSAGE("MELEE");
			break;
		case 2:
			AttackType = EAttackType::Ranged;
			PRINT_DEBUG_MESSAGE("RANGED");
			break;
		}

		if (CurrentTypeIndex > TypeCount || CurrentTypeIndex == 0)
		{
			CurrentTypeIndex = 1;
		}
	}
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
		ApplyBaseStats();
	}
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	return ActualDamage;
}

float ArglkPlayerCharacter::GetHealthPercent() const
{
	return CurrentHealth / MaxHealth;
}
