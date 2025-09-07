// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SkatePState.generated.h"

/**
 * 
 */
UCLASS()
class SKATEBOARDSIM_API ASkatePState : public APlayerState
{
	GENERATED_BODY()

	


public:
	void ScoringActor(AActor* Actorhit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<AActor*> ScoredActors;

	
};
