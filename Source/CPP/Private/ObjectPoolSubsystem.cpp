#include "ObjectPoolSubsystem.h"

#include "UPoolableInterface.h"


void UObjectPoolSubsystem::InitializePoolsFromAsset(UMyPoolConfig* Config)
{
    if (!Config) return;

    // Iterate through every pool defined in the Data Asset
    for (const FPoolDefinition& PoolDef : Config->poolDefinitions)
    {
        if (!PoolDef.actorClass) continue;

        // Create the entry in our Dictionary
        TArray<AActor*>& ActorList = PoolDictionary.FindOrAdd(PoolDef.actorClass);

        // Spawn the actors
        for (int32 i = 0; i < PoolDef.warmupPoolSize; i++)
        {
            AActor* NewActor = GetWorld()->SpawnActor<AActor>(PoolDef.actorClass, FVector::ZeroVector, FRotator::ZeroRotator);
            if (NewActor)
            {
                NewActor->SetActorHiddenInGame(true);
                NewActor->SetActorEnableCollision(false);
                NewActor->SetActorTickEnabled(false);
                ActorList.Add(NewActor);
            }
        }
    }
}

AActor* UObjectPoolSubsystem::GetActorFromPool(TSubclassOf<AActor> ActorClass, FVector Location, FRotator Rotation)
{
    // Find the specific pool for this class
    if (TArray<AActor*>* FoundPool = PoolDictionary.Find(ActorClass))
    {
        if (FoundPool->Num() > 0)
        {
            AActor* PoppedActor = FoundPool->Pop();
            PoppedActor->SetActorLocationAndRotation(Location, Rotation);
            PoppedActor->SetActorHiddenInGame(false);
            PoppedActor->SetActorEnableCollision(true);
            PoppedActor->SetActorTickEnabled(true);
            return PoppedActor;
        }
    }

    // Optional: Auto-expand if empty (Spawn new one)
    return GetWorld()->SpawnActor<AActor>(ActorClass, Location, Rotation);
}

void UObjectPoolSubsystem::ReturnActorToPool(AActor* ActorToReturn)
{
    if (!ActorToReturn) return;

    // Reset Actor
    ActorToReturn->SetActorHiddenInGame(true);
    ActorToReturn->SetActorEnableCollision(false);
    ActorToReturn->SetActorTickEnabled(false);

    // Find which list it belongs to and add it back
    // (Optimization: You can map Actor -> Class type to avoid this lookup if needed)as
    TArray<AActor*>& ActorList = PoolDictionary.FindOrAdd(ActorToReturn->GetClass());
    ActorList.Add(ActorToReturn);
}