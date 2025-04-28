// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();
	virtual ~UGrabComponent() override;


	//Toggle's between Grab and Release
	UFUNCTION(BlueprintCallable)
	virtual void GrabAndRelease();

	//Grab object
	virtual void Grab();

	//Release object
	virtual void Release();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool GetGrabbableInReach(FHitResult& OutHitResult) const;

	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Grab")
	float GrabDistance = 200.f;
	
	UPROPERTY(EditAnywhere, Category = "Grab")
	float GrabRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = "Grab")
	float HoldDistance = 200.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Grab")
	bool Holding = false;

	//Looks through all instances of GrabComponent to see if one is holding something
	bool HoldingSomething() const;

	ECollisionChannel CollisionChannel;

	UPROPERTY(VisibleAnywhere)
	UCharacterMovementComponent* OwnerMovementComponent;

	//Used to trigger different effects upon grabbing something
	virtual void GrabEffect();

	//Used to trigger effects upon releasing something
	virtual void ReleaseEffect();

private:
	//Gets PhysicsHandleComponent
	UPhysicsHandleComponent* GetPhysicsHandle() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
