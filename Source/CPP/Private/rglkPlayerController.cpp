// Fill out your copyright notice in the Description page of Project Settings.


#include "rglkPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CPP/CPP.h"
#include "Player/rglkPlayerCharacter.h"
#include "UI/MainWidget.h"

void ArglkPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputState(EInputState::Gameplay);

	UMainWidget* UI = CreateWidget<UMainWidget>(this, UI_Main);
	UI->AddToViewport();
	if (UI)
	{
		UI->WBP_Gameplay->SetVisibility(ESlateVisibility::Visible);
	}
}

void ArglkPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ArglkPlayerController::OnMoveAction);
		EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ArglkPlayerController::OnAttackAction);
	}
}

void ArglkPlayerController::SetInputState(EInputState NewState)
{
	if (CurrentState == NewState)
	{
		PRINT_DEBUG_MESSAGE("STATE IS THE SAME");
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!Subsystem)
	{
		PRINT_DEBUG_MESSAGE("SUBSYSTEM NOT FOUND");
		return;
	}

	// Clear previous contexts
	Subsystem->ClearAllMappings();

	switch (NewState)
	{
	case EInputState::Gameplay:
		Subsystem->AddMappingContext(GameplayContext, 0);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
		break;

	case EInputState::UI:
		Subsystem->AddMappingContext(UIContext, 0);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		break;
	}

	CurrentState = NewState;
}

void ArglkPlayerController::OnMoveAction(const FInputActionValue& value)
{
	if (ArglkPlayerCharacter* PC = GetPawn<ArglkPlayerCharacter>())
	{
		PC->Move(value);
	}
}

void ArglkPlayerController::OnAttackAction(const FInputActionValue& value)
{
	if (ArglkPlayerCharacter* PC = GetPawn<ArglkPlayerCharacter>())
	{
		PC->Attack(value);
	}
}
