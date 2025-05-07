// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterBase.h"
#include "HugComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UHugComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHugComponent();

	void TryHug();
	void EndHug();

	bool IsTryingToHug () const {return bIsTryingToHug;}

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditAnywhere)
	ACharacterBase* OwnerCharacter = nullptr;

	UPROPERTY(EditAnywhere)
	float HugDistance = 150.0f;
	
	bool bIsTryingToHug = false;

	void MovePlayersTogether(ACharacterBase* P1, ACharacterBase* P2);
	

		
};
