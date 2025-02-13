// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	ItemType = "BigCoin";
	PointValue = 100;
}

void ABigCoinItem::ActionOverlapEvent(AActor* User)
{
	Super::ActionOverlapEvent(User);
}
