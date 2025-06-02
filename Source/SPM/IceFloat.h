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

	UFUNCTION(BlueprintImplementableEvent, Category = "IceFloat")
	void RespawnEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "IceFloat")
	void BreakEvent();

	UFUNCTION(BlueprintCallable, Category = "IceFloat")
	void RespawnObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IceFloat")
	float BreakTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IceFloat")
	float RespawnTime = 0.0f;

	

private:

	FTimerHandle IcefloatTimerHandle;

	bool bHasRespawn = true;

	void HandleBreak();

	void TriggerAnimationBreak();

	void RespawnAnimation();
	
};
