#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ObstacleManager.generated.h"

UCLASS()
class CPP_API AObstacleManager : public AActor
{
	GENERATED_BODY()
    
public:    
	AObstacleManager();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> ObstacleClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* SpawnArea;

private:
	FTimerHandle SpawnTimerHandle;

	void SpawnObstacle();
};