// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPP_API UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	void BindToPlayer(class ArglkPlayerCharacter* Player);
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
	
private:
	UFUNCTION()
	void OnHealthChanged(float Current, float Max);
	UFUNCTION()
	void OnScoreChanged(FString NewScore);
};
