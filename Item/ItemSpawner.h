// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

class UBoxComponent;
class ABaseGameState;

UCLASS()
class SPARTAPRJ06_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemSpawner();
	void SetLevelValue(int32 Value);
	void SpawnItems(int32 WaveValue);
	int32 GetCoinItemCount();

protected:
	virtual void BeginPlay() override;
	FVector CalculateRandomPos();
	AActor* SelectSpawnItem(FVector& Location);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBoxComponent> BoxCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawn")
	TSubclassOf<AActor> SmallCoin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawn")
	TSubclassOf<AActor> BigCoin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawn")
	TSubclassOf<AActor> HealingPotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawn")
	TSubclassOf<AActor> ExplosionMine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawn")
	TSubclassOf<AActor> Slowing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawn")
	TSubclassOf<AActor> Reverse;
	UPROPERTY(EditAnywhere, Category = "ItemSpawn")
	int32 SpawnCount = 40;

protected:
	TObjectPtr<ABaseGameState> BaseGameState;
	const float CoinSpawnRate = 0.4;
	int32 CoinItemCount = 0;
	int32 LevelValue = 0;
};
