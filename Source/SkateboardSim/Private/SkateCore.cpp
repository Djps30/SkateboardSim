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


}

// Called when the game starts or when spawned
void ASkateCore::BeginPlay()
{
	Super::BeginPlay();
	
	Parameters.AddIgnoredActor(GetOwner());
	Parameters.AddIgnoredComponent(StaticMesh);

	Mass = 0.85f * StaticMesh->BodyInstance.GetBodyMass();

}

// Called every frame
void ASkateCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < WheelPoints.Num(); i++)
	{
		FVector ComponentLocation = WheelPoints[i]->GetComponentLocation();
		FVector Start = ComponentLocation + FVector(0.f, 0.f, 50.f);
		FVector End = ComponentLocation - (WheelPoints[i]->GetUpVector() * (HoverDistance + WheelMeshOffset));

		bWheelGround[i] = GetWorld()->LineTraceSingleByChannel(ImpactInfo, Start, End, ECollisionChannel::ECC_Visibility, Parameters);

		if (bWheelGround[i])
		{
			SuspensionForce(i, WheelPoints[i], ImpactInfo, DeltaTime);
			SteerForce(i, WheelPoints[i]);
			//DrawDebugLine(GetWorld(), Start, End , FColor::Blue, false, 1.0f, (uint8)0, 2.0f);
		}
		else
		{
			//DrawDebugLine(GetWorld(), Start, End , FColor::Red, false, 1.0f, (uint8)0, 2.0f);
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
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.f, (uint8)0, 2.0f);

	//filter tireWorldVel

	tireWorldVel = tireWorldVel - (tireWorldVel * SuspensionComponent->GetUpVector());

	//tireWorldVel = tireWorldVel.GetSafeNormal();

	FVector steeringDir = SuspensionComponent->GetRightVector();

	float steeringVel = FVector::DotProduct(steeringDir, tireWorldVel);


	float tireGripFactor;

	float filterSvel = fabs(steeringVel);


	tireGripFactor = 1;
	

	float desiredVelChange = -steeringVel * tireGripFactor;

	float desiredAccel = desiredVelChange / GetWorld()->DeltaTimeSeconds;


	FVector ForceToApply = steeringDir * desiredAccel * (this->Mass / 10);



	StaticMesh->AddForceAtLocation(ForceToApply, SuspensionComponent->GetComponentLocation() + FVector(0, 0, 60.f));

	FVector ForceStart = SuspensionComponent->GetComponentLocation() + FVector(0, 0, 50.f);
	FVector ForceEnd = ForceStart + ForceToApply;

	//DrawDebugLine(GetWorld(), ForceStart, ForceEnd, FColor::Cyan, false, 0.f, (uint8)0, 2.0f);
}



