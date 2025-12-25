#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnerManager.generated.h"

UCLASS()
class CPP_API ASpawnerManager : public AActor
{
	GENERATED_BODY()
    
public:    
	ASpawnerManager();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> ObstacleClass;
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* SpawnArea;

private:
	FTimerHandle SpawnTimerHandle;

	void SpawnObstacle() const;
	void SpawnEnemies() const;
	FVector GetRandomSpawnPointAtEdgePos() const;
};