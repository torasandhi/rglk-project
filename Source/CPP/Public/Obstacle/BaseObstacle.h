// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPoolableInterface.h"
#include "GameFramework/Actor.h"
#include "BaseObstacle.generated.h"

UCLASS()
class CPP_API ABaseObstacle : public AActor, public IPoolableInterface
{
	GENERATED_BODY()

public:
	ABaseObstacle();

	virtual void Tick(float DeltaTime) override;
	virtual void OnSpawnFromPool_Implementation() override;
	virtual FOnReturnedToPool& OnReturnedToPool() override;

protected:
	FTimerHandle ReturnToPoolTImer;
	
	/** Called when the game starts or when spawned*/
	virtual void BeginPlay() override;
	void ReturnSelfToPool();
		
private:
	UPROPERTY(EditAnywhere, Category = "Obstacle")
	float moveSpeed = 10.f;
	float LifeTime = 5.f;

	FOnReturnedToPool ReturnToPool;
};
