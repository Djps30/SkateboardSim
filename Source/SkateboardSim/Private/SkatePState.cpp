// Fill out your copyright notice in the Description page of Project Settings.


#include "SkatePState.h"

void ASkatePState::ScoringActor(AActor* Actorhit)
{
	bool NoMatch = true;
	for (int i = 0; i < ScoredActors.Num(); i++)
	{
		if (ScoredActors[i] == Actorhit)
		{
			NoMatch = false;
			break;
		}
	}

	if (NoMatch)
		ScoredActors.Add(Actorhit);


}
