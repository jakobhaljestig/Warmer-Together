// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterBase.h"

#include "BodyTemperature.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Health.h"
#include "InputActionValue.h"
#include "PerformanceTracker.h"
#include "PushComponent.h"
//#include "WeatherController.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


ACharacterBase::ACharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	BodyTempComponent = CreateDefaultSubobject<UBodyTemperature>(TEXT("BodyTemperature"));
	HealthComponent = CreateDefaultSubobject<UHealth>(TEXT("Health"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentMovementSpeed = BaseMovementSpeed;
	CheckpointLocation = GetActorLocation();

	if (!PerformanceTracker)
	{
		PerformanceTracker = FindComponentByClass<UPerformanceTracker>();  // Hitta om den finns på samma objekt
		if (!PerformanceTracker)
		{
			UE_LOG(LogTemp, Error, TEXT("PerformanceTracker not found!"));
		}
	}

	PushComponent = FindComponentByClass<UPushComponent>();
	if (!PushComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PickupComponent not valid"));
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		LastGroundedZ = GetActorLocation().Z;
	}
	
	UpdateLastSafeLocation();

	UAdaptiveWeatherSystem* WeatherSystemInstance = GetGameInstance()->GetSubsystem<UAdaptiveWeatherSystem>();

	if (!WeatherSystemInstance || !BodyTempComponent)
		return;

	const FWeatherState& CurrentWeather = WeatherSystemInstance->GetCurrentWeather();

	// Temperatur påverkar kroppstemperatur
	// float TempFactor = FMath::Clamp(-CurrentWeather.Temperature / 30.0f, 0.0f, 1.0f);
	// BodyTempComponent->CoolDown(DeltaTime * TempFactor * BaseCoolingRate);

	// Vind påverkar rörelse
	if (CurrentWeather.WindSpeed > WindResistanceThreshold)
	{
		float WindFactor = FMath::Clamp(CurrentWeather.WindSpeed / MaxWindSpeed, 0.0f, 1.0f);
		CurrentMovementSpeed = BaseMovementSpeed * (1.0f - WindFactor * 0.4f);
	}
	else
	{
		CurrentMovementSpeed = BaseMovementSpeed;
	}
	
	// Snö påverkar sikt – detta kan styra t.ex. dimma, post-process etc
	// UpdateVisibility(CurrentWeather.Visibility);

}

void ACharacterBase::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterBase::Look);
		
		EnhancedInputComponent->BindAction(HugAction, ETriggerEvent::Started, this, &ACharacterBase::BeginHug);
        EnhancedInputComponent->BindAction(HugAction, ETriggerEvent::Completed, this, &ACharacterBase::EndHug);

		EnhancedInputComponent->BindAction(PushAction, ETriggerEvent::Started, this, &ACharacterBase::TogglePush);
		EnhancedInputComponent->BindAction(PushAction, ETriggerEvent::Completed, this, &ACharacterBase::TogglePush);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACharacterBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACharacterBase::BeginHug(const FInputActionValue& Value)
{
	bIsTryingToHug = true;
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Hug mapping is working"));

	ACharacter* Char1 = UGameplayStatics::GetPlayerCharacter(this, 0);
	ACharacter* Char2 = UGameplayStatics::GetPlayerCharacter(this, 1);

	if (!Char1 || !Char2 || Char1 == Char2) return;

	ACharacterBase* Player1 = Cast<ACharacterBase>(Char1);
	ACharacterBase* Player2 = Cast<ACharacterBase>(Char2);

	if (!Player1 || !Player2) return;

	const float HugDistance = 200.0f; // Kanske inte hårdkoda avständ

	if (Player1->bIsTryingToHug && Player2->bIsTryingToHug)
	{
		float Distance = FVector::Dist(Player1->GetActorLocation(), Player2->GetActorLocation());

		if (Distance <= HugDistance)
		{	
			Player1->Hug();
		}else
		{
			UE_LOG(LogTemplateCharacter, Warning, TEXT("Distance too big between players"));
		}
		
	}else{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("Only 1 character hugging"));
	}

}

void ACharacterBase::EndHug(const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Hug has ended"));
	bIsTryingToHug = false;
}

void ACharacterBase::Hug()
{
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Characters are hugging"));
	if (BodyTempComponent)
    	{
    		BodyTempComponent->ShareTemp();
    	}
    	else
    	{
    		UE_LOG(LogTemplateCharacter, Error, TEXT("BodyTempComponent is nullptr!"));
    	}
	//GetOwner()->GetComponentByClass<UBodyTemperature>()->ShareTemp();
}

void ACharacterBase::TogglePush(const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Display, TEXT("Push Toggled"));
	PushComponent->GrabAndRelease();
}
void ACharacterBase::OnDeath()
{
	if (bHasDied)
		return;

	bHasDied = true;

	UE_LOG(LogTemp, Warning, TEXT("OnDeath triggered."));

	//metod för att uppdatera performancetracker.cpp
	if (PerformanceTracker)
	{
		PerformanceTracker->RegisterDeath();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PerformanceTracker is null!"));
	}
	
		RespawnAtCheckpoint();
	
}

void ACharacterBase::SetCheckpointLocation(FVector Location)
{
	CheckpointLocation = Location;
}

void ACharacterBase::RespawnAtCheckpoint()
{
	FVector NewLocation = FVector(CheckpointLocation.X - 200, CheckpointLocation.Y, CheckpointLocation.Z + 46);
	bHasDied = false;

	//bör inte ticka ner efter spelaren respawnar, endast kylenivån, sen efter kylenivån är på 0 så ska den börja ticka igen
	if (HealthComponent)
	{
		HealthComponent->ResetHealth();
	}

	//denna bör dock
	UBodyTemperature* Temp = Cast<UBodyTemperature>(BodyTempComponent);
	Temp->ResetTemp();
	
	SetActorLocation(NewLocation);
}

void ACharacterBase::RespawnToLastSafeLocation()
{
	SetActorLocation(LastSafeLocation, false, nullptr, ETeleportType::TeleportPhysics);
}

void ACharacterBase::UpdateLastSafeLocation()
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		AActor* Ground = GetCharacterMovement() -> CurrentFloor.HitResult.GetActor();

		if (!Ground-> ActorHasTag(TEXT("IgnoreLastSafeLocation")))
		{
			if (FVector::Dist(LastSafeLocation, GetActorLocation()) > 50.0f)
			{
				LastSafeLocation = GetActorLocation();
			}
		}
		
	}

}

void ACharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// Calculate fall distance
	float FallHeight = LastGroundedZ - GetActorLocation().Z;
	float FallDistanceMeters = FallHeight / 100.0f; 
	
	if (FallDistanceMeters > FallDamageThreshold) // meters min for fall damage
	{
		float FallDamage = 50.f + ((FallDistanceMeters - FallDamageThreshold) * FallDamageMultiplier); 
		HealthComponent->TakeDamage(FallDamage);
		UE_LOG(LogTemp, Warning, TEXT("Threshold reached %f"),FallDistanceMeters);
		UE_LOG(LogTemp, Warning, TEXT("Threshold reached %f"),FallDamage);
	} 
}

