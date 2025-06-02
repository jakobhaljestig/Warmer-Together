// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomizeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UCustomizeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomizeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customize")
	UMaterial* CurrentMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customize")
	TArray<UMaterial*> CharacterMaterials;

	UFUNCTION(BlueprintCallable, Category = "Customize")
	void NextMaterial();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customize")
	int MaterialID = 0;

		
};
