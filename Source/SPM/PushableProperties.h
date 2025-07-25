// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PushableProperties.generated.h"


class UPushComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UPushableProperties : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPushableProperties();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfGrabbers = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RequiredNumberOfGrabbers = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UPushComponent*> Grabbers;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HoldDistance = 100;

	//Checks if object should be able to be pushed
	bool CanPush() const;

	//Gets location of all grabbers and centers the location to prevent janky behavior
	FVector GetPushPosition() const;

	//Checks if all grabbers are facing a similiar direction
	bool GrabbersHaveSameRotation() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling = false;

	

		
};
