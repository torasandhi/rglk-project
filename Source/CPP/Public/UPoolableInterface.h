// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UPoolableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPoolableInterface : public UInterface
{
	GENERATED_BODY()
};

class CPP_API IPoolableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
	void OnSpawnFromPool();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnReturnedToPool, AActor*);
	virtual FOnReturnedToPool& OnReturnedToPool() = 0;
};
