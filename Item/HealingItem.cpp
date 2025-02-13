// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HealingItem.h"
#include "Player/PlayerCharacter.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActionOverlapEvent(AActor* User)
{
	Super::ActionOverlapEvent(User);

	Cast<APlayerCharacter>(User)->AddHp(HealAmount);
	
	if (!IsValid(BaseGameState))
	{
		return;
	}

	BaseGameState->PlayItemEffectSound(1, AudioComp);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroyItem, 0.3f, false);
	//DestroyItem();
}
