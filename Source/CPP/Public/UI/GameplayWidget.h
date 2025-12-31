// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPP_API UGameplayWidget : public UUserWidget, public IWidgetInterface
{
	GENERATED_BODY()

public :
	void BindToPlayer(class ArglkPlayerCharacter* Player);
	virtual void OnShowWidget_Implementation() override;
	virtual void OnHideWidget_Implementation() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
	
private:
	UFUNCTION()
	void OnHealthChanged(float Current, float Max);
	UFUNCTION()
	void OnScoreChanged(int32 NewScore) const;
	UFUNCTION()
	void PollForPlayerBind();

	UPROPERTY()
	FTimerHandle FindPlayerToBindTimer;
};
