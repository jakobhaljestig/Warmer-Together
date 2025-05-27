
#include "FlockManager.h"
#include "FlockAgent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFlockManager::AFlockManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}


// Called when the game starts or when spawned
void AFlockManager::BeginPlay()
{
	Super::BeginPlay();


	for (int i = 0; i < NumAgents; ++i)
	{
		FVector RandLoc = GetActorLocation() + UKismetMathLibrary::RandomPointInBoundingBox(FVector::ZeroVector, SpawnBounds);
		AFlockAgent* Agent = GetWorld()->SpawnActor<AFlockAgent>(AgentClass, RandLoc, FRotator::ZeroRotator);
		if (Agent)
		{
			Agent->Initialize(this);
			Agents.Add(Agent);
		}
	}
  
}


// Called every frame
void AFlockManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


TArray<AFlockAgent*> AFlockManager::GetNearbyAgents(AFlockAgent* Agent, float Radius) const
{
	TArray<AFlockAgent*> Neighbours;
	FVector AgentLoc = Agent->GetActorLocation();


	for (AFlockAgent* Other : Agents)
	{
		if (Other != Agent && FVector::DistSquared(AgentLoc, Other->GetActorLocation()) <= Radius * Radius)
		{
			Neighbours.Add(Other);
		}
	}
	return Neighbours;
}
