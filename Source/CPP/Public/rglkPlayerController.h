// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "rglkPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UENUM(BlueprintType)
enum class EInputState : uint8
{
	Gameplay,
	UI
};

UCLASS()
class CPP_API ArglkPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void SetInputState(EInputState NewState);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* GameplayContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* UIContext;

	UPROPERTY(EditDefaultsOnly, Category="Actions")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Actions")
	UInputAction* AttackAction;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> UI_Main;

private:
	EInputState CurrentState = EInputState::UI;

	/**OnActions*/
	void OnAttackAction(const FInputActionValue& value);
	void OnMoveAction(const FInputActionValue& value);
};
