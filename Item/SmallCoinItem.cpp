// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SmallCoinItem.h"

ASmallCoinItem::ASmallCoinItem()
{
	ItemType = "SmallCoin";
	PointValue = 50;
}

void ASmallCoinItem::ActionOverlapEvent(AActor* User)
{
	Super::ActionOverlapEvent(User);
}
