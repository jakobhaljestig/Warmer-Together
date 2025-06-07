// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeatherComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterBase.generated.h"


class UPhysicsHandleComponent;
class UPerformanceTracker;
class UBodyTemperature;
class UThrowSnowballComponent;
class UCustomizeComponent;
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

	void ApplySnowballHit(const FHitResult& Hit, const FVector& ImpactDirection);

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
	
	void StartSprint();
	void StopSprint();
	
	//HUGGING
	virtual void BeginHug(const FInputActionValue& Value);
	void EndHug(const FInputActionValue& Value);
	void Hug();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Push")
	UHugComponent* HugComponent;
	
	// KROPPSTEMPERATUR
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Temperature")
	UBodyTemperature* BodyTempComponent;

	//PUSH
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Push")
	UPushComponent* PushComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customize")
	UCustomizeComponent* CustomizeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PhysicsHandle")
	UPhysicsHandleComponent* PhysicsHandleComponent;

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

	void Aim(const FInputActionValue& Value);

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
	
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool IsClimbing() const {return false;}

public:

	//CAMERA
	
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UBodyTemperature* GetBodyTemperature() const { return BodyTempComponent; }

	UPhysicsHandleComponent* GetPhysicsHandle() const { return PhysicsHandleComponent; }
	
	//RESPAWNING
	
	//Mini Respawning
	UPROPERTY(BlueprintReadWrite, Category = "Respawn")
    FVector LastSafeLocation;

	void SetCheckpointLocation(FVector Location);
	
	void RespawnAtCheckpoint();
	
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void RespawnToLastSafeLocation();

	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void StartDelayedRespawn();

	

	UFUNCTION(BlueprintCallable)
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

	UPROPERTY(BlueprintReadOnly)
	bool bIsAimingSB = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsThrowingSB = false;

	UFUNCTION(BlueprintCallable)
	virtual void ResetPlayerState();

	UFUNCTION(BlueprintCallable)
	void SetCameraRotationRate(const float NewRate) { CameraRotationRate = NewRate; }
	
private:
	
	void UpdateLastSafeLocation();

	UPROPERTY(VisibleAnywhere, Category = "Respawn")
	FVector CheckpointLocation;
	
	FTimerHandle RespawnTimeHandle;

	UPROPERTY(EditAnywhere, Category = "Respawn")
	float RespawnTimeDuration = 5.f;

	FVector SavedCameraLocation;
	FRotator SavedCameraRotation;
	
	bool bHasCheckPointLocation = false;

	void ResetTemp() const;
	
	bool bIsRagdoll = false; 
	
	// --- Coyote Time ---
	FTimerHandle CoyoteTimeHandle;
	
	bool bCanUseCoyoteTime = false;

	bool bHasJumped = false;
	
	UPROPERTY(EditAnywhere, Category = "Jump")
	float CoyoteTimeDuration = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotationRate = 70.f;
	
	void EnableCoyoteTime();
	
	void DisableCoyoteTime();

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	
	virtual bool CanJumpInternal_Implementation() const override;

	virtual void OnJumped_Implementation() override;
	
	virtual void Falling() override;
	

	
};

