// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Snowball.generated.h"

UCLASS()
class SPM_API ASnowball : public AActor
{
	GENERATED_BODY()
	
public:	
	ASnowball();

	//För snöbollens kollission
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CollisionComp;

	//projektilrörelsen
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;

	void ThrowInDirection(const FVector& ThrowDirection);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float Speed = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float gravity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float MaxLifeTime = 5.0f;

protected:
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	

};
