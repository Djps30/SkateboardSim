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

	TArray<AActor*> ScoredActors;



	void ScoringActor(AActor* Actorhit);


	
};
