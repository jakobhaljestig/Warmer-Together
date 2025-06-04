// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "TrajectorySpline.generated.h"

UCLASS()
class SPM_API ATrajectorySpline : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrajectorySpline();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* SplineComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Trajectory")
	UStaticMesh* SplineStaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Trajectory")
	UMaterialInterface* SplineMaterial;
	
	void SetTrajectory(const TArray<FVector>& Points);

	void ClearSpline();

protected:
	virtual void BeginPlay() override;

private:
	TArray<UStaticMeshComponent*> SplineMeshes;

};
