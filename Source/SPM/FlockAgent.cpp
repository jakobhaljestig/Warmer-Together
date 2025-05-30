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

   //random vikter individuellt för varje boids så flygandet blir lite mer spännande
   PersonalCohesionWeight = FMath::FRandRange(0.8f, 1.2f) * CohesionWeight;
   PersonalSeparationWeight = FMath::FRandRange(0.8f, 1.2f) * SeparationWeight;
   PersonalAlignmentWeight = FMath::FRandRange(0.8f, 1.2f) * AlignmentWeight;
   PersonalMaxSpeed = FMath::FRandRange(0.8f, 1.2f) * MaxSpeed;
   PersonalMaxForce = FMath::FRandRange(0.8f, 1.2f) * MaxForce;
}


// Called when the game starts or when spawned
void AFlockAgent::BeginPlay()
{
   Super::BeginPlay();
   Velocity = FVector::ForwardVector * MaxSpeed;

   //varierande radie för grannar vid start
   DynamicNeighbourRadius = NeighbourRadius * FMath::FRandRange(0.9f, 1.1f);
   
}


// Called every frame
void AFlockAgent::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);

   ApplyFlocking(DeltaTime);
}

//varje boid flyger mot dem andra boidsen, de flyger inte direkt mot varandra, utan styr gradvis mot varandra. 
FVector AFlockAgent::Cohesion()
{
   FVector Center = FVector::ZeroVector;
   int Count = 0;
   for (AFlockAgent* Flock : FlockManager->GetNearbyAgents(this, DynamicNeighbourRadius))
   {
      Center += Flock->GetActorLocation();
      Count++;
   }
   return Count > 0 ? ((Center / Count - GetActorLocation()).GetSafeNormal() * MaxSpeed - Velocity) : FVector::ZeroVector;
}

//boidsen försöker undvika att styra helt in i de andra boidsen, så om de är för nära väjer de undan. 
FVector AFlockAgent::Separation()
{
   FVector Steer = FVector::ZeroVector;
   int Count = 0;
   for (AFlockAgent* Flock : FlockManager->GetNearbyAgents(this, DynamicNeighbourRadius * 0.5f))
   {
      FVector Diff = GetActorLocation() - Flock->GetActorLocation();
      //ju närmare desto mer seperation
     Steer += Diff /FMath::Max(Diff.SizeSquared(), 1.0f);;
      Count++;
   }
   return Count > 0 ? Steer.GetSafeNormal() * MaxSpeed - Velocity : FVector::ZeroVector;
}

//sista steget är att de försöker matcha de andra i flockens velocity. 
FVector AFlockAgent::Alignment()
{
   FVector Avg = FVector::ZeroVector;
   int Count = 0;
   for (AFlockAgent* Flock : FlockManager->GetNearbyAgents(this, DynamicNeighbourRadius))
   {
      //matchar flockens velocity
      Avg += Flock->GetVelocity();
      Count++;
   }
   return Count > 0 ? ((Avg / Count).GetSafeNormal() * MaxSpeed - Velocity) : FVector::ZeroVector;
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



FVector AFlockAgent::JoinLargerFlockForce()
{
   TArray<AFlockAgent*> Neighbours = FlockManager->GetNearbyAgents(this, DynamicNeighbourRadius);
   int32 MyCount = Neighbours.Num();

   FVector AvgPos = FVector::ZeroVector;
   int32 LargerGroupCount = 0;

   for (AFlockAgent* Other : Neighbours)
   {
      if (!Other || Other == this) continue;

      int32 OtherCount = FlockManager->GetNearbyAgents(this, DynamicNeighbourRadius).Num();
      if (OtherCount > MyCount + 2)
      {
         AvgPos += Other->GetActorLocation();
         LargerGroupCount++;
      }
   }

   if (LargerGroupCount > 0)
   {
      FVector Target = AvgPos / LargerGroupCount;
      FVector Desired = (Target - GetActorLocation()).GetSafeNormal() * MaxSpeed;
      return Desired - Velocity;
   }

   return FVector::ZeroVector;
}



FVector AFlockAgent::StayInBounds() const
{
   if (!FlockManager) return FVector::ZeroVector;

   FVector HalfBounds = FlockManager->SpawnBounds;
   FVector Center = FlockManager->SpawnCenter;
   FVector Min = Center - HalfBounds;
   FVector Max = Center + HalfBounds;

   FVector Correction = FVector::ZeroVector;
   FVector Pos = GetActorLocation();

   if (Pos.X < Min.X) Correction.X = 1;
   else if (Pos.X > Max.X) Correction.X = -1;

   if (Pos.Y < Min.Y) Correction.Y = 1;
   else if (Pos.Y > Max.Y) Correction.Y = -1;

   if (Pos.Z < Min.Z) Correction.Z = 1;
   else if (Pos.Z > Max.Z) Correction.Z = -1;

   return Correction.GetSafeNormal() * MaxSpeed - Velocity;
}

//mjukare rörelser för mer randomness i rörelsen 
FVector AFlockAgent::GetDynamicWander(float Time) const
{
   float Angle = FMath::PerlinNoise1D(Time + UniqueOffset) * 360.f;
   FRotator WanderRot(0, Angle, 0);
   FVector WanderDir = WanderRot.Vector();
   return WanderDir * PersonalMaxSpeed * 0.3f;
}

void AFlockAgent::ApplyFlocking(float DeltaTime)
{
   if (!FlockManager) return;

   float Time = GetWorld()->GetTimeSeconds();
   float Oscillation = FMath::Sin(Time + UniqueOffset) * 0.5f + 1.0f;

   float SpeedOsc = BaseSpeed * Oscillation;
   float ForceOsc = BaseForce * Oscillation;
   
   // Flockbeteenden
   FVector BoundsForce = StayInBounds();
   FVector Cohese = Cohesion() * PersonalCohesionWeight;
   FVector Separate = Separation() * PersonalSeparationWeight;
   FVector Align = Alignment() * PersonalAlignmentWeight;
   FVector Wander = GetDynamicWander(Time);
   FVector Avoid = AvoidObstacles();

   FVector Accel = 
       Cohese +
       Separate +
       Align +
       Wander * RandomWeight +
       BoundsForce * 2.0f +
       Avoid;

   Accel = Accel.GetClampedToMaxSize(ForceOsc);
   Velocity += Accel * DeltaTime;
   Velocity = Velocity.GetClampedToMaxSize(SpeedOsc);

   // Flytta och rotera
   AddActorWorldOffset(Velocity * DeltaTime, true);
   SetActorRotation(Velocity.Rotation());
}

