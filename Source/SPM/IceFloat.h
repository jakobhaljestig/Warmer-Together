// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include  "Components/BoxComponent.h"
#include "IceFloat.generated.h"

UCLASS()
class SPM_API AIceFloat : public AActor
{
	GENERATED_BODY()
	
public:	
	AIceFloat();

protected:
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* IceCollider; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* IceMesh;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "IceFloat")
	void BreakObject();

private:

	FTimerHandle BreakTimerHandle;
	FTimerHandle RespawnTimerHandle;

	void HandleBreak();
	
	void RespawnObject();

	UPROPERTY(EditAnywhere)
	float RespawnTime = 2.5f;

	//Använd för att sätta som delay för timeline så isflaket inte åker upp innan despawn. 
	UPROPERTY(EditAnywhere)
	float BreakTime = 1.0f;
	

};
