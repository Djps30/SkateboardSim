// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkateCore.h"
#include "GameFramework/PlayerController.h"
#include "SkatePController.generated.h"


class UInputAction;
struct FInputActionValue;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class SKATEBOARDSIM_API ASkatePController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;


#pragma region InputFunctions

	void InputImpulse(const FInputActionValue& fValue);

	void InputSteer(const FInputActionValue& fValue);



#pragma endregion

public:
	virtual void SetupInputComponent() override;

	class ASkateCore* SkateRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Impulse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Steer;
};
