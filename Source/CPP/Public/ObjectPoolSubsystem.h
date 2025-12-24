#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CPP/Data/MyPoolConfig.h" // Include your new config header
#include "ObjectPoolSubsystem.generated.h"

UCLASS()
class CPP_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Initialize EVERYTHING from the Data Asset
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void InitializePoolsFromAsset(UMyPoolConfig* Config);

	// Get an actor by its CLASS type
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetActorFromPool(TSubclassOf<AActor> ActorClass, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ReturnActorToPool(AActor* ActorToReturn);
	
private:
	// Key = The Class Type, Value = The Array of pooled actors
	TMap<UClass*, TArray<AActor*>> PoolDictionary;
};