#include "UI/GameplayWidget.h"
#include "CPP/CPP.h"
#include "Player/rglkPlayerCharacter.h"
#include "Subsystem/Instance/ScoreSubsystem.h"
#include "TimerManager.h"

void UGameplayWidget::OnShowWidget_Implementation()
{
	SetVisibility(ESlateVisibility::Visible);

	APawn* OwningPawn = GetOwningPlayerPawn();

	if (ArglkPlayerCharacter* MyChar = Cast<ArglkPlayerCharacter>(OwningPawn))
	{
		BindToPlayer(MyChar);
	}
	else
	{
		PRINT_DEBUG_MESSAGE("GameplayWidget: Player not ready, starting search loop...");

		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				FindPlayerToBindTimer,
				this,
				&UGameplayWidget::PollForPlayerBind,
				0.1f,
				true
			);
		}
	}
}

void UGameplayWidget::OnHideWidget_Implementation()
{
	SetVisibility(ESlateVisibility::Collapsed);

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UScoreSubsystem* SS = GI->GetSubsystem<UScoreSubsystem>())
		{
			SS->OnScoreChanged.RemoveAll(this);
		}
	}
}

void UGameplayWidget::PollForPlayerBind()
{
	APawn* OwningPawn = GetOwningPlayerPawn();

	if (OwningPawn)
	{
		ArglkPlayerCharacter* PlayerChar = static_cast<ArglkPlayerCharacter*>(OwningPawn);
		if (PlayerChar)
		{
			BindToPlayer(PlayerChar);

			// Found him. Stop the loop.
			if (GetWorld())
			{
				GetWorld()->GetTimerManager().ClearTimer(FindPlayerToBindTimer);
			}
		}
	}
}

void UGameplayWidget::OnHealthChanged(float Current, float Max)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Current / Max);
	}
}

void UGameplayWidget::OnScoreChanged(int32 NewScore) const
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(NewScore));
	}
}

void UGameplayWidget::BindToPlayer(ArglkPlayerCharacter* Player)
{
	if (!Player) return;

	Player->OnHealthChanged.RemoveDynamic(this, &UGameplayWidget::OnHealthChanged);
	Player->OnHealthChanged.AddDynamic(this, &UGameplayWidget::OnHealthChanged);

	if (Player->MaxHealth > 0)
	{
		OnHealthChanged(Player->GetHealthPercent() * Player->MaxHealth, Player->MaxHealth);
	}

	if (UScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UScoreSubsystem>())
	{
		ScoreSubsystem->OnScoreChanged.RemoveAll(this); // Safety clear
		ScoreSubsystem->OnScoreChanged.AddUObject(this, &UGameplayWidget::OnScoreChanged);
	}
}
