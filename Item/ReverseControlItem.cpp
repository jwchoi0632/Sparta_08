// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ReverseControlItem.h"

void AReverseControlItem::ActionOverlapEvent(AActor* User)
{
	Super::ActionOverlapEvent(User);

	if (!IsValid(BaseGameState))
	{
		return;
	}
	BaseGameState->StartReverse();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			BaseGameState->EndReverse();
		}), ReverseTime, false);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}
