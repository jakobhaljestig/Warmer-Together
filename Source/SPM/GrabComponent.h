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

	//Toggle's between Grab and Release
	UFUNCTION(BlueprintCallable)
	virtual void GrabAndRelease();

	//Grab object
	UFUNCTION()
	virtual void Grab();

	//Release object
	UFUNCTION()
	virtual void Release();

	//Looks through all instances of GrabComponent to see if one is holding something
	UFUNCTION()
	bool HoldingSomething() const;
	
protected:
	// Called when the game starts
	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	bool GetGrabbableInReach(FHitResult& OutHitResult) const;
	
	bool GetGrabbableInReach(FHitResult& OutHitResult, float Distance) const;

	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Grab")
	float GrabDistance = 200.f;
	
	UPROPERTY(EditAnywhere, Category = "Grab")
	float GrabRadius = 50.f;

	ECollisionChannel CollisionChannel;

	UPROPERTY(VisibleAnywhere)
	UCharacterMovementComponent* OwnerMovementComponent;

	//Used to trigger different effects upon grabbing something
	UFUNCTION()
	virtual void GrabEffect();

	//Used to trigger effects upon releasing something
	UFUNCTION()
	virtual void ReleaseEffect();

	UPROPERTY(VisibleAnywhere)
	UPrimitiveComponent* GrabbedComponent;

private:
	//Gets PhysicsHandleComponent
	UPhysicsHandleComponent* GetPhysicsHandle() const;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, Category = "Grab")
	bool Holding = false;

		
};
