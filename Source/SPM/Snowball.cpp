// Fill out your copyright notice in the Description page of Project Settings.


#include "Snowball.h"

// Sets default values
ASnowball::ASnowball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Collission setup
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp->SetSphereRadius(20.0f); 
	RootComponent = CollisionComp;
	
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp -> SetUpdatedComponent(CollisionComp);
	MovementComp-> InitialSpeed = 3000.0f;
	MovementComp-> MaxSpeed = 3000.0f;
	MovementComp -> bRotationFollowsVelocity = true;
	MovementComp -> bShouldBounce = false;
	MovementComp -> ProjectileGravityScale = 1.0f;
}

void ASnowball::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASnowball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnowball::ThrowInDirection(const FVector& ThrowDirection)
{
	if (MovementComp)
	{
		MovementComp->Velocity = ThrowDirection * MovementComp->InitialSpeed;
	}
}

