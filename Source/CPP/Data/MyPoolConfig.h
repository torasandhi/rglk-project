// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyPoolConfig.generated.h"

USTRUCT(BlueprintType)
struct FPoolDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> actorClass;
	UPROPERTY(Editanywhere, BlueprintReadOnly)
	int32 warmupPoolSize;
};

UCLASS(BlueprintType)
class CPP_API UMyPoolConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool Configurtion")
	TArray<FPoolDefinition> poolDefinitions;
};
