// Fill out your copyright notice in the Description page of Project Settings.
#include "FlockAgent.h"
#include "FlockManager.h"

// Sets default values
AFlockAgent::AFlockAgent(): FlockManager(nullptr), Velocity()
{
   // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = true;

   RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
   SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
   SkeletalMesh -> SetupAttachment(RootComponent);

   SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
   SkeletalMesh->SetCollisionObjectType(ECC_Pawn);
   SkeletalMesh->SetCollisionResponseToAllChannels(ECR_Block);
   
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
   SetActorLocation(NewLocation, true);

   // Justera rotationen i rörelseriktningen
   if (!Velocity.IsNearlyZero())
   {
      FRotator NewRotation = Velocity.Rotation();
      SetActorRotation(NewRotation);
   }
}

//varje boid flyger mot dem andra boidsen, de flyger inte direkt mot varandra, utan styr gradvis mot varandra. 
FVector AFlockAgent::Cohesion()
{
   FVector Center = FVector::ZeroVector;
   int Count = 0;
   for (AFlockAgent* Flock : FlockManager->GetNearbyAgents(this, NeighbourRadius))
   {
      Center += Flock->GetActorLocation();
      Count++;
   }
   return Count > 0 ? ((Center / Count) - GetActorLocation()).GetSafeNormal() * MaxSpeed - Velocity : FVector::ZeroVector;
}

//boidsen försöker undvika att styra helt in i de andra boidsen, så om de är för nära väjer de undan. 
FVector AFlockAgent::Separation()
{
   FVector Steer = FVector::ZeroVector;
   int Count = 0;
   for (AFlockAgent* Flock : FlockManager->GetNearbyAgents(this, NeighbourRadius))
   {
      FVector Diff = GetActorLocation() - Flock->GetActorLocation();
     Steer += Diff.GetSafeNormal() /FMath::Max(Diff.Size(), 1.0f);;
      Count++;
   }
   return Count > 0 ? Steer.GetSafeNormal() * MaxSpeed - Velocity : FVector::ZeroVector;
}

FVector AFlockAgent::AvoidObstacles() const
{
   FHitResult Hit;
   FVector Start = GetActorLocation();
   FVector End = Start + Velocity.GetSafeNormal() * 200.f;

   FCollisionQueryParams Params;
   Params.AddIgnoredActor(this);

   if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
   {
      return Hit.Normal * MaxForce; //knuffa bort från objekt
   }

   return FVector::ZeroVector;
}

FVector AFlockAgent::StayInBounds() const
{
   if (!FlockManager)
   {
      return FVector::ZeroVector;
   }

   FVector ToCenter = FlockManager->SpawnCenter - GetActorLocation();
   FVector HalfBounds = FlockManager->SpawnBounds;

   //om utanför bounds styr tillbaka
   FVector Correction = FVector::ZeroVector;

   FVector Pos = GetActorLocation();
   FVector Min = FlockManager->SpawnBounds - HalfBounds;
   FVector Max = FlockManager->SpawnBounds + HalfBounds;

   if (Pos.X < Min.X)
   {
      Correction.X = 1;
   }
   else if (Pos.X > Max.X)
   {
      Correction.X = -1;
   }
   
   if (Pos.Y < Min.Y)
   {
      Correction.Y = 1;
   }
   else if (Pos.Y > Max.Y)
   {
      Correction.Y = -1;
   }

   if (Pos.Z < Min.Z)
   {
      Correction.Z = 1;
   }
   else if (Pos.Z > Max.Z)
   {
      Correction.Z = -1;
   }

   

   return Correction.GetSafeNormal() * MaxSpeed - Velocity;
}

//sista steget är att de försöker matcha de andra i flockens velocity. 
FVector AFlockAgent::Alignment()
{
   FVector Avg = FVector::ZeroVector;
   int Count = 0;
   for (AFlockAgent* Flock : FlockManager->GetNearbyAgents(this, NeighbourRadius))
   {
      Avg += Flock->GetVelocity();
      Count++;
   }
   return Count > 0 ? (Avg / Count).GetSafeNormal() * MaxSpeed - Velocity : FVector::ZeroVector;
}

void AFlockAgent::ApplyFlocking(float DeltaTime)
{
  FVector BoundsForce = StayInBounds();
   
   FVector Cohese = Cohesion();
   FVector Separate = Separation();
   FVector Align = Alignment();
   
   FVector Avoid = AvoidObstacles();

   FVector Accel =  Cohese + Separate + Align + Avoid + BoundsForce * 2.0f;
   Accel = Accel.GetClampedToMaxSize(MaxForce);
   
   Velocity += Accel * DeltaTime;
   Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);

   FVector Center = FlockManager->GetActorLocation();
   FVector OffsetToCenter = Center - GetActorLocation();

   float DistanceFromCenter = OffsetToCenter.Size();
   float MaxDistance = FlockManager->SpawnBounds.GetMax();

   if (DistanceFromCenter > MaxDistance)
   {
      FVector Correction = OffsetToCenter.GetSafeNormal() * MaxForce;
      Accel += Correction;
   }
}
