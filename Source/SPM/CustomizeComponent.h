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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customize")
	TArray<UMaterial*> CharacterMaterials;

	//Use next material in list
	UFUNCTION(BlueprintCallable, Category = "Customize")
	void NextMaterial();

	//Use next material in list
	UFUNCTION(BlueprintCallable, Category = "Customize")
	void PreviousMaterial();

	//Choose material by index
	UFUNCTION(BlueprintCallable, Category = "Customize")
	void SetMaterialByIndex(int MaterialIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customize")
	int MaterialID = 0;

		
};
