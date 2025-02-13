// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/DecressScoreItem.h"
#include "ReverseControlItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPRJ06_API AReverseControlItem : public ADecressScoreItem
{
	GENERATED_BODY()
	
public:
	virtual void ActionOverlapEvent(AActor* User) override;

protected:
	int32 ReverseTime = 5.0f;
};
