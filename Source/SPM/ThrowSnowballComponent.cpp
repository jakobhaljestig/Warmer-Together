// Fill out your copyright notice in the Description page of Project Settings.
#include "ThrowSnowballComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CharacterBase.h"

UThrowSnowballComponent::UThrowSnowballComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UThrowSnowballComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UThrowSnowballComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsAiming)
	{
		PredictThrowTrajectory();
	}
}



void UThrowSnowballComponent::Aim()
{
	bIsAiming = true;
}


void UThrowSnowballComponent::Throw()
{
	if (!SnowballClass || !bCanThrow || !bIsThrowAreaValid)
	{
		bIsAiming = false;
		//Pathen måste försvinna om man inte får kasta också
		
		return;
	}

	bIsThrowing = true;

	//KOM IHÅG despawna pathen när man väl kastar
	/*if path då ta bort*/
	ClearTrajectoryPath();

	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	
	FVector CameraLocation;
	FRotator CameraRotation;
	CharacterOwner->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
	
	FActorSpawnParameters SpawnParams;
	//Ignorerar spelaren som kastar
	SpawnParams.Owner = GetOwner();

	ASnowball* Snowball = GetWorld()->SpawnActor<ASnowball>(SnowballClass, LastAimLocation, LastAimRotation, SpawnParams);

	if (Snowball)
	{
		FVector ThrowDirection = LastThrowDirection;
		ThrowDirection.Normalize();
		Snowball->ThrowInDirection(ThrowDirection, Speed);

		bCanThrow = false;
		bIsAiming = false;
		bIsThrowing = false;
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetThrow, this, &UThrowSnowballComponent::ResetThrow, SnowballInterval, false);
	}
}


void UThrowSnowballComponent::ResetThrow()
{
	bCanThrow = true;
}


void UThrowSnowballComponent::PredictThrowTrajectory()
{
	if (!SnowballClass || !bCanThrow || !bIsAiming )
	{
		return;
	}

	ClearTrajectoryPath();
	
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	
	FVector CameraLocation;
	FRotator CameraRotation;
	CharacterOwner->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
	FVector CameraForward = CameraRotation.Vector();

	FVector CharacterForward = CharacterOwner->GetActorForwardVector();

	float AimAngleDegrees = FMath::RadiansToDegrees(acosf(FVector::DotProduct(CameraForward.GetSafeNormal(), CharacterForward.GetSafeNormal())));

	if (AimAngleDegrees > 45)
	{
		UE_LOG(LogTemp, Warning, TEXT("Kastar inte – kameran tittar för långt från karaktärens riktning (vinkel: %.1f°)"), AimAngleDegrees);
		ClearTrajectoryPath();
		bIsThrowAreaValid = false;
		return;
	}

	//TILL KASTET I THROW
	LastAimLocation = CharacterOwner -> GetMesh()->GetSocketLocation(HandSocketName);
	LastAimRotation = (CameraRotation.Vector()).Rotation();
	LastThrowDirection = CameraRotation.Vector();
	
	FPredictProjectilePathParams PredictParams;
	PredictParams.StartLocation = LastAimLocation;
	PredictParams.LaunchVelocity = LastThrowDirection * Speed;
	PredictParams.MaxSimTime = 2.0f;
	//FÖR DEBUG
	//PredictParams.DrawDebugType = EDrawDebugTrace::ForDuration; 
	//PredictParams.DrawDebugTime = 0.1f;
	PredictParams.ActorsToIgnore.Add(CharacterOwner);
	PredictParams.bTraceWithCollision = true;
	PredictParams.bTraceComplex = false;
	
	FPredictProjectilePathResult PredictResult;
	
	bool bHit = UGameplayStatics::PredictProjectilePath(CharacterOwner, PredictParams, PredictResult);

	if (bHit)
	{
		if (TrajectoryMeshClass)
		{
			for (const FPredictProjectilePathPointData& Point : PredictResult.PathData)
			{
				AActor* MeshPoint = GetWorld()->SpawnActor<AActor>(TrajectoryMeshClass, Point.Location, FRotator::ZeroRotator);
				TrajectoryMeshes.Add(MeshPoint);
			}
			bIsThrowAreaValid = true;
		}
	}
}

void UThrowSnowballComponent::ClearTrajectoryPath()
{
	for (AActor* Mesh : TrajectoryMeshes)
	{
		if (Mesh)
		{
			Mesh->Destroy();
		}
	}
	TrajectoryMeshes.Empty();
}
