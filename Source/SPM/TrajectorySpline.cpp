// Fill out your copyright notice in the Description page of Project Settings.


#include "TrajectorySpline.h"

// Sets default values
ATrajectorySpline::ATrajectorySpline()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	RootComponent = SplineComponent;

}

// Called when the game starts or when spawned
void ATrajectorySpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
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

	for (int32 i = 0; i < Points.Num(); i++)
	{
		SplineComponent->AddSplinePoint(Points[i], ESplineCoordinateSpace::World);
		//UE_LOG(LogTemp, Warning, TEXT("Setting trajectory with %d points"), Points.Num());

	}

	SplineComponent->UpdateSpline();

	for (int32 i = 0; i < Points.Num() - 1; ++i)
	{
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
		SplineMesh->SetMobility(EComponentMobility::Movable);
		SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::Z); // eller Y, beroende på din mesh
		SplineMesh->SetStaticMesh(SplineStaticMesh);
		SplineMesh->SetMaterial(0, SplineMaterial);
		
		FVector StartPos = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector EndPos = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);
		FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World);

		
		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
		
		SplineMesh->RegisterComponent();

		SplineMeshes.Add(SplineMesh);
	}
}
