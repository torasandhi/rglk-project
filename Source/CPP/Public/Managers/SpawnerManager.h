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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* SpawnArea;

private:
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	TArray<AActor*> SpawnedEnemies;
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> ObstacleClass;
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> EnemyClass;
	UPROPERTY(Editanywhere, Category = "Spawning")
	int32 MaxEnemySpawned;
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval = 2.0f;

	FTimerHandle SpawnTimerHandle;

	void SpawnObstacle() const;
	void SpawnEnemies();
	FVector GetRandomSpawnPointAtEdgePos() const;
	void HandleActorReturnedToPool(AActor* Enemy);
};