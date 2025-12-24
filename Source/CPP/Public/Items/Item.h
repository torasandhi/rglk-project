// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Item.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class CPP_API AItem : public APawn
{
	GENERATED_BODY()

public:
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> CubeClass;
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<AActor*> spawnedCubes;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Controls;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Move;
	
	float forwardValue;
	float rightValue;
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void InitializePlayerControls() const;
	void SpawnCube();
	void DestroyAllCubes();
	void Move(const FInputActionValue& Value);
};
