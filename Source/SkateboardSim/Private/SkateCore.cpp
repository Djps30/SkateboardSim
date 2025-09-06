// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateCore.h"

#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"

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

}

// Called when the game starts or when spawned
void ASkateCore::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkateCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASkateCore::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

