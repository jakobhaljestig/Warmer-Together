// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBig.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"



void ACharacterBig::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// För test
		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Started, this, &ACharacterBig::Crawl);
		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Completed, this, &ACharacterBig::StopCrawl);
	
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

//För test
void ACharacterBig::Crawl (const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Display, TEXT("Player Big  is Crouching"));
}

void ACharacterBig::StopCrawl(const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Display, TEXT("Player Big is not Crouching"));
}

