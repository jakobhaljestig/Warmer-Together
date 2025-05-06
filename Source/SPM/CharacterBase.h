// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BodyTemperature.h"
#include "Health.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterBase.generated.h"

class UPerformanceTracker;
class UBodyTemperature;
class UHealth;
class UAdaptiveWeatherSystem;
class UBodyTemplate;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPushComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACharacterBase : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/* MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HugAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PushAction;


public:
	ACharacterBase();
	
	void Tick(float DeltaTime);

protected:

	virtual void BeginPlay() override;
	
	/** Called for movement input */
	virtual void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void BeginHug(const FInputActionValue& Value);
	void EndHug(const FInputActionValue& Value);

	void Hug() const;

	virtual void BeginPush(const FInputActionValue& Value);
	void EndPush(const FInputActionValue& Value);

	void Landed(const FHitResult& Hit);
	
	// Kroppstemperatur
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Temperature")
	UBodyTemperature* BodyTempComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Push")
	UPushComponent* PushComponent;

	UPROPERTY()
	UAdaptiveWeatherSystem* AdaptiveWeatherSystem;
	

	// Rörelse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseMovementSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrentMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fall Damage")
	float FallDamageMultiplier = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fall Damage")
	float FallDamageThreshold = 6.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fall Damage")
	float LastGroundedZ = 0.0f;

	// Kylningsfaktor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
	float BaseCoolingRate = 5.0f;

	// Vind
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	float WindResistanceThreshold = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	float MaxWindSpeed = 25.0f;

	// Sikt
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	class APostProcessVolume* PostProcessVolume;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsTryingToHug = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsPushing = false;


	// Siktmetod
	void UpdateVisibility(float VisibilityFactor);
	

protected:


	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Temperature")
	UBodyTemperature* BodyTemperatureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealth* HealthComponent;

	// Referens till vår performance-tracker
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Performance")
	UPerformanceTracker* PerformanceTracker;

	//Mini Respawning
	UPROPERTY(BlueprintReadWrite, Category = "Respawn")
    FVector LastSafeLocation;

	void SetCheckpointLocation(FVector Location);
	
	void RespawnAtCheckpoint();
	
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void RespawnToLastSafeLocation();

	void OnDeath();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Performance")
	bool bHasDied = false;

private:
	void UpdatePlayerLocation();

	UPROPERTY(VisibleAnywhere, Category = "Respawn")
	FVector CheckpointLocation;

	bool bIsOnClimbingEdge = false;

};

