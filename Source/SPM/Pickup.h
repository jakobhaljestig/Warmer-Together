// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Pickup.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )


//Todo: Create a Superclass to handle grabbing for both Pickup and Push
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
	//Todo: Move to superclass
	UPROPERTY(EditAnywhere)
	float GrabDistance = 200.f;

	//Todo: Move to superclass
	UPROPERTY(EditAnywhere)
	float GrabRadius = 50.f;

	//Todo: Move to superclass
	UPROPERTY(VisibleAnywhere)
	bool Holding = false;
	
	UPROPERTY(VisibleAnywhere)
	bool Lifting = false;

	//Todo: Should belong to the Superclass, which should create its own PhysicsHandle. This 
	UPhysicsHandleComponent* GetPhysicsHandle() const;

	//Todo: Move to superclass
	bool GetGrabbableInReach(FHitResult& OutHitResult) const;

	//Todo: Move to superclass
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	//Todo: Make Separate Component For Grab That Pickup And Push Can Use Instead
	void PhysicsGrab(FHitResult HitResult);

	//Todo: Move to superclass, should be editable to determine which tracechannel the subclasses want to use
	ECollisionChannel TraceChannel;
	
private:
	UPROPERTY(EditAnywhere)
	float ThrowingForce = 750.f;

	UPROPERTY(EditAnywhere)
	float DroppingForce = 350.f;

	

	
};


