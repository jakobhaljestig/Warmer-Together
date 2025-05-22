
#include "SprintComponent.h"
#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"



USprintComponent::USprintComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USprintComponent::BeginPlay()
{
	Super::BeginPlay();
	Stamina =  MaxStamina;
}

void USprintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USprintComponent::StartSprint()
{
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner && bCanSprint && Stamina > 0)
	{
		CharacterOwner->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		
		GetWorld()->GetTimerManager().SetTimer(StaminaCooldownTimerHandle, this, &USprintComponent::DrainStamina, 0.05f, true);
	}
}

void USprintComponent::DrainStamina()
{
	if (Stamina > 0)
	{
		Stamina -= StaminaDrainRate * 0.1f; 
		
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("Stamina: %.2f"), Stamina));

		if (Stamina <= 0)
		{
			Stamina = 0;
			bCanSprint = false;
			StopSprint();
			GetWorld()->GetTimerManager().ClearTimer(StaminaCooldownTimerHandle); 
		}
	}
}

void USprintComponent::StopSprint()
{
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	CharacterOwner->GetCharacterMovement()->MaxWalkSpeed = 500.f; 
	GetWorld()->GetTimerManager().ClearTimer(StaminaCooldownTimerHandle);
	
	GetWorld()->GetTimerManager().SetTimer(StaminaCooldownTimerHandle, this, &USprintComponent::RegenerateStamina, 0.1f, true);
}

void USprintComponent::RegenerateStamina()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("Stamina: %.2f"), Stamina));
	if (Stamina < MaxStamina)
	{
		Stamina += StaminaRegenRate * GetWorld()->GetDeltaSeconds();
		
		if (Stamina >= MaxStamina)
		{
			Stamina = MaxStamina;
			bIsRegenerating = false;
			GetWorld()->GetTimerManager().ClearTimer(StaminaCooldownTimerHandle);
		}
	}
	if (Stamina > 0 && !bCanSprint) 
	{
		bCanSprint = true;
	}
}

