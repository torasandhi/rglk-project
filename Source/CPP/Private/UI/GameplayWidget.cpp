// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameplayWidget.h"
#include "CPP/CPP.h"
#include "Player/rglkPlayerCharacter.h"



void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APawn* OwningPawn = GetOwningPlayerPawn();
	if (ArglkPlayerCharacter* MyChar = Cast<ArglkPlayerCharacter>(OwningPawn))
	{
		BindToPlayer(MyChar);
	}
	else
	{
		PRINT_DEBUG_MESSAGE("GameplayWidget: Could not find ArglkPlayerCharacter!");
	}
}

void UGameplayWidget::BindToPlayer(ArglkPlayerCharacter* Player)
{
	if (!Player) return;

	PRINT_DEBUG_MESSAGE("CALLED");

	Player->OnHealthChanged.AddDynamic(
		this, &UGameplayWidget::OnHealthChanged
	);
	
	OnHealthChanged(Player->GetHealthPercent() * Player->MaxHealth, Player->MaxHealth);
}

void UGameplayWidget::OnHealthChanged(float Current, float Max)
{
	HealthBar->SetPercent(Current / Max);
}

void UGameplayWidget::OnScoreChanged(FString NewScore)
{
	ScoreText->SetText(FText::FromString(NewScore));
}
