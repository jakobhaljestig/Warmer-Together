// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Pickup.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UPickup : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickup();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void GrabAndRelease();

	UFUNCTION(BlueprintCallable)
	void Throw();

	virtual void Grab();
	
	virtual void Drop(float Force);

protected:
	UPROPERTY(EditAnywhere)
	float GrabDistance = 200.f;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 50.f;

	
	
	UPROPERTY(EditAnywhere)
	bool Holding = false;

	bool Lifting = false;

	UPhysicsHandleComponent* GetPhysicsHandle() const;

	bool GetGrabbableInReach(FHitResult& OutHitResult) const;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	void PhysicsGrab(FHitResult HitResult);

	ECollisionChannel TraceChannel;
	
private:
	UPROPERTY(EditAnywhere)
	float ThrowingForce = 750.f;

	UPROPERTY(EditAnywhere)
	float DroppingForce = 350.f;

	

	
};


