// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "DecressScoreItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPRJ06_API ADecressScoreItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	virtual void ActionOverlapEvent(AActor* User) override;

protected:
	int32 PointValue = 100;
};
