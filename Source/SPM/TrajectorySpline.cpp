// Fill out your copyright notice in the Description page of Project Settings.


#include "TrajectorySpline.h"


ATrajectorySpline::ATrajectorySpline()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	RootComponent = SplineComponent;

}

void ATrajectorySpline::BeginPlay()
{
	Super::BeginPlay();
}


void ATrajectorySpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrajectorySpline::ClearSpline()
{
	for (auto* Mesh : SplineMeshes)
	{
		if (Mesh)
			Mesh->DestroyComponent();
	}
	SplineMeshes.Empty();
	SplineComponent->ClearSplinePoints();
}



void ATrajectorySpline::SetTrajectory(const TArray<FVector>& Points)
{
	ClearSpline();

	for (const FVector& Point : Points)
	{
		SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::World);
	}
	
	SplineComponent->UpdateSpline();

	const float Spacing = 90.0f;
	float Distance = 0.0f;
	const float MaxDistance = SplineComponent->GetSplineLength();

	while (Distance < MaxDistance)
	{
		FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FVector Tangent = SplineComponent->GetTangentAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		Tangent.Normalize();

		//Meshen rotation måste vara i Z 
		FRotator Rotation = FRotationMatrix::MakeFromZ(Tangent).Rotator();

		UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(this);
		MeshComp->SetMobility(EComponentMobility::Movable);
		MeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		
		MeshComp->SetStaticMesh(SplineStaticMesh);
		MeshComp->SetMaterial(0, SplineMaterial);

		MeshComp->SetWorldLocation(Location);
		MeshComp->SetWorldRotation(Rotation);

		//För att bara den karaktär som ska kasta kan se prediction line
		MeshComp->SetOnlyOwnerSee(true);
		MeshComp->bCastHiddenShadow = false;
		MeshComp->SetVisibility(true);
		
		MeshComp->RegisterComponent();

		SplineMeshes.Add(MeshComp);

		Distance += Spacing;
	}
}

