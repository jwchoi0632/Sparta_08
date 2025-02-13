// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SlowingItem.h"
#include "Player/PlayerCharacter.h"

void ASlowingItem::ActionOverlapEvent(AActor* User)
{
	Super::ActionOverlapEvent(User);

	if (!IsValid(BaseGameState))
	{
		return;
	}
	BaseGameState->StartSlow(SlowRate);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			BaseGameState->EndSlow();
			DestroyItem();
		}), SlowTime, false);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}
