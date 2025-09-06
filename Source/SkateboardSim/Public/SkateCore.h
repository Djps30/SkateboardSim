// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SkateCore.generated.h"

UCLASS()
class SKATEBOARDSIM_API ASkateCore : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASkateCore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




	// Boolean for displaying Debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bDebug;

#pragma region Components

	//Base component of the class. Physic calcs will use this component as basis.

	UPROPERTY(VisibleDefaultsOnly)	UStaticMeshComponent* StaticMesh;

	// We set the wheels into these to help the board to rotate afterwards.

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)	USceneComponent* FrontalWheels;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)	USceneComponent* BackWheels;

	//Empty RefPoints for wheels. Using these to help for Steer, Suspension.

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)	USceneComponent* WheelPointFL;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)	USceneComponent* WheelPointFR;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)	USceneComponent* WheelPointBL;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)	USceneComponent* WheelPointBR;



#pragma endregion

#pragma region Properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") TArray<USceneComponent*> WheelPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") TArray<float> PrevPosDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") TArray<bool> bWheelGround;
#pragma endregion


#pragma region Functions



#pragma endregion



};
