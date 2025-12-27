#pragma once

#include "CoreMinimal.h"
#include "rglkCharacter.h" // Inheriting from your Base Character
#include "InputActionValue.h"
#include "rglkPlayerCharacter.generated.h"

// Forward declarations to reduce compile time
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;


UCLASS()
class CPP_API ArglkPlayerCharacter : public ArglkCharacter
{
	GENERATED_BODY()

public:
	ArglkPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void Die() override;

public:
	/** Camera Boom: Positions the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	/** Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	/** Actions */
	void Move(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
};

