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

	UFUNCTION(BlueprintCallable)
	virtual void GrabAndRelease();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool GetGrabbableInReach(FHitResult& OutHitResult) const;

	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	virtual void Grab();

	virtual void Release();
	
	UPROPERTY(EditAnywhere, Category = "Grab")
	float GrabDistance = 200.f;
	
	UPROPERTY(EditAnywhere, Category = "Grab")
	float GrabRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = "Grab")
	float HoldDistance = 200.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Grab")
	bool Holding = false;

	bool HoldingSomething() const;

	ECollisionChannel CollisionChannel;

	UCharacterMovementComponent* OwnerMovementComponent;

	virtual void GrabEffect();

private:
	UPhysicsHandleComponent* GetPhysicsHandle() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
