#include "Managers\ObstacleManager.h"
#include "ObjectPoolSubsystem.h" // Import your pool system
#include "UPoolableInterface.h"
#include "Components/BoxComponent.h"

AObstacleManager::AObstacleManager()
{
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	SpawnArea->SetBoxExtent(FVector(500.f, 500.f, 10.f)); // Default size
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Just a volume, not a wall
}

void AObstacleManager::BeginPlay()
{
	Super::BeginPlay();

	if (ObstacleClass != nullptr)
	{
		GetWorldTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&AObstacleManager::SpawnObstacle,
			SpawnInterval,
			true // Loop = true
		);
	}
}

void AObstacleManager::SpawnObstacle()
{
	// 1. Get Box Data
	FVector Origin = SpawnArea->GetComponentLocation();
	FVector Extent = SpawnArea->GetScaledBoxExtent(); // Extent is Half-Size (Radius)

	FVector SpawnLocation = Origin;

	// 2. Pick a random side (0=Front, 1=Back, 2=Right, 3=Left)
	// We assume Z (Height) is constant, so we only randomize X and Y.
	int32 Side = FMath::RandRange(0, 3);

	switch (Side)
	{
	case 0: // Front Edge (X+)
		SpawnLocation.X += Extent.X;
		SpawnLocation.Y += FMath::RandRange(-Extent.Y, Extent.Y); // Random spot along the width
		break;

	case 1: // Back Edge (X-)
		SpawnLocation.X -= Extent.X;
		SpawnLocation.Y += FMath::RandRange(-Extent.Y, Extent.Y);
		break;

	case 2: // Right Edge (Y+)
		SpawnLocation.X += FMath::RandRange(-Extent.X, Extent.X); // Random spot along the length
		SpawnLocation.Y += Extent.Y;
		break;

	case 3: // Left Edge (Y-)
		SpawnLocation.X += FMath::RandRange(-Extent.X, Extent.X);
		SpawnLocation.Y -= Extent.Y;
		break;
	}
	
	FVector DirectionToCenter = Origin - SpawnLocation;
	FRotator SpawnRotation = DirectionToCenter.Rotation();

	UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();

	if (Pool)
	{
		AActor* spawnedObstacle = Pool->GetActorFromPool(ObstacleClass, SpawnLocation, SpawnRotation);
		if (spawnedObstacle->GetClass()->ImplementsInterface(UPoolableInterface::StaticClass())) return;
		{
			IPoolableInterface::Execute_OnSpawnFromPool(spawnedObstacle);
		}
	}
}
