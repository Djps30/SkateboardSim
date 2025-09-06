// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateCore.h"

#include "CollisionQueryParams.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/BodyInstance.h"


ASkateCore::ASkateCore()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateBoard"));
	StaticMesh->SetSimulatePhysics(true);
	RootComponent = StaticMesh;

	RiderMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rider"));
	RiderMesh->SetupAttachment(StaticMesh);

	FrontalWheels = CreateDefaultSubobject<USceneComponent>(TEXT("FrontWheels"));
	FrontalWheels->SetRelativeLocation(FVector(150.0f,0.0f,10.0f));
	FrontalWheels->SetupAttachment(StaticMesh);

	BackWheels = CreateDefaultSubobject<USceneComponent>(TEXT("BackWheels"));
	BackWheels->SetRelativeLocation(FVector(-150.0f, 0.0f, 10.0f));
	BackWheels->SetupAttachment(StaticMesh);


	WheelPointFR = CreateDefaultSubobject<USceneComponent>(TEXT("WheelPointFR"));
	WheelPointFR->SetRelativeLocation(FVector(0.0f, 33.0f, 0.0f));
	WheelPointFR->SetupAttachment(FrontalWheels);

	WheelPointFL = CreateDefaultSubobject<USceneComponent>(TEXT("WheelPointFL"));
	WheelPointFL->SetRelativeLocation(FVector(0.0f, -33.0f, 0.0f));
	WheelPointFL->SetupAttachment(FrontalWheels);

	WheelPointBR = CreateDefaultSubobject<USceneComponent>(TEXT("WheelPointBR"));
	WheelPointBR->SetRelativeLocation(FVector(0.0f, 33.0f, 0.0f));
	WheelPointBR->SetupAttachment(BackWheels);

	WheelPointBL = CreateDefaultSubobject<USceneComponent>(TEXT("WheelPointBL"));
	WheelPointBL->SetRelativeLocation(FVector(0.0f, -33.0f, 0.0f));
	WheelPointBL->SetupAttachment(BackWheels);

	WheelPoints.Add(WheelPointFR);
	WheelPoints.Add(WheelPointFL);
	WheelPoints.Add(WheelPointBR);
	WheelPoints.Add(WheelPointBL);

	PrevPosDelta.SetNum(WheelPoints.Num());
	bWheelGround.SetNum(WheelPoints.Num());
	WheelForceInterp.SetNum(WheelPoints.Num());


}

// Called when the game starts or when spawned
void ASkateCore::BeginPlay()
{
	Super::BeginPlay();
	
	Parameters.AddIgnoredActor(GetOwner());
	Parameters.AddIgnoredComponent(StaticMesh);

	Mass = 0.85f * StaticMesh->BodyInstance.GetBodyMass();

	AccumulatedTime = 0.f;

	FixedDeltaTime = 1.0f / 60.f;

}

// Called every frame
void ASkateCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulatedTime += DeltaTime;
	while (AccumulatedTime >= FixedDeltaTime)
	{
		UpdateFixedDelta(FixedDeltaTime);
		AccumulatedTime -= FixedDeltaTime;
	}

	for (int i = 0; i < WheelPoints.Num(); i++)
	{
		FVector ComponentLocation = WheelPoints[i]->GetComponentLocation();
		FVector Start = ComponentLocation + FVector(0.f, 0.f, 50.f);
		FVector End = ComponentLocation - (WheelPoints[i]->GetUpVector() * (HoverDistance + WheelMeshOffset));

		bWheelGround[i] = GetWorld()->LineTraceSingleByChannel(ImpactInfo, Start, End, ECollisionChannel::ECC_Visibility, Parameters);

		if (bWheelGround[i])
		{
			SuspensionForce(i, WheelPoints[i], ImpactInfo, DeltaTime);
			if(bDebug) DrawDebugLine(GetWorld(), Start, End , FColor::Blue, false, 1.0f, (uint8)0, 2.0f);
		}
		else
		{
			if (bDebug) DrawDebugLine(GetWorld(), Start, End , FColor::Red, false, 1.0f, (uint8)0, 2.0f);
		}
	}

}

