// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Curves/CurveVector.h"
#include "GameFramework/Pawn.h"
#include "SkateCore.generated.h"


class USpringArmComponent;
class UCameraComponent;


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


	//Floats for making a FixedUpdate logic.
	float AccumulatedTime;

	float FixedDeltaTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Fixed Update function
	void UpdateFixedDelta(float FixedDT);





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

	//Mesh for Rider

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) USkeletalMeshComponent* RiderMesh;

	//Camera

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")	UCameraComponent* Camera;


#pragma endregion

#pragma region Properties

	//Arrays to help manipulate the Wheel points.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") TArray<USceneComponent*> WheelPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") TArray<float> PrevPosDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") TArray<bool> bWheelGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") TArray<FVector> WheelForceInterp;


	//Parameters for wheels.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") float HoverDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") float WheelMeshOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") TObjectPtr<UCurveFloat> SpringCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels") float DampingCoeficient;

	FHitResult ImpactInfo;

	FCollisionQueryParams Parameters;

	float Mass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkateCore")float Impulse_Input;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkateCore")float Steer_Input;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkateCore")float Jump_Input;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkateCore|Parameters")float MaxTurningAngle;

	float WheelTurnSmoother;

	float AirTurnSmoother;

	float JumpingCharge;



	//Animation Booleans

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkateCore|AnimBool") bool bCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkateCore|AnimBool") bool bPushing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkateCore|AnimBool") bool bJumping;


#pragma endregion


#pragma region Functions

	void SuspensionForce(int arrayPos, TObjectPtr<USceneComponent> SuspensionComponent, FHitResult ImpInfo, float DTime);

	void SteerForce(int arrayPos, TObjectPtr<USceneComponent> SuspensionComponent);



	void SetImpulse(float input);

	void SetSteer(float input);

	void SetJump(float input);


	//Make the Skate lose speed naturaly.
	void SpeedReduction();

	void WheelsRotLogic();

	void ImpulsionLogic();

	void JumpingLogic(float DeltaT);

	UFUNCTION(BlueprintCallable)
	void PushFunction(float power);

	void TimedPush();

	UFUNCTION(BlueprintCallable)
	void JumpFunction(float power);

	void ClearJumpBool();

	void AirTurnLogic();


	bool CheckIfGround();

#pragma endregion


	FTimerHandle ImpulseBoolTimer;
};
