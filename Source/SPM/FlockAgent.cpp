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
   
   DynamicNeighbourRadius = NeighbourRadius * FMath::FRandRange(0.9f, 1.1f);
   
}


// Called every frame
void AFlockAgent::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);

   ApplyFlocking(DeltaTime);
   
   FHitResult Hit;
   AddActorWorldOffset(Velocity * DeltaTime, true, &Hit);

   if (Hit.IsValidBlockingHit())
   {
      Velocity = FVector::ZeroVector;
   }

   
   SetActorRotation(Velocity.Rotation());
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
      Avg += Flock->GetVelocity();
      Count++;
   }
   return Count > 0 ? ((Avg / Count).GetSafeNormal() * MaxSpeed - Velocity) : FVector::ZeroVector;
}


FVector AFlockAgent::AvoidObstacles() const
{
   float TraceDistance = Velocity.Size() * 0.5f + 300.f; 
   FVector Start = GetActorLocation();
   FVector Forward = Velocity.GetSafeNormal();

   // Skapa sidovektorer
   FVector Right = FRotationMatrix(Forward.Rotation()).GetUnitAxis(EAxis::Y);
   FVector Left = -Right;

   // Lista med riktningar att testa
   TArray Directions = {
      Forward,                                // rakt fram
      (Forward + Left * 0.5f).GetSafeNormal(),  // snett vänster
      (Forward + Right * 0.5f).GetSafeNormal(), // snett höger
      Left,                                   // rakt vänster (backup)
      Right                                   // rakt höger (backup)
  };

   FCollisionQueryParams Params;
   Params.AddIgnoredActor(this);
   
   float ClosestHitDistance = TraceDistance;
   FVector AvoidForce = FVector::ZeroVector;

   for (const FVector& Dir : Directions)
   {
      FVector End = Start + Dir * TraceDistance;

      FHitResult Hit;
      if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
      {
         float HitDistance = (Hit.ImpactPoint - Start).Size();
         if (HitDistance < ClosestHitDistance)
         {
            ClosestHitDistance = HitDistance;

            FVector Projected = FVector::VectorPlaneProject(Dir, Hit.Normal).GetSafeNormal();
            FVector NormalResponse = Hit.Normal * MaxForce;
            AvoidForce = Projected * MaxForce + NormalResponse;

            // imbormsning
            float BrakeFactor = 1.0f - (HitDistance / TraceDistance);
            AvoidForce -= Velocity * BrakeFactor * 0.5f; // motverka hastighet
         }
      }
   }

   return FVector::ZeroVector; // Inget hinder
}


FVector AFlockAgent::StayInBounds() const
{
   if (!FlockManager)
   {
      return FVector::ZeroVector;
   }

   //FVector ToCenter = FlockManager->SpawnCenter - GetActorLocation();
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

