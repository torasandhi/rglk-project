#include "Managers\SpawnerManager.h"
#include "ObjectPoolSubsystem.h" // Import your pool system
#include "UPoolableInterface.h"
#include "Components/BoxComponent.h"
#include "CPP/CPP.h"

ASpawnerManager::ASpawnerManager()
{
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	SpawnArea->SetBoxExtent(FVector(500.f, 500.f, 10.f)); // Default size
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Just a volume, not a wall
}

void ASpawnerManager::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyClass != nullptr)
	{
		GetWorldTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&ASpawnerManager::SpawnEnemies,
			SpawnInterval,
			true
		);
	}
}


void ASpawnerManager::SpawnObstacle() const
{
	// 1. Safety Check
	if (!ObstacleClass) return;

	// 2. Get Location (Using your new helper!)
	FVector SpawnLocation = GetRandomSpawnPointAtEdgePos();

	// 3. Calculate Rotation (Face Center)
	FVector Origin = SpawnArea->GetComponentLocation();
	FVector DirectionToCenter = Origin - SpawnLocation;
	FRotator SpawnRotation = DirectionToCenter.Rotation();

	// 4. Get from Pool
	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		AActor* SpawnedObstacle = Pool->GetActorFromPool(ObstacleClass, SpawnLocation, SpawnRotation);

		if (SpawnedObstacle)
		{
			// 5. Trigger the Interface (If obstacles use it for Timelines/Resets)
			if (SpawnedObstacle->GetClass()->ImplementsInterface(UPoolableInterface::StaticClass()))
			{
				IPoolableInterface::Execute_OnSpawnFromPool(SpawnedObstacle);
			}
		}
	}
}

void ASpawnerManager::SpawnEnemies() const
{
	if (!EnemyClass) return;

	FVector SpawnLocation = GetRandomSpawnPointAtEdgePos();
	PRINT_DEBUG_MESSAGE(GetRandomSpawnPointAtEdgePos().ToString());
	
	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		// AActor* SpawnedEnemy =
		 Pool->GetActorFromPool(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
	}
}

FVector ASpawnerManager::GetRandomSpawnPointAtEdgePos() const
{
	if (!SpawnArea) return FVector::ZeroVector;

	FVector Origin = SpawnArea->GetComponentLocation();
	FVector Extent = SpawnArea->GetScaledBoxExtent();
	FVector SpawnLocation = Origin;

	// 0=Front, 1=Back, 2=Right, 3=Left
	int32 Side = FMath::RandRange(0, 3);

	switch (Side)
	{
	case 0: // Front (X+)
		SpawnLocation.X += Extent.X;
		SpawnLocation.Y += FMath::RandRange(-Extent.Y, Extent.Y);
		break;
	case 1: // Back (X-)
		SpawnLocation.X -= Extent.X;
		SpawnLocation.Y += FMath::RandRange(-Extent.Y, Extent.Y);
		break;
	case 2: // Right (Y+)
		SpawnLocation.X += FMath::RandRange(-Extent.X, Extent.X);
		SpawnLocation.Y += Extent.Y;
		break;
	case 3: // Left (Y-)
		SpawnLocation.X += FMath::RandRange(-Extent.X, Extent.X);
		SpawnLocation.Y -= Extent.Y;
		break;
	default:
		SpawnLocation.X += Extent.X;
		SpawnLocation.Y += FMath::RandRange(-Extent.Y, Extent.Y);
	}

	return SpawnLocation;
}
