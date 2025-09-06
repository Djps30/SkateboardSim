// Fill out your copyright notice in the Description page of Project Settings.


#include "SkatePController.h"


#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ASkatePController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ASkatePController::OnPossess(APawn* InPawn)
{

	Super::OnPossess(InPawn);
	SkateRef = Cast<ASkateCore>(InPawn);
	if (SkateRef)
	{
		UE_LOG(LogTemp, Display, TEXT("Cast Sucessful!"));
	}

	
}



void ASkatePController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Impulse, ETriggerEvent::Triggered, this, &ASkatePController::InputImpulse);
		EnhancedInputComponent->BindAction(IA_Impulse, ETriggerEvent::Completed, this, &ASkatePController::InputImpulse);

		EnhancedInputComponent->BindAction(IA_Steer, ETriggerEvent::Triggered, this, &ASkatePController::InputSteer);
		EnhancedInputComponent->BindAction(IA_Steer, ETriggerEvent::Completed, this, &ASkatePController::InputSteer);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ASkatePController::InputJump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ASkatePController::InputJump);
	}
	
}

void ASkatePController::InputImpulse(const FInputActionValue& fValue)
{
	SkateRef->SetImpulse(fValue.Get<float>());
}

void ASkatePController::InputSteer(const FInputActionValue& fValue)
{
	SkateRef->SetSteer(fValue.Get<float>());
}

void ASkatePController::InputJump(const FInputActionValue& fValue)
{
	SkateRef->SetJump(fValue.Get<float>());
}
