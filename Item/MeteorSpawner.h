// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorSpawner.generated.h"

class UBoxComponent;

UCLASS()
class SPARTAPRJ06_API AMeteorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeteorSpawner();
	void StartSpawn(int32 LevelValue);
	void StopSpawn();

protected:
	virtual void BeginPlay() override;
	FVector CalculateRandomPos();
	void SpawnMeteor();

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBoxComponent> BoxCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> Meteor;

protected:
	float SpawnTime = 4.0f;
	int32 LevelNum = 0;
	FTimerHandle SpawnTimer;
};
