// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CoinItem.h"

void ACoinItem::ActionOverlapEvent(AActor* User)
{
	Super::ActionOverlapEvent(User);

	if (!IsValid(BaseGameState))
	{
		return;
	}

	BaseGameState->AddScore(PointValue);
	BaseGameState->IncreaseCurrentCoin();
	BaseGameState->PlayItemEffectSound(0, AudioComp);

	//AudioComp = BaseGameState->PlayItemEffectSound(0, this);

	/*GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			BaseGameState->StopItemEffectSound(AudioComp);
			DestroyItem();
		}), 0.3f, false);*/

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroyItem, 0.3f, false);
	//DestroyItem();
}
