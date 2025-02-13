// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstaclesSpawner.generated.h"

class ABaseGameState;

UCLASS()
class SPARTAPRJ06_API AObstaclesSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AObstaclesSpawner();
	void SpawnObstacles(int32 LevelNum);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SpawnObstaclesLevel1();
	void SpawnObstaclesLevel2();
	void SpawnObstaclesLevel3();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> Obstacle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> Empty;

	TObjectPtr<ABaseGameState> BaseGameState;

	FVector StartLocation = FVector::ZeroVector;
	float UpSpeed = 10;
	float TargetDestance = 500;
	bool bIsStart = false;
};
