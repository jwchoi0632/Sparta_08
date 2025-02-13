// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "CoinItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPRJ06_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()
	
public:

protected:
	int32 PointValue;
	
	virtual void ActionOverlapEvent(AActor* User) override;
};
