// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"
#include "rglkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/MainWidget.h"

void UMainMenuWidget::OnShowWidget_Implementation()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::OnHideWidget_Implementation()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::OnStartButtonClick()
{

	// Load Gameplay
	UGameplayStatics::LoadStreamLevel(
		this,
		FName("L_Gameplay"),
		true,
		true,
		FLatentActionInfo()
	);

	// Unload Main Menu
	UGameplayStatics::UnloadStreamLevel(
		this,
		FName("L_MainMenu"),
		FLatentActionInfo(),
		true
	);

	
	// UI
	auto PC = GetWorld()->GetFirstPlayerController<ArglkPlayerController>();
	
	// Hide MainMenu
	if (PC->MainWidget->WBP_MainMenu &&
		PC->MainWidget->WBP_MainMenu->Implements<UWidgetInterface>())
	{
		Execute_OnHideWidget(
			PC->MainWidget->WBP_MainMenu
		);
	}

	// Show Gameplay
	if (PC->MainWidget->WBP_Gameplay &&
		PC->MainWidget->WBP_Gameplay->Implements<UWidgetInterface>())
	{
		Execute_OnShowWidget(
			PC->MainWidget->WBP_Gameplay
		);
	}

	GetWorld()->GetFirstPlayerController<ArglkPlayerController>()->SetInputState(EInputState::Gameplay);
}


void UMainMenuWidget::OnExitButtonClick()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		GetWorld()->GetFirstPlayerController(),
		EQuitPreference::Quit,
		false
	);
}