void ASkateCore::UpdateFixedDelta(float FixedDT)
{
	SpeedReduction();

	WheelsRotLogic();

	ImpulsionLogic();

	for (int i = 0; i < WheelPoints.Num(); i++)
	{
		//UE_LOG(LogTemp, Display, TEXT("Inside WheelsLogic FOR"));

		FVector ComponentLocation = WheelPoints[i]->GetComponentLocation();
		FVector Start = ComponentLocation + FVector(0.f, 0.f, 50.f);
		FVector End = ComponentLocation - (WheelPoints[i]->GetUpVector() * (HoverDistance + WheelMeshOffset));

		bWheelGround[i] = GetWorld()->LineTraceSingleByChannel(ImpactInfo, Start, End, ECollisionChannel::ECC_Visibility, Parameters);

		if (bWheelGround[i])
		{
			SteerForce(i, WheelPoints[i]);
			WheelForceInterp[i] = FVector(0.f, 0.f, 0.f);
		}
		else
		{
			bool bNeedforalign;
			FHitResult ImpactforAlign;
			FVector AlignCheckDir = Start - (FVector(0.f, 0.f, 1.f) * (HoverDistance + WheelMeshOffset));

			bNeedforalign = GetWorld()->LineTraceSingleByChannel(ImpactforAlign, Start, AlignCheckDir, ECollisionChannel::ECC_Visibility, Parameters);
			FVector Pullforce;

			if (!bNeedforalign)
			{
				Pullforce = WheelPoints[i]->GetUpVector() * (this->Mass) * GetWorld()->GetGravityZ();
				if (bDebug) DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.0f, (uint8)0, 2.0f);
			}
			else
			{
				Pullforce = -WheelPoints[i]->GetUpVector() * (this->Mass) * GetWorld()->GetGravityZ();
				if (bDebug) DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, (uint8)0, 2.0f);
			}

			//UE_LOG(LogTemp, Display, TEXT("Not on bGround"));

			FVector GravityForce = FVector(0.f, 0.f, (this->Mass) * GetWorld()->GetGravityZ());

			FVector ForceToApply = (GravityForce + Pullforce)/2;

			WheelForceInterp[i] = FMath::VInterpTo(WheelForceInterp[i], ForceToApply, FixedDT, 0.5f);

			StaticMesh->AddForceAtLocation(WheelForceInterp[i], WheelPoints[i]->GetComponentLocation());
		}

	}
}


void ASkateCore::SuspensionForce(int arrayPos, TObjectPtr<USceneComponent> SuspensionComponent, FHitResult ImpInfo, float DTime)
{

	float fps = 1.0f / DTime;

	float PosDelta = HoverDistance - (SuspensionComponent->GetComponentLocation().Z - ImpInfo.Location.Z);
	float Velocity = (PosDelta - PrevPosDelta[arrayPos]) / DTime;

	float TotalForce = (PosDelta * SpringCurve->GetFloatValue(fps)) + (Velocity * DampingCoeficient);

	FVector ForceToApply = FVector(0.f, 0.f, TotalForce);
	StaticMesh->AddForceAtLocation(ForceToApply, SuspensionComponent->GetComponentLocation(), NAME_None);


	PrevPosDelta[arrayPos] = PosDelta;
}

void ASkateCore::SteerForce(int arrayPos, TObjectPtr<USceneComponent> SuspensionComponent)
{
	FVector ComponentLocation = SuspensionComponent->GetComponentLocation();
	FVector Start = ComponentLocation + FVector(0.f, 0.f, 50.f);
	FVector End;
	FBodyInstance rb = StaticMesh->BodyInstance;
	FVector tireWorldVel = rb.GetUnrealWorldVelocityAtPoint(SuspensionComponent->GetComponentLocation());


	End = Start + tireWorldVel;

	if(bDebug) DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.f, (uint8)0, 2.0f);

	//filter tireWorldVel

	tireWorldVel = tireWorldVel - (tireWorldVel * SuspensionComponent->GetUpVector());

	//tireWorldVel = tireWorldVel.GetSafeNormal();

	FVector steeringDir = SuspensionComponent->GetRightVector();

	float steeringVel = FVector::DotProduct(steeringDir, tireWorldVel);

	float tireGripFactor = 0.5f;
	
	float desiredVelChange = -steeringVel * tireGripFactor;

	float desiredAccel = desiredVelChange / GetWorld()->DeltaTimeSeconds;


	FVector ForceToApply = steeringDir * desiredAccel * (this->Mass / 10);

	StaticMesh->AddForceAtLocation(ForceToApply, SuspensionComponent->GetComponentLocation() + FVector(0, 0, 60.f));

	FVector ForceStart = SuspensionComponent->GetComponentLocation() + FVector(0, 0, 50.f);
	FVector ForceEnd = ForceStart + ForceToApply;

	if (bDebug) DrawDebugLine(GetWorld(), ForceStart, ForceEnd, FColor::Cyan, false, 0.f, (uint8)0, 2.0f);
}

//Functions from PController
#pragma region PControllerFunctions
void ASkateCore::SetImpulse(float input)
{
	UE_LOG(LogTemp, Display, TEXT("IMPULSE %f"), input);
	Impulse_Input = input;

	
}

void ASkateCore::SetSteer(float input)
{
	UE_LOG(LogTemp, Display, TEXT("STEER %f"), input);

	Steer_Input = input;
}

#pragma endregion



void ASkateCore::SpeedReduction()
{
	FVector bodyVelocity;
	FVector Final;

	bodyVelocity = StaticMesh->GetComponentVelocity();

	Final = bodyVelocity * 0.99f;

	StaticMesh->SetPhysicsLinearVelocity(Final, false, NAME_None);
}

void ASkateCore::WheelsRotLogic()
{
	float Angle = MaxTurningAngle * Steer_Input;

	WheelTurnSmoother = FMath::FInterpTo(WheelTurnSmoother, Angle, FApp::GetFixedDeltaTime(), 5);

	FrontalWheels->SetRelativeRotation(FRotator(0,Angle,0));
	BackWheels->SetRelativeRotation(FRotator(0, Angle*-1, 0));
}

void ASkateCore::ImpulsionLogic()
{
	if (Impulse_Input != 0)
	{
		PushFunction(Impulse_Input * 20.f);
	}

}

void ASkateCore::PushFunction(float power)
{
	StaticMesh->AddImpulse(StaticMesh->GetForwardVector() * power, NAME_None, true);
}



