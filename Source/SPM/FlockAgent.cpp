// Fill out your copyright notice in the Description page of Project Settings.
#include "FlockAgent.h"
#include "FlockManager.h"

// Sets default values
AFlockAgent::AFlockAgent(): FlockManager(nullptr), Velocity()
{
   // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = true;
  
   RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
   Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
   Mesh->SetupAttachment(RootComponent);
}


void AFlockAgent::Initialize(AFlockManager* Manager)
{
   FlockManager = Manager;
}


// Called when the game starts or when spawned
void AFlockAgent::BeginPlay()
{
   Super::BeginPlay();
   Velocity = FVector::ForwardVector * MaxSpeed;
  
}


// Called every frame
void AFlockAgent::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);

   ApplyFlocking(DeltaTime);

   // Uppdatera position och rotation
   FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;
   SetActorLocation(NewLocation);

   // Justera rotationen i rörelseriktningen
   if (!Velocity.IsNearlyZero())
   {
      FRotator NewRotation = Velocity.Rotation();
      SetActorRotation(NewRotation);
   }


}


FVector AFlockAgent::Alignement()
{
   FVector Avg = FVector::ZeroVector;
   int Count = 0;
   for (auto* Other : FlockManager->GetNearbyAgents(this, NeighbourRadius))
   {
      Avg += Other->GetVelocity();
      Count++;
   }
   return Count > 0 ? (Avg / Count).GetSafeNormal() * MaxSpeed - Velocity : FVector::ZeroVector;
}


FVector AFlockAgent::Cohesion()
{
   FVector Center = FVector::ZeroVector;
   int Count = 0;
   for (auto* Other : FlockManager->GetNearbyAgents(this, NeighbourRadius))
   {
      Center += Other->GetActorLocation();
      Count++;
   }
   return Count > 0 ? ((Center / Count) - GetActorLocation()).GetSafeNormal() * MaxSpeed - Velocity : FVector::ZeroVector;
}


FVector AFlockAgent::Separation()
{
   FVector Steer = FVector::ZeroVector;
   int Count = 0;
   for (auto* Other : FlockManager->GetNearbyAgents(this, NeighbourRadius * 0.5f))
   {
      FVector Diff = GetActorLocation() - Other->GetActorLocation();
      Steer += Diff / FMath::Max(Diff.SizeSquared(), 1.0f); // Undvik delning med 0
      Count++;
   }
   return Count > 0 ? Steer.GetSafeNormal() * MaxSpeed - Velocity : FVector::ZeroVector;
}


void AFlockAgent::ApplyFlocking(float DeltaTime)
{
   auto Align = Alignement();
   auto Cohese = Cohesion();
   auto Separate = Separation();


   FVector Accel = Align + Cohese + Separate;
   Accel = Accel.GetClampedToMaxSize(MaxForce);


   Velocity += Accel * DeltaTime;
   Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
}
