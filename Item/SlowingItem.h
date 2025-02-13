// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/DecressScoreItem.h"
#include "SlowingItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPRJ06_API ASlowingItem : public ADecressScoreItem
{
	GENERATED_BODY()
	
public:
	virtual void ActionOverlapEvent(AActor* User) override;

protected:
	float SlowRate = 0.3f;
	float SlowTime = 3.0f;
};
