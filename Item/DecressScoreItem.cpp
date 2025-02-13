// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DecressScoreItem.h"

void ADecressScoreItem::ActionOverlapEvent(AActor* User)
{
	Super::ActionOverlapEvent(User);

	if (!IsValid(BaseGameState))
	{
		return;
	}

	BaseGameState->AddScore(-PointValue);
}
