// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "ExplosionItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPRJ06_API AExplosionItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AExplosionItem();

protected:
	virtual void ActionOverlapEvent(AActor* User) override;
	void Explosion();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USphereComponent> ExplosionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	float ExplosionDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	float ExplosionRadius = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	int32 ExplosionDamage = 20;

protected:

};
