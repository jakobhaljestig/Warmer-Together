// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BodyTemperature.h"
#include "InputActionValue.h"
#include "WeatherComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ThrowSnowballComponent.h"
#include "CharacterBase.generated.h"


class UPerformanceTracker;
class UBodyTemperature;
class UAdaptiveWeatherSystem;
class UBodyTemplate;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPushComponent;
class USprintComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACharacterBase : public ACharacter
{
	GENERATED_BODY()

	friend class UHugComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowSnowballAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DanceAction;

public:
	
	ACharacterBase();
	
	void Tick(float DeltaTime);

	void ApplySnowballHit() const;

protected:

	virtual void BeginPlay() override;
	
	//MOVEMENT + LOOK
	virtual void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	virtual void Jump() override;

	// RÖRELSE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseMovementSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrentMovementSpeed;

	//Sprint -
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprint")
	USprintComponent* SprintComponent;
	
	void StartSprint ();
	void StopSprint();
	
	//HUGGING
	void BeginHug(const FInputActionValue& Value);
	void EndHug(const FInputActionValue& Value);
	void Hug();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Push")
	UHugComponent* HugComponent;
	
	// KROPPSTEMPERATUR
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Temperature")
	UBodyTemperature* BodyTempComponent;

	//PUSH
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Push")
	UPushComponent* PushComponent;

	UPROPERTY(BlueprintReadOnly)
	bool bIsPushing = false;

	virtual void BeginPush(const FInputActionValue& Value);
	void EndPush(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dance")
	bool bIsDancing;
	
	void StartDance(const FInputActionValue& Value);
	void EndDance(const FInputActionValue& Value);

	//---Kasta Snöboll ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snowball")
	UThrowSnowballComponent* ThrowSnowballComponent;

	void Throw(const FInputActionValue& Value);
	
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	float HugTimer = 0;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float HugTime = 5.6;

	//VÄDER
	UPROPERTY()
	UWeatherComponent* WeatherComponent;
	

	//FALL DAMAGE
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fall Damage")
	float FallDamageMultiplier = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fall Damage")
	float FallDamageThreshold = 6.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fall Damage")
	float LastGroundedZ = 0.0f;

	void Landed(const FHitResult& Hit);
	

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
	
	void UpdateVisibility(float VisibilityFactor);
	

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	//CAMERA
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Referens till vår performance-tracker
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Performance")
	UPerformanceTracker* PerformanceTracker;

	//RESPAWNING
	
	//Mini Respawning
	UPROPERTY(BlueprintReadWrite, Category = "Respawn")
    FVector LastSafeLocation;

	void SetCheckpointLocation(FVector Location);
	
	void RespawnAtCheckpoint();
	
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void RespawnToLastSafeLocation();

	virtual void OnDeath();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Performance")
	bool bHasDied = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsHugging;

	UPROPERTY(BlueprintReadOnly)
	bool bSuccesfulHug = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSprinting;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsLifting = false;
	
private:

	
	void UpdatePlayerLocation();

	UPROPERTY(VisibleAnywhere, Category = "Respawn")
	FVector CheckpointLocation;
	
	FTimerHandle RespawnTimeHandle;

	UPROPERTY(EditAnywhere, Category = "Respawn")
	float RespawnTimeDuration = 5.f;



	
	// --- Coyote Time ---
	FTimerHandle CoyoteTimeHandle;
	
	bool bCanUseCoyoteTime = false;

	bool bHasJumped = false;
	
	UPROPERTY(EditAnywhere, Category = "Jump")
	float CoyoteTimeDuration = 0.2f;
	
	void EnableCoyoteTime();
	
	void DisableCoyoteTime();

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	
	virtual bool CanJumpInternal_Implementation() const override;

	virtual void OnJumped_Implementation() override;
	
	virtual void Falling() override;

	bool bHasCheckPointLocation = false;

	void ResetTemp() const;
	
};

