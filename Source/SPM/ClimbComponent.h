// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClimbComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Checka om man kan klättra
	bool CanClimb();

	void StartClimb();

	void StopClimb();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClimbSpeed = 200.0f;


	UPROPERTY(EditAnywhere)
	ACharacter* ClimbCharacter;

	//För att påverka movement på Player när den klättrar. 
	UPROPERTY(EditAnywhere)
	class UCharacterMovementComponent* MovementComponent;
	
private:
	bool bIsClimbing;

	bool ClimbingInReach (FHitResult& HitResult);
	

		
};
